#ifndef OBSERVER_MANAGER_H
#define OBSERVER_MANAGER_H

#include <EntityProject/ANVIL/drone_delivery_system.h>
#include <EntityProject/entity_observer.h>
#include <EntityProject/web_scene_viewer.h>
#include <EntityProject/entity_console_logger.h>
#include <vector>
#include <string>
#include "src/package.h"
#include "src/drone.h"

namespace csci3081 {

/// @brief A manager for adding and removing observers from entities.
class ObserverManager{
 public:
 /**
  * @brief Adds a new observer to the respective entity's list of observers.
  *
  * @param[in] Entity* entity - the entity the observer is being added to
  * @param[in] EntityObserver* observer - the observer being added to the entity
  *
  * return Nothing.
  */
  void AddObserver(entity_project::Entity* entity, entity_project::EntityObserver* observer);

  /**
   * @brief Removes an observer from the respective entity's list of observers.
   *
   * @param[in] Entity* entity - the entity the observer is being removed from
   * @param[in] EntityObserver* observer - the observer being removed from the entity
   *
   * return Nothing.
   */
  void RemoveObserver(entity_project::Entity* entity, entity_project::EntityObserver* observer);

 private:
  /**
   * @brief Vector of all the observers that have been added.
   */
  std::vector<entity_project::EntityObserver*> *observers;
};

}  // namespace csci3081

#endif  // OBSERVER_MANAGER_H
