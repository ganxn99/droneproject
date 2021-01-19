#include "src/package.h"
#include "src/json_helper.h"

namespace csci3081 {

Package::Package(const picojson::object& val) {
  // type
  AddType<Package>();
  // static
  isDynamic_ = false;
  // id
  id_ = IdManager::GetUniqueId();
  // name
  if (JsonHelper::ContainsKey(val, "name")) {
    name_ = JsonHelper::GetString(val, "name");
  } else {
    name_ = "package";
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
  // weight
  if (JsonHelper::ContainsKey(val, "weight")) {
    weight_ = JsonHelper::GetDouble(val, "weight");
  } else {
    weight_ = 5;
  }
  // details
  details_ = val;
}

void Package::Notify(const picojson::value &event, const Entity &entity) {
    for (int i = 0; i < observers_.size(); i++) {
      observers_[i]->OnEvent(event, entity);
    }
}

picojson::value Package::GetNotification(int notification_num) {
    if (notification_num == 0) {
        const char* notificationText ="{\"type\":\"notify\", \"value\": \"scheduled\"}";
        picojson::value notification;
        picojson::parse(notification, notificationText);
        return notification;
    } else if (notification_num == 1) {
        const char* notificationText ="{\"type\":\"notify\", \"value\": \"en route\"}";
        picojson::value notification;
        picojson::parse(notification, notificationText);
        return notification;
    } else if (notification_num == 2) {
        const char* notificationText ="{\"type\":\"notify\", \"value\": \"delivered\"}";
        picojson::value notification;
        picojson::parse(notification, notificationText);
        return notification;
    }
}

}  // namespace csci3081
