#ifndef DRONE_POOL_H_
#define DRONE_POOL_H_

#include "src/drone.h"
#include "src/package.h"
#include "src/customer.h"
#include <vector>
#include "src/drone_scheduling_strategy.h"

namespace csci3081 {

/// @brief Drone pool acts as "Context" for the strategy pattern involved in scheduling drone
///  deliveries.
/*
 * @brief Contains a pool of drones and can score all of them at once based on the currently
 *  assigned scheduling strategy.
 */
class DronePool {
 public:
    /**
     * @brief Sets the currently used scheduling strategy.
     *
     * @param[in] schedulingStrategy DroneSchedulingStrategy - new scheduling strategy to use
     *
     * @return Nothing.
     */
    void SetSchedulingStrategy(DroneSchedulingStrategy *schedulingStrategy) {
      schedulingStrategy_ = schedulingStrategy;
    }
    /**
     * @brief Adds a drone to the drone pool.
     *
     * @param[in] drone Drone - drone to be added to pool
     *
     * @return Nothing.
     */
    void AddDrone(Drone* drone) {
      drones.push_back(drone);
    }
    /**
     * @brief Gets the drone pool.
     *
     * @return vector array of drones in the drone pool.
     */
    std::vector<Drone*> GetDrones() {
      return drones;
    }
    /**
     * @brief Scores all drones in the pool based off of the currently used strategy.
     *
     * @param[in] package Package - package object to score drones off of
     *
     * @return Sets the score member of each drone using the current scheduling strategy.
     */
    void ScoreAllDrones(Package* package);

 private:
    std::vector<Drone*> drones;  //!< All the drones in the pool.
    DroneSchedulingStrategy* schedulingStrategy_;  //!< The currently used scheduling strategy.
};

}  // namespace csci3081

#endif  // DRONE_POOL_H_
