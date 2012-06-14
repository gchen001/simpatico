#ifndef DDMD_PAIR_POTENTIAL_TEST_H
#define DDMD_PAIR_POTENTIAL_TEST_H

#include <ddMd/potentials/pair/PairPotential.h>
#include <ddMd/potentials/pair/PairPotentialImpl.h>
#include <ddMd/storage/AtomStorage.h>
#include <ddMd/storage/AtomIterator.h>
#include <ddMd/storage/GhostIterator.h>
#include <ddMd/communicate/Domain.h>
#include <util/boundary/Boundary.h>
#include <ddMd/chemistry/Atom.h>

#include <inter/pair/DpdPair.h>

#include <util/random/Random.h>

#include <test/UnitTest.h>
#include <test/UnitTestRunner.h>
#include <test/ParamFileTest.h>

using namespace Util;
using namespace Inter;
using namespace DdMd;

class PairPotentialTest: public ParamFileTest< PairPotentialImpl<DpdPair> >
{

private:

      Boundary    boundary;
      Domain      domain;
      AtomStorage storage;

public:

   virtual void setUp()
   {
      object().setNAtomType(1);
      object().associate(domain, boundary, storage);

      // Read parameter file
      openFile("in/PairPotential");
      domain.readParam(file());
      storage.readParam(file());
      object().readParam(file());
      closeFile();
   }

   void readAtoms(const char* filename)
   {

      int atomCount = 0; // Number to be distributed by master
      int i;
      Vector  pos;
      Atom*   ptr;

      std::ifstream atomposfile;
      atomposfile.open(filename);

      // Read Max # of atoms to be distributed.
      atomposfile >> atomCount;

      //std::cout << std::endl;
      //std::cout << "Num Atoms to be distributed = " << atomCount << std::endl;

      // Read atoms 
      for(i = 0; i < atomCount; ++i) {
         ptr = storage.newAtomPtr();
         ptr->setId(i);
         ptr->setTypeId(0);

         //Read a position from file.
         atomposfile >> ptr->position();

         //Use position vector for velocity for now
         ptr->velocity() = ptr->position();

         storage.addNewAtom();

      }
      file().close();

   }

   void randomAtoms(int nAtom, const Vector& lower, const Vector& upper, 
                    double cutoff)
   {

      // Set coordinate bounds
      Vector  lowerGhost;
      Vector  upperGhost;
      int    i, j;
      for (i = 0; i < Dimension; ++i) {
          lowerGhost[i] = lower[i] - cutoff;
          upperGhost[i] = upper[i] + cutoff;
      }

      // Create new random number generator
      Random random;
      random.setSeed(20);

      // Place atoms at random in extended region
      Vector pos;
      Atom*  ptr;
      bool   ghost;
      for (i = 0; i < nAtom; ++i) {
         ghost = false;
         for (j = 0; j < Dimension; ++j) {
            pos[j] = random.uniform(lowerGhost[j], upperGhost[j]);
            if (pos[j] < lower[j]) 
               ghost = true;
            if (pos[j] > upper[j]) 
               ghost = true;
            TEST_ASSERT(pos[j] >= lowerGhost[j]);
            TEST_ASSERT(pos[j] <= upperGhost[j]);
         }
         if (ghost) {
            ptr = storage.newGhostPtr();
            ptr->setId(i);
            ptr->setTypeId(0);
            ptr->position() = pos;
            storage.addNewGhost();
         } else {
            ptr = storage.newAtomPtr();
            ptr->setId(i);
            ptr->setTypeId(0);
            ptr->position() = pos;
            storage.addNewAtom();
         }
         std::cout << pos << "   " << ghost << std::endl;
      }
   }

   void zeroForces()
   {
      AtomIterator  atomIter;
      GhostIterator ghostIter;

      storage.begin(atomIter);
      for ( ; atomIter.notEnd(); ++atomIter) {
         atomIter->force().zero();
      }
      storage.begin(ghostIter);
      for ( ; ghostIter.notEnd(); ++ghostIter) {
         ghostIter->force().zero();
      }

   }

   void writeForces()
   {
      std::cout << std::endl;

      AtomIterator  atomIter;
      storage.begin(atomIter);
      for ( ; atomIter.notEnd(); ++atomIter) {
         std::cout << atomIter->force() << "  " << 0 << std::endl;
      }

      #if 0
      GhostIterator ghostIter;
      storage.begin(ghostIter);
      for ( ; ghostIter.notEnd(); ++ghostIter) {
         std::cout << ghostIter->force() << "  " << 1 << std::endl;
      }
      #endif

   }

   #if 0
   void saveForces()
   {
      AtomIterator  atomIter;
      GhostIterator ghostIter;

      storage.begin(atomIter);
      for ( ; atomIter.notEnd(); ++atomIter) {
         force_[i] = atomIter->force();
      }
      storage.begin(ghostIter);
      for ( ; ghostIter.notEnd(); ++ghostIter) {
         force_[i] = ghostIter->force();
      }

   }
   #endif

   void testRead1()
   {
      printMethod(TEST_FUNC);
      readAtoms("in/positions1");
     
      TEST_ASSERT(storage.nAtom()  == 3); 
      TEST_ASSERT(storage.nGhost() == 0); 

      AtomIterator iter;
      std::cout << std::endl;
      storage.begin(iter);
      for ( ; iter.notEnd(); ++iter) {
         std::cout << iter->position() << std::endl;
      }      

      object().setMethodId(2); // N^2 loop
      object().addForces();

      std::cout << std::endl;
      storage.begin(iter);
      for ( ; iter.notEnd(); ++iter) {
         std::cout << iter->force() << std::endl;
      }      
   }

   void testRandom1()
   {
      printMethod(TEST_FUNC);

      const int nAtom = 120;
      const int pairCapacity = 1000;
      double cutoff   = 1.2;
      Vector  lower(0.0);
      Vector  upper(2.0, 3.0, 4.0);
      randomAtoms(nAtom, lower, upper, cutoff);

      object().initialize(lower, upper, cutoff, pairCapacity);
      object().findNeighbors(lower, upper);

      zeroForces();
      object().setMethodId(0);
      object().addForces();
      writeForces();
 
      zeroForces();
      object().setMethodId(1);
      object().addForces();
      writeForces();

      zeroForces();
      object().setMethodId(2);
      object().addForces();
      writeForces();

   }

};

TEST_BEGIN(PairPotentialTest)
TEST_ADD(PairPotentialTest, testRead1)
TEST_ADD(PairPotentialTest, testRandom1)
TEST_END(PairPotentialTest)

#endif 