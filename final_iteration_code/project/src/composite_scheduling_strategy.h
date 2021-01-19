#ifndef COMPOSITE_SCHEDULING_H_
#define COMPOSITE_SCHEDULING_H_

#include "src/drone_scheduling_strategy.h"
#include "src/drone.h"
#include "src/package.h"
#include "src/customer.h"
#include <vector>

namespace csci3081 {

/// @brief A composite scheduling strategy class that can create combinations of various drone
/// scheduling strategies.
class CompositeSchedulingStrategy : public DroneSchedulingStrategy {
 public:
  /**
   * @brief The destructor. Deletes all strategies this composite is composed of.
   */
  ~CompositeSchedulingStrategy();
  /**
   * @brief Adds an additional scheduling strategy to this composite.
   *
   * @param[in] schedulingStrategy DroneSchedulingStrategy - the scheduling strategy to be added
   *
   * @return Nothing.
   */
  void AddSchedulingStrategy(DroneSchedulingStrategy* schedulingStrategy) {
    strategies_.push_back(schedulingStrategy);
  }
  /**
   * @brief Scores a drone based on all of the scheduling strategies that currently
   *  make up the composite.
   *
   * @param[in] drone Drone - drone object to be scored
   * @param[in] package Package - potentail delivery package to be considered in scoring
   *
   * @return float score rating for the drone given all strategies that compose the composite.
   */
  float Score(Drone* drone, Package* package);

 private:
  /*
   * @brief Vector of strategies that make up the composite.
   */
  std::vector<DroneSchedulingStrategy*> strategies_;
};

}  // namespace csci3081

#endif  // COMPOSITE_SCHEDULING_H_
