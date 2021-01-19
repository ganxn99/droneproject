#include "src/id_manager.h"

namespace csci3081 {

// intitialize uniqueId to 0.
int IdManager::uniqueId = 0;

int IdManager::GetUniqueId() {
  return ++uniqueId;
}

}  // namespace csci3081
