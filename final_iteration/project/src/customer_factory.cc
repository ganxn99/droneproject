#include "src/customer_factory.h"

namespace csci3081 {

entity_project::Entity* CustomerFactory::CreateEntity(const picojson::object& val) {
  if (JsonHelper::ContainsKey(val, "type")) {
    std::string type = JsonHelper::GetString(val, "type");
    if (type == type_) {
      return new Customer(val);
    }
  }
  return NULL;
}

}  // namespace csci3081
