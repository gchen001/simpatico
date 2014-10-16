#ifndef DDMD_ATOM_CONTEXT_H
#define DDMD_ATOM_CONTEXT_H

/*
* Simpatico - Simulation Package for Polymeric and Molecular Liquids
*
* Copyright 2010 - 2012, The Regents of the University of Minnesota
* Distributed under the terms of the GNU General Public License.
*/

namespace DdMd{
   /**
   * Descriptor for context of an Atom within a molecule and species.
   *
   * The information in the AtomContext struct is not used by any 
   * of the essential algorithms of ddSim, but may be used by some
   * analyzers or modifiers.
   */
   struct AtomContext{

      /**
      * Index of the species of molecule.
      */
      int speciesId;

      /**
      * Index of molecule within its molecular species.
      */
      int moleculeId;

      /**
      * Index of atom within its parent molecule.
      *
      * Note: This takes on values from zero to one less than the 
      * number of of atoms within the molecule. Corresponding atoms 
      * in identical molecules have the same atomId. This index is
      * thus distinct from the global atom id returned by Atom::id(), 
      * which is unique within the system.
      */
      int atomId;

      /**
      * Bit mask indicating what atom groups this atom belongs to.
      *
      * This unsigned int is a bit mask in which individual bits
      * can be set, unset and queries using the Util:Bitt class.
      * Each bit is set if this atom belongs to a corresponding
      * group of atoms.
      */
      unsigned int groupMask;

      /**
      * Set all data members to null default values. 
      */
      void clear();

   };

   /*
   * Set all data members to null default values. 
   */
   inline void AtomContext::clear()
   {
      speciesId = -1;
      moleculeId = -1;
      atomId = -1;
      groupMask = 0;
   }

}
#endif
