#ifndef PACKAGE_H_
#define PACKAGE_H_

#include <EntityProject/ANVIL/package.h>
#include <EntityProject/entity_observer.h>
#include "src/customer.h"
#include "src/id_manager.h"
#include <vector>
#include <string>

namespace csci3081 {

/// @brief A package entity object that can be delivered by drones to customers in the simulation.
/**
 * The package is one of the three major entity_project::Entity instances in this simulation
 *  (the other two being Drone and Customer). It has a unique id, a position, size and direction
 *  as well as the capabilites to interact with observers.
 */
class Package : public entity_project::Package {
 public:
   /**
   * @brief The default constructor.
   *
   * Should not be used.
   */
  Package() { AddType<Package>(); }

  /**
  * @brief The parameter constructor.
  *
  * @param[in] val JSON object - contains all data necessary to initialize/render package
  */
  explicit Package(const picojson::object& val);

  /**
   * @brief The destructor.
   */
  ~Package() {}

  /**
  * @brief Returns the destination/customer of the package.
  *
  * @return Customer that the package is being delivered to.
  */
  entity_project::Customer* GetDestination() { return dest_; }

  /**
  * @brief Sets the destination/customer of the package.
  *
  * @param[in] dest Customer - customer the package is being delivered to
  *
  * @return Nothing.
  */
  void SetDestination(entity_project::Customer* dest) { dest_ = dest; }

  /**
  * @brief Sets the position of the package.
  *
  * Used primarily by drones in order to carry packages.
  *
  * @param[in] position vector<float> - x, y, z position to set package to
  *
  * @return Nothing.
  */
  void SetPosition(const std::vector<float>& position)
    { for (int i = 0; i < 3; i++) { position_[i] = position[i]; } }

  /**
  * @brief Sets the direction of the package.
  *
  * Used primarily by drones in order for packages direction to match the drone's.
  *
  * @param[in] dir vector<float> - x, y, z direction to set package to
  *
  * @return Nothing.
  */
  void SetDirection(const std::vector<float>& dir)
    { for (int i = 0; i < 3; i++) { direction_[i] = dir[i]; } }

  /**
   * @brief Sets whether or not the package is dynamic or static.
   *
   * @param[in] bool isDynamic - true=dynamic, false=static
   *
   * @return Nothing.
   */
  void SetIsDynamic(bool isDynamic) { isDynamic_ = isDynamic; }

  /**
  * @brief Returns whether the customer is dynamic or static.
  *
  * @return bool true or false depending on whether the package is dynamic or static, respectivally.
  */
  bool IsDynamic() { return isDynamic_; }

  /**
   * @brief Sets whether or not the package has been marked as delivered.
   *
   * Used primarily to delete the package from the simulation once it has been delivered.
   *
   * @param[in] bool beenDelivered - true=delivered, false=not-delivered
   *
   * @return Nothing.
   */
  void SetBeenDelivered(bool beenDelivered) {
    beenDelivered_ = beenDelivered;
    if (beenDelivered) {
      dest_->AsType<Customer>()->SetRecievedPackage(true);
    }
  }

  /**
   * @brief Returns whether or not the package has been delivered.
   *
   * @return bool whether or not the package has been delivered.
   */
  bool BeenDelivered() { return beenDelivered_; }

  /**
  * @brief Returns the weight of the package in kg.
  *
  * @return float containing the weight of the package.
  */
  float GetWeight() const { return weight_; }

  /**
   * @brief Notifies all of the package's observers.
   *
   * @param[in] const picojson::value &event The picojson event sent to observers.
   *
   * @param[in] const Entity &entity The entity who's observers are notified.
   *
   * @return Nothing.
   */
  void Notify(const picojson::value &event, const Entity &entity);

  /**
   * @brief Returns the packages vector of entity observers.
   *
   * @return The address of the packages vector of observers.
   */
  std::vector<entity_project::EntityObserver*>* GetObservers() { return &observers_; }

  /**
   * @brief Returns picojson::value message for observer notification.
   *
   * @param[in] int notification_num The number that designates the appropriate message.
   *
   * @return The picojson::value which is the notifcation message for the observers.
   */
  picojson::value GetNotification(int notification_num);

  /**
   * @brief Gets package's priority.
   *
   * @return srting containing this package's priority (e.g. "medium").
   */
  std::string GetPriority() { return priority_; }

  /**
   * @brief Gets package's minimum delivery time.
   *
   * @return float containing the minimum time this package can be delivered in.
   */
  float GetMinDeliveryTime() { return minDeliveryTime_; }

  /**
   * @brief Sets package's priority info.
   *
   * @param[in] string priority - package's priority info
   *
   * @return Nothing.
   */
  void SetPriority(std::string priority) { priority_ = priority; }

  /**
   * @brief Sets the package's minimum delivery time.
   *
   * @param[in] picojson::object deliveryDetails - package's delivery details
   *
   * @return Nothing.
   */
  void SetMinDeliveryTime(float minDeliveryTime) { minDeliveryTime_ = minDeliveryTime; }

 private:
  /**
   * @brief Used to track all observers for this package entity instance.
   */
  std::vector<entity_project::EntityObserver*> observers_;
  float weight_;  //!< Package's weight/mass (kg).
  entity_project::Customer* dest_;  //!< Package's delivery destination.
  bool isDynamic_ = false;  //!< Tracks whether the package is staic or dynamic.
  bool beenDelivered_ = false;  //!< Tracks whether the package has been delivered.
  std::string priority_ = "none";  //!< This package's priority, assigned by scheduler.
  float minDeliveryTime_ = -1;  //!< The minimum time this package should be delivered in.
};

}  // namespace csci3081

#endif  // PACKAGE_H_
