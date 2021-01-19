#include "gtest/gtest.h"
#include "EntityProject/ANVIL/drone_delivery_system.h"
#include "src/drone.h"
#include "src/customer.h"
#include "src/package.h"
#include <EntityProject/project_settings.h>
#include <algorithm>

namespace csci3081 {

using entity_project::DroneDeliverySystem;
using entity_project::Entity;
using csci3081::Drone;
using csci3081::Customer;
using csci3081::Package;

class SchedulerTests : public ::testing::Test {
 protected:
  virtual void SetUp() {
    system = GetDroneDeliverySystem("default");
    graph = parser.CreateGraph("data/umn.osm", "data/umn-height.csv");
    system->SetGraph(graph);
    const char* droneJsonText =
    "{\"direction\":[1,0,0],\"duration\":2,\"mesh\":\"models\\/s9drone.glb\","
    "\"name\":\"drone\",\"position\":[498.29199999999997,253.88300000000001,"
    "-228.62299999999999],\"radius\":1,\"rotation\":[0,0,0,0],\"scale\":"
    "[0.10000000000000001,0.10000000000000001,0.10000000000000001],"
    "\"speed\":30,\"start\":2,\"type\":\"drone\"}";
    const char* farawayDroneJsonText =
    "{\"direction\":[1,0,0],\"duration\":2,\"mesh\":\"models\\/s9drone.glb\","
    "\"name\":\"drone\",\"position\":[100000,253.88300000000001,"
    "-228.62299999999999],\"radius\":1,\"rotation\":[0,0,0,0],\"scale\":"
    "[0.10000000000000001,0.10000000000000001,0.10000000000000001],"
    "\"speed\":30,\"start\":2,\"type\":\"drone\"}";
    const char* customerJsonText =
    "{\"direction\":[1,0,0],\"mesh\":\"models\\/RobotExpressive.glb\","
    "\"name\":\"customer\",\"position\":[-951.412,254.665,298.271],"
    "\"radius\":1,\"rotation\":[0,0,0,0],\"scale\":"
    "[0.25,0.25,0.25],\"type\":\"customer\"}";
    const char* packageJsonText =
    "{\"direction\":[1,0,0],\"mesh\":\"models\\/cardboardBox.glb\","
    "\"name\":\"package\",\"position\":[-255.994,257.798,-296.85899999999998],"
    "\"radius\":1,\"rotation\":[0,0,0,0],\"scale\":"
    "[0.02,0.02,0.01],\"type\":\"package\"}";
    const char* packageNextToCustomerJsonText =
    "{\"direction\":[1,0,0],\"mesh\":\"models\\/cardboardBox.glb\","
    "\"name\":\"package\",\"position\":[-951.412,254.665,298.271],"
    "\"radius\":1,\"rotation\":[0,0,0,0],\"scale\":"
    "[0.02,0.02,0.01],\"type\":\"package\"}";
    picojson::value droneV;
    picojson::value farawayDroneV;
    picojson::value customerV;
    picojson::value packageV;
    picojson::value packageNextToCustomerV;
    picojson::parse(droneV, droneJsonText);
    picojson::parse(farawayDroneV, farawayDroneJsonText);
    picojson::parse(customerV, customerJsonText);
    picojson::parse(packageV, packageJsonText);
    picojson::parse(packageNextToCustomerV, packageNextToCustomerJsonText);
    droneJson = droneV.get<picojson::object>();
    farawayDroneJson = farawayDroneV.get<picojson::object>();
    customerJson = customerV.get<picojson::object>();
    packageJson = packageV.get<picojson::object>();
    packageNextToCustomerJson = packageNextToCustomerV.get<picojson::object>();
  }
  virtual void TearDown() {}

  DroneDeliverySystem* system;
  picojson::value::object droneJson;
  picojson::value::object farawayDroneJson;
  picojson::value::object customerJson;
  picojson::value::object packageJson;
  picojson::value::object packageNextToCustomerJson;
  const entity_project::IGraph* graph;
  entity_project::OSMGraphParser parser;
};
/*******************************************************************************
 * Test Cases
 ******************************************************************************/

// Tests if multiple packages can be scheduled to a single drone
TEST_F(SchedulerTests, MultiplePackagesOneDrone) {
  Package* package1 = system->CreateEntity(packageJson)->AsType<Package>();
  Package* package2 = system->CreateEntity(packageJson)->AsType<Package>();
  Customer* customer1 = system->CreateEntity(customerJson)->AsType<Customer>();
  Customer* customer2 = system->CreateEntity(customerJson)->AsType<Customer>();
  Drone* drone = system->CreateEntity(droneJson)->AsType<Drone>();
  system->AddEntity(package1);
  system->AddEntity(package2);
  system->AddEntity(customer1);
  system->AddEntity(customer2);
  system->AddEntity(drone);
  system->ScheduleDelivery(package1, customer1, droneJson);
  system->ScheduleDelivery(package2, customer2, droneJson);

  EXPECT_EQ(drone->NumberOfStackedTargets(), 4);
}

// Tests if multiple packages can be scheduled evenly to multiple drones
TEST_F(SchedulerTests, MultiplePackagesMultipleDrones) {
  std::vector<Package*> packages;
  for (int i = 0; i < 6; i++) {
    packages.push_back(system->CreateEntity(packageJson)->AsType<Package>());
    system->AddEntity(packages[i]);
  }
  std::vector<Customer*> customers;
  for (int i = 0; i < 6; i++) {
    customers.push_back(system->CreateEntity(customerJson)->AsType<Customer>());
    system->AddEntity(customers[i]);
  }
  std::vector<Drone*> drones;
  for (int i = 0; i < 3; i++) {
    drones.push_back(system->CreateEntity(droneJson)->AsType<Drone>());
    system->AddEntity(drones[i]);
  }

  for (int i = 0; i < 6; i++) {
    system->ScheduleDelivery(packages[i], customers[i], droneJson);
  }

  EXPECT_EQ(drones[0]->NumberOfStackedTargets(), 4);
  EXPECT_EQ(drones[1]->NumberOfStackedTargets(), 4);
  EXPECT_EQ(drones[2]->NumberOfStackedTargets(), 4);
}

// Tests if drones that are closer to the package get scheduled for delivery
TEST_F(SchedulerTests, MultiplePackagesFarawayDrone) {
  std::vector<Package*> packages;
  for (int i = 0; i < 5; i++) {
    packages.push_back(system->CreateEntity(packageJson)->AsType<Package>());
    system->AddEntity(packages[i]);
  }
  std::vector<Customer*> customers;
  for (int i = 0; i < 5; i++) {
    customers.push_back(system->CreateEntity(customerJson)->AsType<Customer>());
    system->AddEntity(customers[i]);
  }
  Drone* closeDrone = system->CreateEntity(droneJson)->AsType<Drone>();
  Drone* farDrone = system->CreateEntity(farawayDroneJson)->AsType<Drone>();

  system->AddEntity(closeDrone);
  system->AddEntity(farDrone);

  for (int i = 0; i < 5; i++) {
    system->ScheduleDelivery(packages[i], customers[i], droneJson);
  }

  EXPECT_EQ(closeDrone->NumberOfStackedTargets(), 10);
  EXPECT_EQ(farDrone->NumberOfStackedTargets(), 0);
}

// Tests if packages that are next to the customer are delivered immediately
TEST_F(SchedulerTests, PackageNextToCustomer) {
  Package* package1 = system->CreateEntity(packageNextToCustomerJson)->AsType<Package>();
  Package* package2 = system->CreateEntity(packageNextToCustomerJson)->AsType<Package>();
  Package* package3 = system->CreateEntity(packageJson)->AsType<Package>();
  Customer* customer1 = system->CreateEntity(customerJson)->AsType<Customer>();
  Customer* customer2 = system->CreateEntity(customerJson)->AsType<Customer>();
  Customer* customer3 = system->CreateEntity(customerJson)->AsType<Customer>();
  Drone* drone = system->CreateEntity(droneJson)->AsType<Drone>();
  system->AddEntity(package1);
  system->AddEntity(package2);
  system->AddEntity(package3);
  system->AddEntity(customer1);
  system->AddEntity(customer2);
  system->AddEntity(customer3);
  system->AddEntity(drone);

  EXPECT_EQ(system->GetEntities().size(), 7);

  // package next to customer should be deleted immediately
  system->ScheduleDelivery(package1, customer1, droneJson);
  system->Update(0.1);
  EXPECT_EQ(drone->NumberOfStackedTargets(), 0);
  EXPECT_EQ(system->GetEntities().size(), 6);

  // package next to customer should be deleted immediately
  system->ScheduleDelivery(package2, customer2, droneJson);
  system->Update(0.1);
  EXPECT_EQ(drone->NumberOfStackedTargets(), 0);
  EXPECT_EQ(system->GetEntities().size(), 5);

  // package faraway from customer should not be deleted
  system->ScheduleDelivery(package3, customer3, droneJson);
  system->Update(0.1);
  EXPECT_EQ(drone->NumberOfStackedTargets(), 2);
  EXPECT_EQ(system->GetEntities().size(), 5);
  bool packageStillInEntities = std::find(system->GetEntities().begin(),
                                          system->GetEntities().end(), package3)
                                          != system->GetEntities().end();
  EXPECT_EQ(packageStillInEntities, true);
}

}  // namespace csci3081
