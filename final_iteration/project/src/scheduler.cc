#include "src/scheduler.h"
#include "src/drone.h"
#include "src/customer.h"
#include "src/package.h"
#include <algorithm>
#include "src/float_vector_functions.h"
#include "src/json_helper.h"
#include "src/drone_pool.h"
#include "src/drone_scheduling_strategy.h"
#include "src/composite_scheduling_strategy.h"
#include "src/time_strategy.h"
#include "src/potential_distance_strategy.h"
#include "src/weight_capacity_strategy.h"

using entity_project::Entity;

namespace csci3081 {

std::vector<entity_project::Entity*>* Scheduler::entities_;

void Scheduler::ScheduleDelivery(entity_project::Package* package,
  entity_project::Customer* dest, const picojson::object& details,
  std::vector<entity_project::Entity*>* entities) {
  // this assignment is redundant after the first time, but necessary for google tests (bc static)
  entities_ = entities;

  // ensure the package is actually part of the simulation (if it's not then return)
  if (std::find(entities_->begin(), entities_->end(), package) == entities_->end()) {
    return;
  }

  // ensure the package has not already been scheduled
  if (package->AsType<Package>()->IsDynamic()) {
    return;
  }

  // set the package's details
  if (JsonHelper::ContainsKey(details, "priority")) {
    package->AsType<Package>()->SetPriority(JsonHelper::GetString(details, "priority"));
  }
  if (JsonHelper::ContainsKey(details, "minDeliveryTime")) {
    package->AsType<Package>()->SetMinDeliveryTime(JsonHelper::GetDouble(details,
                                                                                "minDeliveryTime"));
  }

  // set the package's destination
  package->AsType<Package>()->SetDestination(dest);

  // if the package is right next to the customer, deliver it immediately without using a drone
  float packageToCustomerDist = FloatVectorFunctions::Distance(package->GetPosition(),
                                                               dest->GetPosition());
  if (packageToCustomerDist <= package->GetRadius() + dest->GetRadius()) {
    package->AsType<Package>()->SetBeenDelivered(true);
    return;
  }

  // otherwise, perform the scheduling algorithm
  ScheduleAlgorithm(package);
}

void Scheduler::RescheduleDelivery(entity_project::Package* package) {
  // perform the scheduling algorithm
  ScheduleAlgorithm(package);
}

void Scheduler::ScheduleAlgorithm(entity_project::Package* package) {
  // populate drone pool with all drones that could ever carry the package (i.e. those whose
  //  max weight capacity is greater than the package weight and aren't out of battery)
  DronePool dronePool;
  for (int i = 0; i < entities_->size(); i++) {
    Drone* drone = (*entities_)[i]->AsType<Drone>();
    if (drone) {
      // ensure drones base max weight capacity is at least high enough to carry the package
      //  and also ensure that that the drone has any battery left
      if (drone->GetMaxCapacity() >= package->GetWeight() && drone->GetRemainingBattery() > 0) {
        dronePool.AddDrone(drone);
      }
    }
  }

  // return without scheduling if there are no drones (with high enough weight capacity and battery)
  //  in the simulation
  if (dronePool.GetDrones().empty()) {
    return;
  }

  /*************************************************************************************************
                                Scheduling Algortihm
  *************************************************************************************************/
  // if there are drones in the simulation, choose the best drone to assign deliver based on a
  //  composite strategy pattern that scores each drone. The drone with the highest score will
  //  be assigned the delivery, and if no drone can pick up the package then no drone will be
  //  assigned to delivery.

  // create a composite scheduling strategy
  CompositeSchedulingStrategy* schedulingStrategy = new CompositeSchedulingStrategy();
  schedulingStrategy->AddSchedulingStrategy(new PotentialDistanceStrategy());
  schedulingStrategy->AddSchedulingStrategy(new TimeStrategy());
  // schedulingStrategy->AddSchedulingStrategy(new WeightCapacityStrategy());

  // set the drone pool's scheduling strategy to this composite strategy
  dronePool.SetSchedulingStrategy(schedulingStrategy);
  // score all the drones in the pool based on the strategy
  dronePool.ScoreAllDrones(package->AsType<Package>());

  // find the drone with the maximum score
  float maxScore = dronePool.GetDrones()[0]->GetScore();
  Drone* maxScoreDrone = dronePool.GetDrones()[0];
  for (int i = 1; i < dronePool.GetDrones().size(); i++) {
    Drone* currentDrone = dronePool.GetDrones()[i];
    if (currentDrone->GetScore() > maxScore) {
      maxScore = currentDrone->GetScore();
      maxScoreDrone = currentDrone;
    }
  }

  // if the max score wasn't negative (drone can't deliver package) then assign it to delivery
  if (maxScore >= 0) {
    // set the package to dynamic as it is now going to be scheduled for delivery
    package->AsType<Package>()->SetIsDynamic(true);

    // notify the package observer with the "scheduled" notification.
    picojson::value notification = package->AsType<Package>()->GetNotification(0);
    package->AsType<Package>()->Notify(notification, *package);

    // add the delivery to the drone
    maxScoreDrone->AddDelivery(package->AsType<Package>());
  }
}

}  // namespace csci3081
