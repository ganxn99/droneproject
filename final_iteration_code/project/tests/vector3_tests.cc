#include "gtest/gtest.h"
#include "src/vector3.h"
#include <cmath>

namespace csci3081 {

using csci3081::Vector3;

class Vector3Tests : public ::testing::Test {
 protected:
  virtual void SetUp() {}
  virtual void TearDown() {}
  Vector3 v1;
  Vector3 v2{1, 2, 3};
  Vector3 v3{0.1, -0.2, 3.1};
};
/*******************************************************************************
 * Test Cases
 ******************************************************************************/

TEST_F(Vector3Tests, VectorAddition) {
  Vector3 v = v2 + v3;
  EXPECT_LT(abs(v.GetX() - 1.1), 0.001);
  EXPECT_LT(abs(v.GetY() - 1.8), 0.001);
  EXPECT_LT(abs(v.GetZ() - 6.1), 0.001);
}

TEST_F(Vector3Tests, VectorSubtraction) {
  Vector3 v = v2 - v3;
  EXPECT_LT(abs(v.GetX() - 0.9), 0.001);
  EXPECT_LT(abs(v.GetY() - 2.2), 0.001);
  EXPECT_LT(abs(v.GetZ() + 0.1), 0.001);
}

TEST_F(Vector3Tests, ScalarMultiplication) {
  Vector3 v = v2 * 2;
  EXPECT_LT(abs(v.GetX() - 2), 0.001);
  EXPECT_LT(abs(v.GetY() - 4), 0.001);
  EXPECT_LT(abs(v.GetZ() - 6), 0.001);
}

TEST_F(Vector3Tests, ScalarDivision) {
  Vector3 v = v2 / 2;
  EXPECT_LT(abs(v.GetX() - 0.5), 0.001);
  EXPECT_LT(abs(v.GetY() - 1), 0.001);
  EXPECT_LT(abs(v.GetZ() - 1.5), 0.001);
}

TEST_F(Vector3Tests, Distance) {
  Vector3 v{3, 4, 0};
  Vector3 v0{0, 0, 0};
  EXPECT_LT(abs(v.Distance(v0) - 5), 0.001);
}

TEST_F(Vector3Tests, Magnitude) {
  Vector3 v{3, 4, 0};
  EXPECT_LT(abs(v.Magnitude() - 5), 0.001);
}

}  // namespace csci3081
