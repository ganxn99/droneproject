#include "src/composite_scheduling_strategy.h"

namespace csci3081 {

CompositeSchedulingStrategy::~CompositeSchedulingStrategy() {
  for (int i = 0; i < strategies_.size(); i++) {
    delete strategies_[i];
  }
}

float CompositeSchedulingStrategy::Score(Drone* drone, Package* package) {
  float score = 0;
  for (int i = 0; i < strategies_.size(); i++) {
    score += strategies_[i]->Score(drone, package);
  }
  return score;
}

}  // namespace csci3081
