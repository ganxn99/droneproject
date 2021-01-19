#include "src/drone_pool.h"

namespace csci3081 {

void DronePool::ScoreAllDrones(Package* package) {
  for (int i = 0; i < drones.size(); i++) {
    drones[i]->SetScore(schedulingStrategy_->Score(drones[i], package));
  }
}

}  // namespace csci3081
