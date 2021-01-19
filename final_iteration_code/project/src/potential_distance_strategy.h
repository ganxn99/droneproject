#ifndef POTENTIAL_DISTANCE_STRATEGY_H_
#define POTENTIAL_DISTANCE_STRATEGY_H_

#include "src/drone_scheduling_strategy.h"
#include "src/drone.h"
#include "src/package.h"
#include "src/customer.h"
#include <vector>

namespace csci3081 {

/// @brief Scheduling strategy that determines score based off of the distance a drone can
///  travel given its speed and battery life.
class PotentialDistanceStrategy : public DroneSchedulingStrategy {
 public:
    /**
     * @brief Scores a drone based on its (weighted) distance to a specified package.
     *
     * @param[in] drone Drone - drone object to be scored
     * @param[in] package Package - package object
     *
     * @return float score rating for the drone given its distance to the package.
     */
    float Score(Drone* drone, Package* package) {
        return drone->GetMaxSpeed() * drone->GetRemainingBattery() / weight;
    }

 private:
    float weight = 18000;  //!< Weight used to scale importance of distance in this strategy.
};

}  // namespace csci3081

#endif  // POTENTIAL_DISTANCE_STRATEGY_H_
