#include "src/drone_simulation.h"
#include "src/drone.h"
#include "src/customer.h"
#include "src/package.h"

using entity_project::Entity;

namespace csci3081 {

DroneSimulation::~DroneSimulation() {
  // delete all entities on simulation exit
  for (int i = 0; i < entities_.size(); i++) {
    delete entities_[i];
  }
  entities_.clear();
}

void DroneSimulation::AddObserver(entity_project::Entity* entity,
  entity_project::EntityObserver* observer) {
  // use ObserverManager to add an observer
  observer_manager.AddObserver(entity, observer);
}


void DroneSimulation::RemoveObserver(entity_project::Entity* entity,
  entity_project::EntityObserver* observer) {
  // use ObserverManager to remove an observer
  observer_manager.RemoveObserver(entity, observer);
}

void DroneSimulation::Update(float dt) {
  // iterate through all entities to find drones and update them
  for (int i = 0; i < entities_.size(); i++) {
    Entity* entity = entities_[i];
    if (entity->AsType<Drone>()) {  // find drones
      entity->AsType<Drone>()->Update(dt);  // update them
    }
  }
  // delete any packages that have been delivered
  for (int i = 0; i < entities_.size(); i++) {
    Entity* entity = entities_[i];
    if (entity->AsType<Package>()) {  // find packages
      if (entity->AsType<Package>()->BeenDelivered()) {  // if package delivered, delete it
        entities_.erase(entities_.begin() + i);
        delete entity;
        i -= 1;
      }
    }
  }
}

}  // namespace csci3081
