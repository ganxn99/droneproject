#include "gtest/gtest.h"
#include "EntityProject/ANVIL/drone_delivery_system.h"
#include "EntityProject/ANVIL/drone.h"
#include "EntityProject/ANVIL/customer.h"
#include "EntityProject/ANVIL/package.h"
#include <EntityProject/project_settings.h>

namespace csci3081 {

using entity_project::DroneDeliverySystem;
using entity_project::Entity;
using entity_project::Drone;
using entity_project::Customer;
using entity_project::Package;

class FactoryTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    system = GetDroneDeliverySystem("default");
  }
  virtual void TearDown() {}

  DroneDeliverySystem* system;
};
/*******************************************************************************
 * Test Cases
 ******************************************************************************/

TEST_F(FactoryTest, DroneCreated) {
  picojson::object obj;
  obj["type"] = picojson::value("drone");
  Entity* entity = system->CreateEntity(obj);
  ASSERT_NE(entity, nullptr);
  Drone* drone = entity->AsType<Drone>();
  ASSERT_NE(drone, nullptr);
  ASSERT_EQ(system->GetEntities().size(), 0);
  system->AddEntity(drone);
  ASSERT_EQ(system->GetEntities().size(), 1);
  ASSERT_EQ(picojson::value(system->GetEntities()[0]->GetDetails()).serialize(),
            picojson::value(obj).serialize());
}

TEST_F(FactoryTest, CustomerCreated) {
  picojson::object obj;
  obj["type"] = picojson::value("customer");
  Entity* entity = system->CreateEntity(obj);
  ASSERT_NE(entity, nullptr);
  Customer* customer = entity->AsType<Customer>();
  ASSERT_NE(customer, nullptr);
  ASSERT_EQ(system->GetEntities().size(), 0);
  system->AddEntity(customer);
  ASSERT_EQ(system->GetEntities().size(), 1);
  ASSERT_EQ(picojson::value(system->GetEntities()[0]->GetDetails()).serialize(),
            picojson::value(obj).serialize());
}

TEST_F(FactoryTest, PackageCreated) {
  picojson::object obj;
  obj["type"] = picojson::value("package");
  Entity* entity = system->CreateEntity(obj);
  ASSERT_NE(entity, nullptr);
  Package* package = entity->AsType<Package>();
  ASSERT_NE(package, nullptr);
  ASSERT_EQ(system->GetEntities().size(), 0);
  system->AddEntity(package);
  ASSERT_EQ(system->GetEntities().size(), 1);
  ASSERT_EQ(picojson::value(system->GetEntities()[0]->GetDetails()).serialize(),
            picojson::value(obj).serialize());
}

TEST_F(FactoryTest, DroneCustomerPackageSimultaneuosCreation) {
  picojson::object obj1;
  obj1["type"] = picojson::value("drone");
  Entity* entity1 = system->CreateEntity(obj1);
  picojson::object obj2;
  obj2["type"] = picojson::value("customer");
  Entity* entity2 = system->CreateEntity(obj2);
  picojson::object obj3;
  obj3["type"] = picojson::value("package");
  Entity* entity3 = system->CreateEntity(obj3);
  ASSERT_NE(entity1, nullptr);
  ASSERT_NE(entity2, nullptr);
  ASSERT_NE(entity3, nullptr);
  Drone* drone = entity1->AsType<Drone>();
  Customer* customer = entity2->AsType<Customer>();
  Package* package = entity3->AsType<Package>();
  ASSERT_NE(drone, nullptr);
  ASSERT_NE(customer, nullptr);
  ASSERT_NE(package, nullptr);
  ASSERT_EQ(system->GetEntities().size(), 0);
  system->AddEntity(drone);
  system->AddEntity(customer);
  system->AddEntity(package);
  ASSERT_EQ(system->GetEntities().size(), 3);
  ASSERT_EQ(picojson::value(system->GetEntities()[0]->GetDetails()).serialize(),
            picojson::value(obj1).serialize());
  ASSERT_EQ(picojson::value(system->GetEntities()[1]->GetDetails()).serialize(),
            picojson::value(obj2).serialize());
  ASSERT_EQ(picojson::value(system->GetEntities()[2]->GetDetails()).serialize(),
            picojson::value(obj3).serialize());
}


}  // namespace csci3081
