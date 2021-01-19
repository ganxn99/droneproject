#include "src/customer.h"
#include "src/json_helper.h"

namespace csci3081 {

Customer::Customer(const picojson::object& val) {
  // type
  AddType<Customer>();
  // static
  isDynamic_ = false;
  // id
  id_ = IdManager::GetUniqueId();
  // name
  if (JsonHelper::ContainsKey(val, "name")) {
    name_ = JsonHelper::GetString(val, "name");
  } else {
    name_ = "customer";
  }
  // position
  if (JsonHelper::ContainsKey(val, "position")) {
    const picojson::array& position = JsonHelper::GetArray(val, "position");
    for (int f = 0; f < position.size(); f++) {
      position_[f] = position[f].get<double>();
    }
  } else {
    position_[0] = 0;
    position_[1] = 0;
    position_[2] = 0;
  }
  // direction
  if (JsonHelper::ContainsKey(val, "direction")) {
    const picojson::array& direction = JsonHelper::GetArray(val, "direction");
    for (int f = 0; f < direction.size(); f++) {
      direction_[f] = direction[f].get<double>();
    }
  } else {
    direction_[0] = 1;
    direction_[1] = 0;
    direction_[2] = 0;
  }
  // radius
  if (JsonHelper::ContainsKey(val, "radius")) {
    radius_ = JsonHelper::GetDouble(val, "radius");
  } else {
    radius_ = 1;
  }
  // details
  details_ = val;
}

}  // namespace csci3081
