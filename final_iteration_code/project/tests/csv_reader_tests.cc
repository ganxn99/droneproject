#include "gtest/gtest.h"
#include "src/csv_reader.h"

namespace csci3081 {

class CSVReaderTest : public ::testing::Test {
 protected:
  virtual void SetUp() {}
  virtual void TearDown() {}
};
/*******************************************************************************
 * Test Cases
 ******************************************************************************/

TEST_F(CSVReaderTest, ContainsModel) {
  csci3081::CSVReader csvReader;
  EXPECT_EQ(csvReader.ContainsModel("Q-36-01"), true);
  EXPECT_EQ(csvReader.ContainsModel("Q-36-02"), true);
  EXPECT_EQ(csvReader.ContainsModel("Q-36-03"), true);
  EXPECT_EQ(csvReader.ContainsModel("Q-36-04"), true);
  EXPECT_EQ(csvReader.ContainsModel("Q-36-05"), true);
  EXPECT_EQ(csvReader.ContainsModel("Q-36-06"), false);
}

TEST_F(CSVReaderTest, ReadWeight) {
  csci3081::CSVReader csvReader;
  EXPECT_EQ(csvReader.ReadWeight("Q-36-01"), 50);
  EXPECT_EQ(csvReader.ReadWeight("Q-36-02"), 200);
  EXPECT_EQ(csvReader.ReadWeight("Q-36-03"), 5);
  EXPECT_EQ(csvReader.ReadWeight("Q-36-04"), 120);
  EXPECT_EQ(csvReader.ReadWeight("Q-36-05"), 150);
}

TEST_F(CSVReaderTest, ReadMaxSpeed) {
  csci3081::CSVReader csvReader;
  EXPECT_EQ(csvReader.ReadMaxSpeed("Q-36-01"), 60);
  EXPECT_EQ(csvReader.ReadMaxSpeed("Q-36-02"), 55);
  EXPECT_EQ(csvReader.ReadMaxSpeed("Q-36-03"), 40);
  EXPECT_EQ(csvReader.ReadMaxSpeed("Q-36-04"), 30);
  EXPECT_EQ(csvReader.ReadMaxSpeed("Q-36-05"), 60);
}

TEST_F(CSVReaderTest, ReadAcceleration) {
  csci3081::CSVReader csvReader;
  EXPECT_EQ(csvReader.ReadAcceleration("Q-36-01"), 4);
  EXPECT_EQ(csvReader.ReadAcceleration("Q-36-02"), 4.5);
  EXPECT_EQ(csvReader.ReadAcceleration("Q-36-03"), 5);
  EXPECT_EQ(csvReader.ReadAcceleration("Q-36-04"), 6);
  EXPECT_EQ(csvReader.ReadAcceleration("Q-36-05"), 3);
}

TEST_F(CSVReaderTest, ReadWeightCapacity) {
  csci3081::CSVReader csvReader;
  EXPECT_EQ(csvReader.ReadWeightCapacity("Q-36-01"), 10);
  EXPECT_EQ(csvReader.ReadWeightCapacity("Q-36-02"), 20);
  EXPECT_EQ(csvReader.ReadWeightCapacity("Q-36-03"), 10);
  EXPECT_EQ(csvReader.ReadWeightCapacity("Q-36-04"), 19);
  EXPECT_EQ(csvReader.ReadWeightCapacity("Q-36-05"), 15);
}

TEST_F(CSVReaderTest, ReadBatteryCapacity) {
  csci3081::CSVReader csvReader;
  EXPECT_EQ(csvReader.ReadBatteryCapacity("Q-36-01"), 300);
  EXPECT_EQ(csvReader.ReadBatteryCapacity("Q-36-02"), 10);
  EXPECT_EQ(csvReader.ReadBatteryCapacity("Q-36-03"), 30);
  EXPECT_EQ(csvReader.ReadBatteryCapacity("Q-36-04"), 600);
  EXPECT_EQ(csvReader.ReadBatteryCapacity("Q-36-05"), 120);
}

}  // namespace csci3081
