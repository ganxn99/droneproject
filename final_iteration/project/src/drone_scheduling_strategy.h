#ifndef DRONE_SCHEDULING_STRATEGY_H_
#define DRONE_SCHEDULING_STRATEGY_H_

#include "src/drone.h"
#include "src/package.h"
#include "src/customer.h"
#include <vector>

namespace csci3081 {

/// @brief An abstract strategy class for implementing drone scheduling strategies.
class DroneSchedulingStrategy {
 public:
  /**
   * @brief Pure virutal method to scores a drone based on a strategy.
   *
   * @param[in] drone Drone - drone object to be scored
   * @param[in] package Package - package object
   *
   * @return float score rating for the drone depending on a strategy.
   */
  virtual float Score(Drone* drone, Package* package) = 0;
};

}  // namespace csci3081

#endif  // DRONE_SCHEDULING_STRATEGY_H_
