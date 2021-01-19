#ifndef ENTITY_FACTORY_H_
#define ENTITY_FACTORY_H_

#include <EntityProject/osm_graph_parser.h>
#include <EntityProject/graph.h>

#include <vector>
namespace csci3081 {

/// An abstract entity_project::Entity factory used as a template for Drone, Customer and
///  and Package factories.
class EntityFactory {
 public:
  /**
   * @brief Generates an entity based on the passed JSON object.
   *
   * @param[in] val JSON object, has type of "drone," "customer," or "package" - used to create
   *  entity of correct type
   *
   * @return entity_project::Entity object of type passed by val (Drone, Customer, or Package).
   */
  virtual entity_project::Entity* CreateEntity(const picojson::object& val) = 0;
};

}  // namespace csci3081

#endif  // ENTITY_FACTORY_H_
