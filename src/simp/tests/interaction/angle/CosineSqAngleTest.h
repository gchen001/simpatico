#ifndef COSINE_SQ_ANGLE_TEST_H
#define COSINE_SQ_ANGLE_TEST_H

#include <simp/interaction/angle/CosineSqAngle.h>
#include "AngleTestTemplate.h"

class CosineSqAngleTest : public AngleTestTemplate<CosineSqAngle>
{
public:

   using AngleTestTemplate<CosineSqAngle>::readParamFile;

   void setUp()
   {
      eps_ = 1.0E-5;
      setNAngleType(1);  
      readParamFile("in/CosineSqAngle");  
   } 

   void testForce()
   {
      printMethod(TEST_FUNC);
      b1_ = Vector( 0.2, 0.90,  0.3);
      b2_ = Vector(-0.1, 0.85, -0.4);
      type_ = 0;
      forceTest();
   }

};

TEST_BEGIN(CosineSqAngleTest)
TEST_ADD(CosineSqAngleTest, testForce)
TEST_END(CosineSqAngleTest)

#endif
