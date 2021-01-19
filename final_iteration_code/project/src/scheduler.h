#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "EntityProject/ANVIL/customer.h"
#include "EntityProject/ANVIL/package.h"
#include "src/route_manager.h"
#include <vector>

namespace csci3081 {

/// Class in charge of scheduling deliveries.
/**
 * This class manages scheduling of Package deliveries to specified customers and
 *  is foundational to any actual simulation of packages, deliveries and customers ocurring.
 *  This class can intelligently handle the scheduling of multiple packages for multiple drones,
 *  prioritizing based on drone availability, battery capacity, weight capacity, and the
 *  distance to packages, all at run-time.
 */
class Scheduler {
 public:
  /**
   * @brief Schedules delivery of a specified package to a specified customer.
   *
   * As described in DroneSimulation, this method follows the following principles to schedule
   *  packages.\n
   * - If the package is not in the simulation, the delivery will not be scheduled.\n
   * - If the package has already been scheduled, it will not be scheduled again.\n
   * - Otherwise, the package follows the following scheduling schema:
   * [#1] - if there are avaible drones, the drone that is closest to the package is selected
   * [#2] - if no available drones, chose an occupied drone (ideally) close to the package who
   *  has weight capacity and battery left to pick up the package.
   * [#3] - if no available or occupied drones, add package to queue of drone who will be the
   *  closest closest after it completes all of it's scheduled routes
   *
   * @param[in] package Package object - package scheduled to be delivered
   * @param[in] dest Customer object - customer scheduled to recieve delivery
   * @param[in] details JSON object - details of the delivery
   * @param[in] entities_ entity_project::Entity object array - pointer to all entities in the
   *  simulation
   *
   * @return Nothing.
   */
  void ScheduleDelivery(entity_project::Package* package,
    entity_project::Customer* dest, const picojson::object& details,
    std::vector<entity_project::Entity*>* entities);

  /**
   * @brief Reschedules delivery of a dropped package.
   *
   * @param[in] package Package object - the (dropped) package that needs to be rescheduled
   *
   * @return Nothing.
   */
  void RescheduleDelivery(entity_project::Package* package);

 private:
  /**
   * @brief Does the actually scheduling. Is shared by Schedule and Reschedule methods to limit
   *  redundant code.
   *
   * @param[in] package Package object - package scheduled to be delivered
   *
   * @return Nothing.
   */
  void ScheduleAlgorithm(entity_project::Package* package);
  /**
   * @brief pointer to Drone Simulations entities vector. Used to have information about the drones in
   *  the system when rescheduling.
   */
  static std::vector<entity_project::Entity*>* entities_;
};

}  // namespace csci3081

#endif  // SCHEDULER_H_
