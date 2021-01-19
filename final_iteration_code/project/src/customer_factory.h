#ifndef CUSTOMER_FACTORY_H
#define CUSTOMER_FACTORY_H

#include <string>
#include "src/customer.h"
#include "src/json_helper.h"
#include "src/entity_factory.h"

namespace csci3081 {

/// @brief This is a concrete factory for the customer class.
class CustomerFactory: public EntityFactory {
 public:
  /**
   * @brief Attempts to create a Customer entity using the json object passed.
   *
   * @param[in] picojson object - details used to create a customer
   *
   * @return Customer entity or NULL depending on type of json passed.
   */
  entity_project::Entity* CreateEntity(const picojson::object& val);

 private:
  std::string type_ = "customer";  //!< The type of entity this factory produces.
};

}  // namespace csci3081

#endif  // CUSTOMER_FACTORY_H
