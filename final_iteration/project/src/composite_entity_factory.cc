#include "src/composite_entity_factory.h"

namespace csci3081 {

CompositeEntityFactory::CompositeEntityFactory() {
  AddFactory(new DroneFactory());
  AddFactory(new PackageFactory());
  AddFactory(new CustomerFactory());
}

CompositeEntityFactory::~CompositeEntityFactory() {
  for (int i = 0; i < factories_.size(); i++) {
    delete factories_[i];
  }
}

entity_project::Entity* CompositeEntityFactory::CreateEntity(const picojson::object& val) {
  for (int i = 0; i < factories_.size(); i++) {
    entity_project::Entity* entity = factories_[i]->CreateEntity(val);
    if (entity) {
      return entity;
    }
  }
  return NULL;
}

}  // namespace csci3081
