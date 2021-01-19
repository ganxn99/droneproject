#ifndef PACKAGE_FACTORY_H
#define PACKAGE_FACTORY_H

#include <string>
#include "src/package.h"
#include "src/json_helper.h"
#include "src/entity_factory.h"

namespace csci3081 {

/// @brief This is a concrete factory for the package class.
class PackageFactory: public EntityFactory {
 public:
  /**
   * @brief Attempts to create a Package entity using the json object passed.
   *
   * @param[in] picojson object - details used to create a customer
   *
   * @return Package entity or NULL depending on type of json passed.
   */
  entity_project::Entity* CreateEntity(const picojson::object& val);

 private:
  std::string type_ = "package";  //!< The type of entity this factory produces.
};

}  // namespace csci3081

#endif  // PACKAGE_FACTORY_H
