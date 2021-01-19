#include "gtest/gtest.h"
#include "src/float_vector_functions.h"
#include <vector>

namespace csci3081 {

using csci3081::FloatVectorFunctions;

class FloatVectorFunctionsTests : public ::testing::Test {
 protected:
  virtual void SetUp() {}
  virtual void TearDown() {}

  std::vector<float> zeroVector{0, 0, 0};
  std::vector<float> unitVector{1, 0, 0};
  std::vector<float> defaultVector{3, 4, 0};
  FloatVectorFunctions floatVectorFunctions;
};
/*******************************************************************************
 * Test Cases
 ******************************************************************************/

// vector distance test
TEST_F(FloatVectorFunctionsTests, VectorDistanceTest) {
  EXPECT_EQ(floatVectorFunctions.Distance(zeroVector, zeroVector), 0);
  EXPECT_EQ(floatVectorFunctions.Distance(zeroVector, unitVector), 1);
  EXPECT_EQ(floatVectorFunctions.Distance(zeroVector, defaultVector), 5);
}

// vector magnitude test
TEST_F(FloatVectorFunctionsTests, VectorMagnitudeTest) {
  EXPECT_EQ(floatVectorFunctions.Magnitude(zeroVector), 0);
  EXPECT_EQ(floatVectorFunctions.Magnitude(unitVector), 1);
  EXPECT_EQ(floatVectorFunctions.Magnitude(defaultVector), 5);
}

}  // namespace csci3081
