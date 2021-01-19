#ifndef COMPOSITE_ENTITY_FACTORY_H
#define COMPOSITE_ENTITY_FACTORY_H

#include <string>
#include <vector>
#include "src/drone.h"
#include "src/package.h"
#include "src/customer.h"
#include "src/json_helper.h"
#include "src/entity_factory.h"
#include "src/drone_factory.h"
#include "src/package_factory.h"
#include "src/customer_factory.h"

namespace csci3081 {

/// @brief A composite entity factory class that can hold combinations of various entity
/// factories.

class CompositeEntityFactory : public EntityFactory {
 public:
  /**
   * @brief Default constructor. Creates a composite of all current concrete entity
   *  factories.
   */
  CompositeEntityFactory();
  /**
   * @brief The destructor. Deletes all factories this composite is made up of.
   */
  ~CompositeEntityFactory();
  /**
   * @brief Creates an entity specified by the picojson object passed by iterating through all
   *  factories that currently make up the composite.
   *
   * @param[in] val picojson object - details of the entity to be created
   *
   * return Entity that is of the type specified in the picojson object.
   */
  entity_project::Entity* CreateEntity(const picojson::object& val);
  /**
   * @brief Adds an additional entity factory to this composite.
   *
   * @param[in] factory EntityFactory - the entity factory to be added to the composite
   *
   * @return Nothing.
   */
  void AddFactory(EntityFactory* factory) { factories_.push_back(factory); }

 private:
  std::vector<EntityFactory*> factories_;  //!< Vector of factories that make up the composite.
};

}  // namespace csci3081

#endif  // COMPOSITE_ENTITY_FACTORY_H
