#ifndef ID_MANAGER_H_
#define ID_MANAGER_H_

namespace csci3081 {

/// @brief An object used to generate unique identification numbers.
/**
 * This class is utilized by the Drone, Customer and Package objects in order to
 *  safely generate unique ids for each object so that they can be displayed in the simulation.
 *  This is accomplished using a static member variable.
 */
class IdManager {
 public:
  /**
   * @ brief Returns a unique id number.
   *
   * return int unique id.
   */
  static int GetUniqueId();
 private:
  static int uniqueId;  //!< Statically incremented to generate unique ids.
};

}  // namespace csci3081

#endif  // ID_MANAGER_H_
