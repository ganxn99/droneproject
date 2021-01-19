#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <EntityProject/ANVIL/customer.h>
#include "src/id_manager.h"

namespace csci3081 {

/// @brief A customer entity object used as a destination for delivering packages in the simulation.
/**
 * The customer is one of the three major entity_project::Entity instances in this simulation
 *  (the other two being Drone and Package). It has a unique id, a position, size and direction as
 *  well as the ability to recieve a package.
 */
class Customer : public entity_project::Customer {
 public:
   /**
   * @brief The default constructor.
   *
   * Should not be used.
   */
  Customer() { AddType<Customer>(); }

  /**
   * @brief The destructor.
   */
  ~Customer() {}

  /**
  * @brief The parameter constructor.
  *
  * @param[in] val JSON object - contains all data necessary to initialize/render customer
  */
  explicit Customer(const picojson::object& val);

  /**
  * @brief Returns whether the customer is dynamic or static.
  *
  * @return bool true or false depending on whether the customer is dynamic or static, respectivally.
  */
  bool IsDynamic() { return isDynamic_; }

  /**
  * @brief Returns whether or not the customer has received its package.
  *
  * @return bool true or false depending on whether the customer has recieved its package.
  */
  bool RecievedPackage() { return RecievedPackage_; }

  /**
  * @brief Returns whether or not the customer has received its package.
  *
  * @return bool true or false depending on whether the customer has recieved its package.
  */
  void SetRecievedPackage(bool RecievedPackage) { RecievedPackage_ = RecievedPackage; }

 private:
  bool isDynamic_ = false;  //!< Tracks whether the customer is staic or dynamic. Static by default.
  bool RecievedPackage_ = false;  //!< Set true when the customer has been delivered its package.
};

}  // namespace csci3081

#endif  // CUSTOMER_H_
