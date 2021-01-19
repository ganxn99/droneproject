#ifndef DRONE_SIMULATION_H_
#define DRONE_SIMULATION_H_

#include <EntityProject/ANVIL/drone_delivery_system.h>
#include <EntityProject/osm_graph_parser.h>
#include <EntityProject/graph.h>
#include <vector>
#include <string>
#include "src/json_helper.h"
#include "src/entity_factory.h"
#include "src/scheduler.h"
#include "src/observer_manager.h"
#include <EntityProject/entity_console_logger.h>
#include <cmath>
#include <unordered_map>
#include <map>
#include "src/route_manager.h"
#include <unistd.h>
#include "src/composite_entity_factory.h"

namespace csci3081 {

/// @brief Facade for DroneDeliverySystem interface. Controls all basic aspects of drone simulation.
/**
 * Manages drone simulation setup through instantion and control over all entities and observers,
 *  as well the actual simulation itself through scheduling of deliveries and runtime updating of
 *  all entities.
 */
class DroneSimulation : public entity_project::DroneDeliverySystem {
 public:
  /**
   * @brief The destructor. Deletes all entities in entities_ vector.
   */
  ~DroneSimulation();

  /**
   * @brief Returns the name of the team that developed this implementation.
   *
   * @return string containing name of team that developed this implementation.
   */
  inline const std::string& GetTeamName() const { return teamName_; }

  /**
   * @brief Generation of an entity with type defined by the JSON object passed.
   *
   * Generation is managed by a composite entity factory.
   *
   * @param[in] val JSON object, ideally has type of "drone," "customer," or "package"
   *
   * @return entity_project::Entity object of type passed by val (Drone, Customer, or Package).
   */
  entity_project::Entity* CreateEntity(const picojson::object& val) {
    return composite_factory.CreateEntity(val); }

  /**
   * @brief Adds an entity to the simulation.
   *
   * @param[in] entity entity_project::Entity object
   *
   * @return Nothing.
   */
  inline void AddEntity(entity_project::Entity* entity) {
     entities_.push_back(entity); }

  /**
   * @brief Sets all the nodes of the graph.
   *
   * @return Nothing.
   */
  void SetGraph(const entity_project::IGraph* graph) {
    RouteManager::Instance()->SetNodes(graph->GetNodes()); }

  /**
   * @brief Schedules delivery of a package to a specified customer, given specified details.
   *
   * This method uses DroneSimulation's inherent Scheduler object to schedule the delivery.\n
   * - If the package is not in the simulation the delivery will not be scheduled.\n
   * - If there are drones in the simulation, choose the best drone to deliver the package:\n
   * [#1] - if there are avaible drones, choose the drone that is closest to the package.\n
   * [#2] - if there are no available drones, choose an occupied drone that has enough
   *  remaining weight capcity and is ideally close to the package.\n
   * [#3] - if there are no available or occupied drones, add the package to bottom of the stack
   *  of the drone who will be the closest.
   *
   * @param[in] package Package - package scheduled to be delivered
   * @param[in] dest Customer - customer scheduled to recieve delivery
   * @param[in] details JSON object - details of the delivery
   *
   * @return Nothing.
   */
  inline void ScheduleDelivery(entity_project::Package* package,
    entity_project::Customer* dest, const picojson::object& details)
      { scheduler_.ScheduleDelivery(package, dest, details, &entities_); }

  /**
   * @brief Adds an observer to specified entity.
   *
   * @param[in] entity entity_project::Entity object
   * @param[in] observer entity_project::EntityObserver object
   *
   * @return Nothing.
   */
  void AddObserver(entity_project::Entity* entity, entity_project::EntityObserver* observer);

  /**
   * @brief Removes specified observer from specified entity.
   *
   * @param[in] entity entity_project::Entity object
   * @param[in] observer entity_project::EntityObserver object
   *
   * @return Nothing.
   */
  void RemoveObserver(entity_project::Entity* entity, entity_project::EntityObserver* observer);

  /**
   * @brief Returns vector array of all currently simulated entities.
   *
   * @return std::vector of all entity_project::Entity objects currently added to the simulation.
   */
  inline const std::vector<entity_project::Entity*>& GetEntities() const { return entities_; }

 /**
  * @brief Updates all updateable entities (namely Drone) to most recent timestep.
  *
  * This method is used for simulation purposes. It updates all drones based on the time that has
  *  passed between the current and previous frame. This method also calls for the removal of
  *  delivered packages from the simulation.
  *
  * @param[in] dt float - timestep to simulate over for current frame, dependent on framerate of
  *  simulation
  *
  * @return Nothing.
  */
  void Update(float dt);

 private:
  /**
   * @brief Name of team that developed this implementation.
   */
  std::string teamName_ = "The Most";
  /**
   * @brief Vector array of all entity_project::Entity objects currently instantiated in the
   *  simulation.
   */
  std::vector<entity_project::Entity*> entities_;
  Scheduler scheduler_;  //!< Handles actual scheduling of deliveries.
  ObserverManager observer_manager;  //!< Handles all drone and pacckage observer functionalities.
  /**
   * @brief Composite factory capable of building drones, packages, and customers.
   */
  CompositeEntityFactory composite_factory;
};

}  // namespace csci3081


#endif  // DRONE_SIMULATION_H_
