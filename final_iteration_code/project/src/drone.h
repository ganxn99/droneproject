#ifndef DRONE_H_
#define DRONE_H_

#include <EntityProject/ANVIL/drone.h>
#include "EntityProject/simple_UMN_route_manager.h"
#include "src/customer.h"
#include "src/package.h"
#include "src/route_manager.h"
#include "src/id_manager.h"
#include "src/float_vector_functions.h"
#include <queue>
#include <deque>
#include <tuple>
#include <vector>
#include <EntityProject/osm_graph_parser.h>
#include <EntityProject/graph.h>
#include <string>
#include <utility>

namespace csci3081 {

using entity_project::SimpleUMNRoutePoint;

/// @brief A drone entity object used for delivering packages in the simulation.
/**
 * The drone is one of the three major entity_project::Entity instances in this simulation
 *  (the other two being Customer and Package).  It has a unique id, a position, size and direction
 *  as well as the capabilites to move around and interact with observers. Additionally, the
 *  drone is responsible for calling route generation maintaining a target stack for all the targets
 *  it is scheduled to travel to.
 */
class Drone : public entity_project::Drone {
 public:
   /**
   * @brief The default constructor.
   *
   * Should not be used.
   */
  Drone() { AddType<Drone>(); }

  /**
  * @brief The parameter constructor.
  *
  * @param[in] val JSON object - contains all data necessary to initialize/render drone.
  */
  explicit Drone(const picojson::object& val);

  /**
   * @brief The destructor.
   */
  ~Drone() {}

  /**
  * @brief Returns the target destination of the current route the drone is following.
  *
  * @return entity_project::Entity that the drone currently traveling towards.
  */
  entity_project::Entity* GetCurrentTarget() { return currentTarget_; }


  /**
  * @brief Returns whether or not the drone is currently delivering a package.
  *
  * Returns true both when picking up or dropping off a package, false otherwise.
  *
  * @return bool true or false depending on whether or not drone is currently delivering a package.
  */
  bool CurrentlyDelivering() { return currentlyDelivering; }

  /**
  * @brief Adds another package for the drone to deliver.
  *
  * @param[in] package Package - package for this drone to deliver
  *
  * @return Nothing.
  */
  void AddDelivery(Package* package);

  /**
  * @brief Returns total number of targets this drone currently has on its stack.
  *
  * @return int number of targets this drone is currently scheduled to travel to (including
  *  the current target being traveled to)
  */
  int NumberOfStackedTargets() { targetStack_.size(); }

  /**
   * @brief Handles movement, collision, package pickup, dropoff, etc. of actual live-simulation.
   *
   * @param[in] dt float - timestep from previous frame
   *
   * @return Nothing.
   */
  void Update(float dt);

  /**
  * @brief Returns the current route the drone is traveling along.
  *
  * @return Vector array of strings denoting the drone's current route.
  */
  std::vector<std::string>& GetCurrentRoute() const { return *currentRouteNamesPtr; }

  /**
  * @brief Returns the remaining battery life of the drone in seconds.
  *
  * @return float containing the remaining battery life of the drone.
  */
  float GetRemainingBattery() const { return remainingBatteryCapacity_; }

  /**
  * @brief Returns the current speed of the drone in m/s.
  *
  * @return float containing the speed at which the drone is currently traveling.
  */
  float GetCurrentSpeed() const { return currentSpeed_; }

  /**
  * @brief Returns the max carrying capacity of the drone in kg.
  *
  * @return float containing the max carrying capacity of drone.
  */
  float GetMaxCapacity() const { return weightCapacity_; }

  /**
  * @brief Returns the current remaining carrying capacity of the drone in kg.
  *
  * @return float containing the remaining carrying capacity of drone.
  */
  float GetRemainingCapacity() const { return remainingWeightCapacity_; }

  /**
  * @brief Returns the base acceleration of the drone in m/s^2.
  *
  * @return float containing the base acceleration of drone.
  */
  float GetBaseAcceleration() const { return baseAcceleration_; }

  /**
  * @brief Returns the weight of the drone in kg.
  *
  * @return float containing the weight/mass of drone.
  */
  float GetWeight() const { return weight_; }

  /**
  * @brief Returns the maximum possible speed of the drone in m/s.
  *
  * @return float containing the maximum possible speed of drone.
  */
  float GetMaxSpeed() const { return maxSpeed_; }

  /**
  * @brief Returns the maximum battery capacity of the drone in seconds.
  *
  * @return float containing the maximum battery capacity of drone.
  */
  float GetMaxBattery() const { return batteryCapacity_; }

  /**
  * @brief Returns the physics model the drone is simulating with.
  *
  * @return string containing the physics model of drone (velocity, acceleration or force).
  */
  std::string GetPhysicsModel() const { return physicsModel_; }

  /**
   * @brief This function notifies the observer.
   *
   * @param[in] const picojson::value event - the event notification
   *
   * @return Nothing.
   */
  void Notifies(const picojson::value &event);

  /**
   * @brief Returns the drones vector of entity observers.
   *
   * @return The address of the drones vector of observers.
   */
  std::vector<entity_project::EntityObserver*>* GetObservers() { return &observers_; }

  /**
   * @brief This function set the status of the notification.
   *
   * @param[in] takes in an int of number
   *
   * @return Nothing.
   */
  picojson::value GetStatus(int num);

  /**
   * @brief Sets the drone's (scheduling) score.
   *
   * @param[in] score float - the drone's new score
   *
   * @return Nothing.
   */
  void SetScore(float score) { score_ = score; }

  /**
   * @brief Gets the drone's current (scheduling) score.
   *
   * @return The drone's current (scheduling) score.
   */
  float GetScore() { return score_; }

  /**
   * @brief Estiamtes the total distance the drone will travel to pick up a package and 
   *  deliver it to its destination by using the euclidean distance between items on the stack.
   *
   * @param[in] package Package - package we wish to estimate delivery distance of.
   *
   * @return float total estimated distance the drone will travel to pick up and deliver 
   *  this package (while taking into account all deliveries along the way).
   */
  float PackageDeliveryEuclideanDistance(Package* package);

  /**
   * @brief Computes the total distance the drone will travel to pick up a package and 
   *  deliver it to its destination, considering all other deliveries it must make along the way.
   *
   * @param[in] package Package - package we wish to know delivery distance of.
   *
   * @return float total distance making up the route the drone will travel to pick up and deliver 
   *  this package (while taking into account all deliveries along the way)
   */
  float PackageDeliveryRouteDistance(Package* package);

 private:
  /**
  * @brief Moves the drone in specified direction over given timestep.
  *
  * Called in MoveAlongRoute(), moves the drone in the given direciton by using Euler integration
  *  along with the drone's speed and depending on the drone's physics model.
  *
  * @param[in] direction float* - array of x, y and z directions
  * @param[in] dt float - timestep from previous frame
  *
  * @return Nothing.
  */
  void Move(std::vector<float> direction, float dt);

  /**
  * @brief Moves the drone along a given route over given timestep
  *
  * Called in Update(), this function itself calls Move() after calculating the direction between
  *  the drone and the next point in the route. This next point is kept track of externally by
  *  the private member variable currRoutePointi.
  *
  * @param[in] route vector<SimpleUMNRoutePoint> - vector array of points along the route
  * @param[in] dt float - timestep from previous frame
  *
  * @return Nothing.
  */
  void MoveAlongRoute(std::vector<SimpleUMNRoutePoint> route, float dt);

  /**
  * @brief Helper function to look at the target on the front of the stack and begin
  *  and generate a route to begin traveling to it.
  *
  * @return Nothing.
  */
  void SetCurrentTarget();

  /**
  * @brief Removes package from this drone's scheduled pacakges and carried packages vectors.
  *
  * @param[in] package Package object - package to be removed
  *
  * @return Nothing.
  */
  void RemovePackage(entity_project::Package* package);

  /**
   * @brief This function notifies the observers if the drone is moving or idle.
   *
   * @param[in] bool moving variable for setting the movement status of the drone.
   *
   * @return Nothing.
   */
  void SetMoving(bool moving);
  /**
   * @brief Determines where in the stack to insert a package and its destination.
   *  Does so by finding the most efficient place it fits in given the drone's weight
   *  capacity.
   *
   * @param[in] package Package - the package that we sould like to add (which contains 
   *  the destination it wishes to travel to)
   *
   * @return A pair of indices. The first corresponds to where the package should be inserted,
   *  and the second to the destination..
   */
  std::pair<int, int> GetInsertIndices(Package* package);

  std::vector<entity_project::EntityObserver*> observers_;  //!< Vector of drone's observers.
  float currentSpeed_ = 0;  //!< Drone's current speed of travel.
  float height_ = 0;  //!< The height off the ground at which the drone travels.
  float packageHeight_ = 0;  //!< The height below the drone the package should hang.
  std::string model_;  //!< Drone's unique model signifier.
  std::string physicsModel_;  //!< Drone's physics model (velocity, acceleration or force).
  float weight_;  //!< Weight of the drone (kg).
  float weightCapacity_;  //!< Max weight the drone can carry (kg).
  float remainingWeightCapacity_;  //!< Actual current remaining weight capacity of the drone (kg).
  float batteryCapacity_;  //!< Max capapcity of the drone's battery (s).
  float remainingBatteryCapacity_;  //!< Remaining capapcity of the drone's battery (s).
  float maxSpeed_;  //!< Max speed the drone can reach (m/s).
  float baseAcceleration_;  //!< Drone's acceleration (m/s^2).
  bool currentlyDelivering = false;  //!< Tracks whether drone is currently delivering a package.
  /**
   * @brief "stack" of targets for drone to travel to. Drone pops targets to travel to them. This
   *  allows for dynamic routing where targets can be appended at runtime.
   */
  std::deque<entity_project::Entity*> targetStack_;
  std::vector<SimpleUMNRoutePoint> currentRoute_;  //!< The current route the drone is following.
  int currentRouteIndex_ = 0;  //!< Index of current point being traveled to in current route.
  entity_project::Entity* currentTarget_;  //!< The current target entity the drone is headed to.
  std::vector<float> currentTargetPos_;  //!< The current target position drone is headed to.
  /**
   * @brief List of all the packages the drone is currently carrying.
   */
  std::vector<entity_project::Package*> carriedPackages_;
  /**
   * @brief List of all the packages the drone is either currently carrying or scheduled to pickup.
   */
  std::vector<entity_project::Package*> scheduledPackages_;
  std::vector<std::string> currentRouteNames;  //!< The vector of route positions in string from.
  std::vector<std::string>* currentRouteNamesPtr;  //!< Ptr to the vector of route names.
  float score_;  //!< Score rating that determines drone's viability for a delivery.
};

}  // namespace csci3081

#endif  // DRONE_H_
