#include "src/csv_reader.h"
#include <fstream>
#include <sstream>
#include <utility>
#include <iostream>

namespace csci3081 {

bool CSVReader::ContainsModel(std::string model) {
  // cache the csv file if it hasn't been cached already
  if (cachedCSV.empty()) {
    CacheCSV();
  }
  // return true if csv contains this model
  return (cachedCSV.find(model) != cachedCSV.end());
}

float CSVReader::ReadWeight(std::string model) {
  // cache the csv file if it hasn't been cached already
  if (cachedCSV.empty()) {
    CacheCSV();
  }
  // find weight attached to this model
  return cachedCSV.at(model)[weightColumn];
}

float CSVReader::ReadMaxSpeed(std::string model) {
  // cache the csv file if it hasn't been cached already
  if (cachedCSV.empty()) {
    CacheCSV();
  }
  // find mas speed attached to this model
  return cachedCSV.at(model)[maxSpeedColumn];
}

float CSVReader::ReadAcceleration(std::string model) {
  // cache the csv file if it hasn't been cached already
  if (cachedCSV.empty()) {
    CacheCSV();
  }
  // find acceleration attached to this model
  return cachedCSV.at(model)[accelerationlColumn];
}

float CSVReader::ReadWeightCapacity(std::string model) {
  // cache the csv file if it hasn't been cached already
  if (cachedCSV.empty()) {
    CacheCSV();
  }
  // find weight capacity attached to this model
  return cachedCSV.at(model)[weightCapacityColumn];
}

float CSVReader::ReadBatteryCapacity(std::string model) {
  // cache the csv file if it hasn't been cached already
  if (cachedCSV.empty()) {
    CacheCSV();
  }
  // find battery capacity attached to this model
  return cachedCSV.at(model)[batteryCapacityColumn];
}

void CSVReader::CacheCSV() {
  // open csv file
  std::ifstream csvFile("data/planet-x.csv");

  // if open was successful, store its contents in the cachedCSV map
  if (csvFile) {
    // get a stream for the first row (header) of the csv and deliminate it with ','
    std::string row;
    std::getline(csvFile, row);
    std::istringstream buffer(row);
    std::string columnName;
    char delim = ',';  // deliminatoor

    // iterate through the header to get determine which column each piece of information is in
    int actualIndex = 0;  // the actual index/column in the header
    int modelActualIndex = 0;
    int weightActualIndex = 0;
    int maxSpeedActualIndex = 0;
    int accelerationActualIndex = 0;
    int weightCapacityActualIndex = 0;
    int batteryCapacityActualIndex = 0;
    int relativeIndex = 0;  // the relative index/column to what's been found so far
    while (relativeIndex < csvColumnSize) {
      // get the next column name and save it to columnName
      std::getline(buffer, columnName, delim);
      // assign columns based on columnName
      if (columnName == "Model #") {
        modelActualIndex = actualIndex;
        relativeIndex++;
      } else if (columnName == "Mass (kg)") {
        weightColumn = relativeIndex;
        weightActualIndex = actualIndex;
        relativeIndex++;
      } else if (columnName == "Max Speed (km/h)") {
        maxSpeedColumn = relativeIndex;
        maxSpeedActualIndex = actualIndex;
        relativeIndex++;
      } else if (columnName == "Base Acceleration (m/s^2)") {
        accelerationlColumn = relativeIndex;
        accelerationActualIndex = actualIndex;
        relativeIndex++;
      } else if (columnName == "WeightCapacity (kg)") {
        weightCapacityColumn = relativeIndex;
        weightCapacityActualIndex = actualIndex;
        relativeIndex++;
      } else if (columnName == "Base Battery Capacity (seconds)") {
        batteryCapacityColumn = relativeIndex;
        batteryCapacityActualIndex = actualIndex;
        relativeIndex++;
      }
      actualIndex++;
    }

    // iterate through each row of csv (after header) and add data to our cachedCSV map
    while (std::getline(csvFile, row)) {
      // get a stream for the current row
      std::istringstream buffer(row);
      std::string data;  // will store individual pieces of data found in the csv file

      // create a string/float pair to store model and related model information found on this row
      std::vector<float> modelData(csvColumnSize);
      std::pair<std::string, std::vector<float>> modelInformation("", modelData);

      // iterate through the data in this row and add it to the map
      for (int i = 0, j = 0; i < csvColumnSize; j++) {
        // get the next piece of data in the row
        std::getline(buffer, data, delim);
        // if we're currently in the column corresponding to "Model", we should set the key of our
        //  key-value pair to that model name
        //
        // otherwise, let's add the data that we've found to the appropriate location in the
        //  value portion of our key-value pair
        if (j == modelActualIndex) {
          modelInformation.first = data;
          i++;
        } else if (j == weightActualIndex || j == maxSpeedActualIndex
                  || j == accelerationActualIndex || j == weightCapacityActualIndex
                  || j == batteryCapacityActualIndex) {
          modelInformation.second[i] = stof(data);
          i++;
        }
      }
      // if we haven't already inserted information about this model yet,
      //  insert this row's information in key-value pair form to our map
      if (cachedCSV.find(modelInformation.first) == cachedCSV.end()) {
        cachedCSV.insert(modelInformation);
      }
    }

    // close the csv file
    csvFile.close();
  }
}

}  // namespace csci3081
