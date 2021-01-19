#ifndef WEIGHT_CAPACITY_STRATEGY_H_
#define WEIGHT_CAPACITY_STRATEGY_H_

#include "src/drone.h"
#include "src/package.h"
#include "src/customer.h"
#include <vector>

namespace csci3081 {

/// @brief Scheduling strategy that determines score based off of drone's total weight capacity.
class WeightCapacityStrategy : public DroneSchedulingStrategy {
 public:
  /**
   * @brief Scores a drone based on its (weighted) total weight capacity. 
   *
   * @param[in] drone Drone - drone object to be scored
   * @param[in] package Package - package object
   *
   * @return float score rating for the drone given its total weight capacity.
   */
  float Score(Drone* drone, Package* package) {
    return drone->GetRemainingCapacity() / weight;
  }

 private:
  float weight = 20;  //!< Weight used to scale importance of weight capacity in this strategy.
};

}  // namespace csci3081

#endif  // WEIGHT_CAPACITY_STRATEGY_H_
