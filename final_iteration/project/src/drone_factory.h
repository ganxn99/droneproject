#ifndef DRONE_FACTORY_H
#define DRONE_FACTORY_H

#include <string>
#include "src/drone.h"
#include "src/json_helper.h"
#include "src/entity_factory.h"

namespace csci3081 {

/// @brief This is a concrete factory for the drone class.
class DroneFactory : public EntityFactory {
 public:
  /**
   * @brief Attempts to create a Drone entity using the json object passed.
   *
   * @param[in] picojson object - details used to create a customer
   *
   * @return Drone entity or NULL depending on type of json passed.
   */
  entity_project::Entity* CreateEntity(const picojson::object& val);

 private:
  std::string type_ = "drone";  //!< The type of entity this factory produces.
};

}  // namespace csci3081

#endif  // DRONE_FACTORY_H
