#include "src/observer_manager.h"

namespace csci3081 {

void ObserverManager::AddObserver(entity_project::Entity* entity,
 entity_project::EntityObserver* observer) {
  // if entity is a package, add package observer
  Package* package = entity->AsType<Package>();
  if (package) {
    observers = package->GetObservers();
    observers->push_back(observer);
  }
  // if entity is a drone, add drone observer
  Drone* drone = entity->AsType<Drone>();
  if (drone) {
    observers = drone->GetObservers();
    observers->push_back(observer);
  }
}

void ObserverManager::RemoveObserver(entity_project::Entity* entity,
 entity_project::EntityObserver* observer) {
  // if entity is a package, remove package observer
  Package* package = entity->AsType<Package>();
  if (package) {
    observers = package->GetObservers();
    for (int i = 0; i < observers->size(); i++) {
      if ((*observers)[i] == observer) {
        observers->erase(observers->begin() + i);
        break;
      }
    }
  }
  // if entity is a drone, remove drone observer
  Drone* drone = entity->AsType<Drone>();
  if (drone) {
    observers = drone->GetObservers();
    for (int i = 0; i < observers->size(); i++) {
      if ((*observers)[i] == observer) {
        observers->erase(observers->begin() + i);
        break;
      }
    }
  }
}

}  // namespace csci3081
