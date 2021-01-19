#ifndef CSV_READER_H_
#define CSV_READER_H_

#include <string>
#include <map>
#include <vector>

namespace csci3081 {

/// @brief Able to read and store data from "planet-x.csv" so that drone can access it more easily.
/**
 * This class is not a general csv file reader, and instead acts only on "planet-x.csv" for the sole
 *  purpose of making drone initialization with the Planet X Drone Fleet more straightforward. It
 *  does, however, adapt to row/column swaps in the csv file, duplicate model names, and live
 *  run-time changes and read correctly regardless.
 */
class CSVReader {
 public:
  /**
   * @brief Returns whether or not the csv file contains information for specified model.
   *
   * @param[in] model string - model of drone
   *
   * @return True or false depending on whether drone model is specified in csv.
   */
  bool ContainsModel(std::string model);
  /**
   * @brief Returns the weight of specified drone model.
   *
   * @param[in] model string - model of drone
   *
   * @return Weight of specified model of drone.
   */
  float ReadWeight(std::string model);
  /**
   * @brief Returns the max speed of specified drone model.
   *
   * @param[in] model string - model of drone
   *
   * @return Max speed of specified model of drone.
   */
  float ReadMaxSpeed(std::string model);
  /**
   * @brief Returns the base accleration of specified drone model.
   *
   * @param[in] model string - model of drone
   *
   * @return Base accleration of specified model of drone.
   */
  float ReadAcceleration(std::string model);
  /**
   * @brief Returns the weight capacity of specified drone model.
   *
   * @param[in] model string - model of drone
   *
   * @return Weight capacity of specified model of drone.
   */
  float ReadWeightCapacity(std::string model);
  /**
   * @brief Returns the battery capacity of specified drone model.
   *
   * @param[in] model string - model of drone
   *
   * @return Battery capacity of specified model of drone.
   */
  float ReadBatteryCapacity(std::string model);

 private:
  /**
   * @brief Caches the csv file into a map so that it can be more efficiently accesed in the future.
   */
  void CacheCSV();
  /**
   * Map representing cached csv file. Format:\n
   *  key -> model\n
   *  val -> vector of weight, max speed, acceleration, etc.\n
   */
  std::map<std::string, std::vector<float>> cachedCSV;
  int weightColumn;  //!< The column in the csv file that stores mass info.
  int weightCapacityColumn;  //!< The column in the csv file that stores weight capacity info.
  int maxSpeedColumn;  //!< The column in the csv file that stores max speed info.
  int accelerationlColumn;  //!< The column in the csv file that stores acceleration info.
  int batteryCapacityColumn;  //!< The column in the csv file that stores battery info.
  int csvColumnSize = 6;  //!< The number of columns in the csv file..
};

}  // namespace csci3081

#endif  // CSV_READER_H_
