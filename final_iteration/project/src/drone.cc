#include "src/drone.h"
#include "src/json_helper.h"
#include "src/csv_reader.h"
#include "src/vector3.h"
#include "src/scheduler.h"
#include <cmath>
#include <unistd.h>

namespace csci3081 {

Drone::Drone(const picojson::object& val) {
  // type
  AddType<Drone>();
  // id
  id_ = IdManager::GetUniqueId();
  // name
  if (JsonHelper::ContainsKey(val, "name")) {
    name_ = JsonHelper::GetString(val, "name");
  } else {
    name_ = "drone";
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
  position_[1] += height_;  // add height to y value (height defaults to 0)
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
  // physics model
  if (JsonHelper::ContainsKey(val, "physics-model")) {
    physicsModel_ = JsonHelper::GetString(val, "physics-model");
  } else {
    physicsModel_ = "no physics model";
  }
  // model
  if (JsonHelper::ContainsKey(val, "model")) {
    model_ = JsonHelper::GetString(val, "model");
  } else {
    model_ = "no model";
  }
  // if a valid model, get related information
  CSVReader csvReader;
  if (csvReader.ContainsModel(model_)) {
    // weight
    weight_ = csvReader.ReadWeight(model_);
    // max speed
    maxSpeed_ = csvReader.ReadMaxSpeed(model_);
    // acceleration
    baseAcceleration_ = csvReader.ReadAcceleration(model_);
    // weight capapcity
    weightCapacity_ = csvReader.ReadWeightCapacity(model_);
    remainingWeightCapacity_ = weightCapacity_;
    // battery capacity
    batteryCapacity_ = csvReader.ReadBatteryCapacity(model_);
    remainingBatteryCapacity_ = batteryCapacity_;
  } else {
    // defaults to "Q-36-01" model specs if no model specified
    weight_ = 50;
    maxSpeed_ = 60;
    baseAcceleration_ = 4;
    weightCapacity_ = 10;
    remainingWeightCapacity_ = weightCapacity_;
    batteryCapacity_ = 300;
    remainingBatteryCapacity_ = batteryCapacity_;
  }
  // details
  details_ = val;
}

void Drone::Move(std::vector<float> direction, float dt) {
  // create Vector3 versions of direction, position and velocity
  Vector3 dir = Vector3::FloatToVector3(direction);
  dir = dir / dir.Magnitude();  // normalize the direction
  Vector3 pos = Vector3::FloatToVector3(position_);
  Vector3 vel = dir * currentSpeed_;

  // move depending on drone's physics model
  if (physicsModel_ == "acceleration") {  // ***************ACCELERATION MODEL***************
    vel = vel + dir * baseAcceleration_ * dt;
    currentSpeed_ = vel.Magnitude();
    if (currentSpeed_ > maxSpeed_) {  // limit speed to maxSpeed
      vel = vel * maxSpeed_ / currentSpeed_;
      currentSpeed_ = maxSpeed_;
    }
    pos = pos + vel * dt;
  } else if (physicsModel_ == "force") {  // ******************FORCE MODEL*******************
    float force = weight_ * baseAcceleration_;
    float totalWeight = weight_ + (weightCapacity_ - remainingWeightCapacity_);
    float acceleration_ = force / totalWeight;
    vel = vel + dir * acceleration_ * dt;
    currentSpeed_ = vel.Magnitude();
    if (currentSpeed_ > maxSpeed_) {  // limit speed to maxSpeed
      vel = vel * maxSpeed_ / currentSpeed_;
      currentSpeed_ = maxSpeed_;
    }
    pos = pos + vel * dt;
  } else {  // ******************************************DEFAULT VELOCITY MODEL***************
    pos = pos + dir * maxSpeed_ * dt;
  }

  // update actual local vector<float> versions of direction and position to newly calculated values
  position_[0] = pos.GetX();
  position_[1] = pos.GetY();
  position_[2] = pos.GetZ();
  for (int i = 0; i < 3; i++) {
     direction_[i] = direction[i];
  }
}

void Drone::MoveAlongRoute(std::vector<SimpleUMNRoutePoint> route, float dt) {
  // get position of next point in the route
  std::vector<float> nextRoutePoint {route[currentRouteIndex_].x,
                                     route[currentRouteIndex_].y,
                                     route[currentRouteIndex_].z};
  // height should be maintained until final route point where package needs to be picked up
  if (currentRouteIndex_ != route.size() - 1) {
    nextRoutePoint[1] = route[currentRouteIndex_].y + height_;
  }
  // get distance between current position and route position
  float distance = FloatVectorFunctions::Distance(nextRoutePoint, position_);
  // check if current position has reached the next point along the route using this distance
  if (distance <= radius_) {
    // increment to next point in the route
    currentRouteIndex_++;
    // update the next route point position based on new route point
    nextRoutePoint[0] = route[currentRouteIndex_].x;
    if (currentRouteIndex_ == route.size() - 1) {  // height maintained until final route point
      nextRoutePoint[1] = route[currentRouteIndex_].y;
    } else {
      nextRoutePoint[1] = route[currentRouteIndex_].y + height_;
    }
    nextRoutePoint[2] = route[currentRouteIndex_].z;
  }
  // determine the direction between the current position and next route point
  std::vector<float> dir {(nextRoutePoint[0] - position_[0]),
                          (nextRoutePoint[1] - position_[1]),
                          (nextRoutePoint[2] - position_[2])};
  // move in that direction
  Move(dir, dt);
}

void Drone::AddDelivery(Package* package) {
  scheduledPackages_.push_back(package);
  // get indices at which to insert package and customer respectively
  std::pair<int, int> insertIndices = GetInsertIndices(package);
  // insert the package
  targetStack_.emplace(targetStack_.begin()+insertIndices.first, package);
  // insert the customer
  targetStack_.emplace(targetStack_.begin()+insertIndices.second, package->GetDestination());
  // if the package is now on the top, set it as a current target
  if (insertIndices.first == 0)
    SetCurrentTarget();
}

void Drone::SetCurrentTarget() {
  if (!targetStack_.empty()) {
    // set the current target to the front of the stack
    currentTarget_ = targetStack_.front();
    // generate shortest route from current position to target
    std::pair<std::vector<SimpleUMNRoutePoint>, std::vector<std::string>> route =
      RouteManager::Instance()->GenerateRouteAStar(this, currentTarget_);
    currentRoute_ = route.first;
    currentRouteNames = route.second;
    currentRouteNamesPtr = &currentRouteNames;
    // reset route index and determine target position
    currentTargetPos_ = currentTarget_->GetPosition();
    currentRouteIndex_ = 0;
    // if not currently delivering, we are now
    if (!currentlyDelivering) {
      currentlyDelivering = true;
      SetMoving(true);
    }
  } else {
    // nothing remaining on the stack so go idle and notify observer
    currentlyDelivering = false;
    SetMoving(false);
  }
}

void Drone::RemovePackage(entity_project::Package* package) {
  // remove package from scheduled packages
  for (int i = 0; i < scheduledPackages_.size(); i++) {
    if (scheduledPackages_[i] == package) {
      scheduledPackages_.erase(scheduledPackages_.begin() + i);
      break;
    }
  }
  // remove package from carried packages
  for (int i = 0; i < carriedPackages_.size(); i++) {
    if (carriedPackages_[i] == package) {
      carriedPackages_.erase(carriedPackages_.begin() + i);
      break;
    }
  }
}

void Drone::Update(float dt) {
  // if making a delivery, keep moving, drain battery and deliver the package!
  if (currentlyDelivering) {
    // move along the current route
    MoveAlongRoute(currentRoute_, dt);

    // check for proximity to current target
    float distanceToTarget = FloatVectorFunctions::Distance(position_, currentTargetPos_);
    // if close to the target, take action depending on what the target is
    if (distanceToTarget <= radius_ + currentTarget_->GetRadius()) {
      if (currentTarget_->AsType<Package>()) {
        // **********PACKAGE*************
        Package* targetPackage = currentTarget_->AsType<Package>();
        // set drone speed to 0 to pick up the package.
        currentSpeed_ = 0;
        // pick up the package.
        carriedPackages_.push_back(targetPackage);
        // decrease the actual remaining weight capacity of the drone.
        remainingWeightCapacity_ -= targetPackage->GetWeight();
        // notify package observer with the "en route" notification.
        picojson::value notification;
        notification = targetPackage->GetNotification(1);
        targetPackage->Notify(notification, *targetPackage);
      } else if (currentTarget_->AsType<Customer>()) {
        // ***********CUSTOMER***********
        Customer* targetCustomer = currentTarget_->AsType<Customer>();
        // set drone speed to 0 to deliver the package.
        currentSpeed_ = 0;
        // find which package corresponds to this customer and deliver it.
        for (int i = 0; i < carriedPackages_.size(); i++) {
          Package* carriedPackage = carriedPackages_[i]->AsType<Package>();
          Customer* packageCustomer = carriedPackage->GetDestination()->AsType<Customer>();
          if (packageCustomer == targetCustomer) {
            // deliver the package and increment the drone's weight capacities
            carriedPackage->SetBeenDelivered(true);
            remainingWeightCapacity_ += carriedPackage->GetWeight();
            // notify package observer with the "delivered" notification.
            picojson::value notification;
            notification = carriedPackage->GetNotification(2);
            carriedPackage->Notify(notification, *carriedPackage);
            // remove this package from the carriedPackages and scheduledPackages vectors
            RemovePackage(carriedPackages_[i]);
            break;
          }
        }
      }
      // pop the current target off the top of the stack since we are done traveling to it,
      //  then set the current target to the next target on the top of the stack
      //  if none are remaining the drone will be set to idle mode.
      targetStack_.pop_front();
      SetCurrentTarget();
    }

    // update all carried packages' positions and directions
    for (int i = 0; i < carriedPackages_.size(); i++) {
      carriedPackages_[i]->AsType<Package>()->SetPosition(position_);
      carriedPackages_[i]->AsType<Package>()->SetDirection(direction_);
    }

    // drain the battery
    remainingBatteryCapacity_ -= dt;

    // once the battery life is gone, stop delivering, drop all packages, and reschedule them,
    if (remainingBatteryCapacity_ <= 0) {
      // stop delivering (i.e. entering idle mode)
      currentlyDelivering = false;
      SetMoving(false);

      // drop and reschedule all the drone's currently scheduled packages
      Scheduler scheduler;
      for (int i = 0; i < scheduledPackages_.size(); i++) {
        scheduledPackages_[i]->AsType<Package>()->SetIsDynamic(false);
        scheduler.RescheduleDelivery(scheduledPackages_[i]);
      }
      scheduledPackages_.clear();
      carriedPackages_.clear();

      // reset the drone's weight capacity
      remainingWeightCapacity_ = weightCapacity_;
    }
  }
}

void Drone::SetMoving(bool moving) {
    if (moving) {
        // notify drone observer that the drone is moving
        picojson::value noti = this->GetStatus(0);
        this->Notifies(noti);
    } else {
        // notify drone observer that the drone is now in idle mode
        picojson::value noti = this->GetStatus(1);
        this->Notifies(noti);
    }
}

void Drone::Notifies(const picojson::value &event) {
    for (int i = 0; i < observers_.size(); i++) {
      observers_[i]->OnEvent(event, *this);
    }
}

picojson::value Drone::GetStatus(int num) {
  picojson::object notification;
  if (num == 0) {  // moving notification
    // convert current route to vector of vectors
    std::vector<std::vector<float>> routeVector;
    std::vector<float> routePoint(3);
    for (int i = 0; i < currentRoute_.size(); i++) {
      routePoint[0] = currentRoute_[i].x;
      routePoint[1] = currentRoute_[i].y;
      routePoint[2] = currentRoute_[i].z;
      routeVector.push_back(routePoint);
    }
    // create picojson notification
    notification["type"] = picojson::value("notify");
    notification["value"] = picojson::value("moving");
    notification["path"] = JsonHelper::EncodeArray(routeVector);
    return picojson::value(notification);
  } else if (num == 1) {  // idle notification
    notification["type"] = picojson::value("notify");
    notification["value"] = picojson::value("idle");
    return picojson::value(notification);
  }
}

float Drone::PackageDeliveryEuclideanDistance(Package* package) {
  // get the indices at which the package and customer should be inserted
  std::pair<int, int> insertIndices = GetInsertIndices(package);

  float totalDistance = 0;

  // insert the package and customer temporarily at their respective indices
  targetStack_.emplace(targetStack_.begin()+insertIndices.first, package);
  targetStack_.emplace(targetStack_.begin()+insertIndices.second, package->GetDestination());

  // calculate total route distance from current position to customer
  totalDistance += totalDistance += FloatVectorFunctions::Distance(position_,
    targetStack_[0]->GetPosition());
  for (int i = 1; i < targetStack_.size(); i++) {
    totalDistance += FloatVectorFunctions::Distance(targetStack_[i-1]->GetPosition(),
      targetStack_[i]->GetPosition());
    if (targetStack_[i]->AsType<Customer>() == package->GetDestination()) {
      break;
    }
  }

  // remove the package and customer from the stack
  targetStack_.erase(targetStack_.begin()+insertIndices.second);
  targetStack_.erase(targetStack_.begin()+insertIndices.first);

  // return the total distance
  return totalDistance;
}

float Drone::PackageDeliveryRouteDistance(Package* package) {
  // get the indices at which the package and customer should be inserted
  std::pair<int, int> insertIndices = GetInsertIndices(package);

  float totalDistance = 0;

  // insert the package and customer temporarily at their respective indices
  targetStack_.emplace(targetStack_.begin()+insertIndices.first, package);
  targetStack_.emplace(targetStack_.begin()+insertIndices.second, package->GetDestination());

  // calculate total route distance from current position to customer
  totalDistance += totalDistance += RouteManager::Instance()->RouteDistance(this, targetStack_[0]);
  for (int i = 1; i < targetStack_.size(); i++) {
    totalDistance += RouteManager::Instance()->RouteDistance(targetStack_[i-1], targetStack_[i]);
    if (targetStack_[i]->AsType<Customer>() == package->GetDestination()) {
      break;
    }
  }

  // remove the package and customer from the stack
  targetStack_.erase(targetStack_.begin()+insertIndices.second);
  targetStack_.erase(targetStack_.begin()+insertIndices.first);

  // return the total distance
  return totalDistance;
}

std::pair<int, int> Drone::GetInsertIndices(Package* package) {
  // initialize values to start at beginning of stack
  std::pair<int, int> insertIndices;
  insertIndices.first = 0;
  insertIndices.second = 0;
  float pkgWeightSum = weightCapacity_ - remainingWeightCapacity_;
  // iterate until indices are found
  while (true) {
    // iterate until a non-package object is found, summing up package weight along the way
    for (int i = insertIndices.first; i < targetStack_.size(); i++) {
      Package* pkg = targetStack_[i]->AsType<Package>();
      if (!pkg) {
        break;
      }
      pkgWeightSum += pkg->GetWeight();
      insertIndices.second++;
    }
    // if the summed weight of the packages allows room for the new package, iterate until the
    //  last customer in the package-customer pyramid is found
    if (pkgWeightSum + package->GetWeight() <= weightCapacity_) {
      for (int i = insertIndices.second; i < targetStack_.size(); i++) {
        Customer* cust = targetStack_[i]->AsType<Customer>();
        if (!cust) {
          break;
        }
        insertIndices.second++;
      }
      insertIndices.second++;
      return insertIndices;
    }

    // if there wasn't room for the package in this chain, then we should find the start of the
    //  next package chain and try again there
    insertIndices.first = insertIndices.second;
    for (int i = insertIndices.second; i < targetStack_.size(); i++) {
      Customer* cust = targetStack_[i]->AsType<Customer>();
      if (!cust) {
        break;
      }
      insertIndices.first++;
    }
    insertIndices.second = insertIndices.first;
    // now that we have a new chain of packages, reset the package sum weight and try again.
    pkgWeightSum = 0;
  }
}

}  // namespace csci3081
