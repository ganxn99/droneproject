#include "gtest/gtest.h"
#include "EntityProject/ANVIL/drone_delivery_system.h"
#include "src/drone.h"
#include "src/customer.h"
#include "src/package.h"
#include <EntityProject/project_settings.h>
#include <EntityProject/web_scene_viewer.h>
#include <EntityProject/entity_console_logger.h>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
namespace csci3081 {

using entity_project::DroneDeliverySystem;
using entity_project::Entity;
using entity_project::WebSceneViewer;
using entity_project::EntityConsoleLogger;
using csci3081::Drone;
using csci3081::Customer;
using csci3081::Package;

class DroneSimulationTests : public ::testing::Test {
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
    "\"name\":\"drone\",\"position\":[598.29199999999997,253.88300000000001,"
    "-228.62299999999999],\"radius\":1,\"rotation\":[0,0,0,0],\"scale\":"
    "[0.10000000000000001,0.10000000000000001,0.10000000000000001],"
    "\"speed\":30,\"start\":2,\"type\":\"drone\"}";
    const char* customerJsonText =
    "{\"direction\":[1,0,0],\"mesh\":\"models\\/RobotExpressive.glb\","
    "\"name\":\"customer\",\"position\":[-951.41200000000003,254.66499999999999,"
    "298.27100000000002],\"radius\":1,\"rotation\":[0,0,0,0],\"scale\":"
    "[0.25,0.25,0.25],\"type\":\"customer\"}";
    const char* packageJsonText =
    "{\"direction\":[1,0,0],\"mesh\":\"models\\/cardboardBox.glb\","
    "\"name\":\"package\",\"position\":[-255.994,257.798,-296.85899999999998],"
    "\"radius\":1,\"rotation\":[0,0,0,0],\"scale\":"
    "[0.02,0.02,0.01],\"type\":\"package\"}";
    picojson::value droneV;
    picojson::value farawayDroneV;
    picojson::value customerV;
    picojson::value packageV;
    picojson::parse(droneV, droneJsonText);
    picojson::parse(farawayDroneV, farawayDroneJsonText);
    picojson::parse(customerV, customerJsonText);
    picojson::parse(packageV, packageJsonText);
    droneJson = droneV.get<picojson::object>();
    farawayDroneJson = farawayDroneV.get<picojson::object>();
    customerJson = customerV.get<picojson::object>();
    packageJson = packageV.get<picojson::object>();
  }
  virtual void TearDown() {}

  DroneDeliverySystem* system;
  picojson::value::object droneJson;
  picojson::value::object farawayDroneJson;
  picojson::value::object customerJson;
  picojson::value::object packageJson;
  const entity_project::IGraph* graph;
  entity_project::OSMGraphParser parser;
};
/*******************************************************************************
 * Test Cases
 ******************************************************************************/

// create entity test
TEST_F(DroneSimulationTests, CreateEntityTests) {
  picojson::object obj;
  obj["type"] = picojson::value("entity");
  Entity* entity = system->CreateEntity(obj);

  EXPECT_EQ(entity, nullptr);

  Package* package = system->CreateEntity(packageJson)->AsType<Package>();
  Customer* customer = system->CreateEntity(customerJson)->AsType<Customer>();
  Drone* drone = system->CreateEntity(droneJson)->AsType<Drone>();

  EXPECT_EQ(picojson::value(package->GetDetails()).serialize(),
            picojson::value(packageJson).serialize());
  EXPECT_EQ(picojson::value(customer->GetDetails()).serialize(),
            picojson::value(customerJson).serialize());
  EXPECT_EQ(picojson::value(drone->GetDetails()).serialize(),
            picojson::value(droneJson).serialize());
}

// package schedule delivery test
TEST_F(DroneSimulationTests, ScheduleDeliveryTests) {
  // create some packages, customers and drones
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
  // schedule delivery for each package and customer pair
  for (int i = 0; i < 6; i++) {
    system->ScheduleDelivery(packages[i], customers[i], droneJson);
    // expect package's destination to be customer
    EXPECT_EQ(picojson::value(packages[i]->GetDestination()->GetDetails()).serialize(),
              picojson::value(customers[i]->GetDetails()).serialize());
    if (i < 3) {
      // expect the drone's current target to be the package
      EXPECT_EQ(picojson::value(drones[0]->GetCurrentTarget()->GetDetails()).serialize(),
                picojson::value(packages[i]->GetDetails()).serialize());
    }
  }

  // ensure that additional packages were added evenly to queues of drones
  EXPECT_EQ(drones[0]->NumberOfStackedTargets(), 4);
  EXPECT_EQ(drones[1]->NumberOfStackedTargets(), 4);
  EXPECT_EQ(drones[2]->NumberOfStackedTargets(), 4);
}

// package schedule delivery test
TEST_F(DroneSimulationTests, AddEntityTests) {
  Package* package = system->CreateEntity(packageJson)->AsType<Package>();
  Customer* customer = system->CreateEntity(customerJson)->AsType<Customer>();
  Drone* drone = system->CreateEntity(droneJson)->AsType<Drone>();

  EXPECT_EQ(system->GetEntities().size(), 0);

  system->AddEntity(package);

  EXPECT_EQ(system->GetEntities().size(), 1);
  EXPECT_EQ(picojson::value(system->GetEntities()[0]->GetDetails()).serialize(),
            picojson::value(package->GetDetails()).serialize());

  system->AddEntity(customer);

  EXPECT_EQ(system->GetEntities().size(), 2);
  EXPECT_EQ(picojson::value(system->GetEntities()[1]->GetDetails()).serialize(),
            picojson::value(customer->GetDetails()).serialize());

  system->AddEntity(drone);

  EXPECT_EQ(system->GetEntities().size(), 3);
  EXPECT_EQ(picojson::value(system->GetEntities()[2]->GetDetails()).serialize(),
            picojson::value(drone->GetDetails()).serialize());
}

// package add observer test
TEST_F(DroneSimulationTests, AddPackageObserver) {
    std::vector<Package*> packages;
    packages.push_back(system->CreateEntity(packageJson)->AsType<Package>());
    system->AddEntity(packages[0]);
    WebSceneViewer* web_scene_viewer_;
    EntityConsoleLogger* entity_console_logger_;
    system->AddObserver(packages[0], web_scene_viewer_);
    system->AddObserver(packages[0], entity_console_logger_);


    EXPECT_EQ((*(packages[0]->GetObservers())).size(), 2);
}



TEST_F(DroneSimulationTests, PackageConsoleObserverTest) {
    Package* package = system->CreateEntity(packageJson)->AsType<Package>();
    Customer* customer = system->CreateEntity(customerJson)->AsType<Customer>();
    Drone* drone = system->CreateEntity(droneJson)->AsType<Drone>();
    system->AddEntity(package);
    system->AddEntity(customer);
    system->AddEntity(drone);
    std::string notification1;
    std::string notification2;
    std::string notification3;
    std::string notification_string1;
    std::string notification_string2;
    std::string notification_string3;
    std::fstream file;
    file.open("cout.txt", std::ios::out);
    // Backup streambuffers of  cout
    std::streambuf* stream_buffer_cout = std::cout.rdbuf();
    std::streambuf* stream_buffer_cin = std::cin.rdbuf();
    // Get the streambuffer of the file
    std::streambuf* stream_buffer_file = file.rdbuf();
    // Redirect cout to file
    std::cout.rdbuf(stream_buffer_file);
    // // Redirect cout back to screen
    // cout.rdbuf(stream_buffer_cout);
    EntityConsoleLogger* entity_console_logger_0_ = new EntityConsoleLogger;
    system->AddObserver(package, entity_console_logger_0_);
    EXPECT_EQ((*(package->GetObservers())).size(), 1);
    system->ScheduleDelivery(package, customer, droneJson);
    notification_string1 = "Entity #2: " + std::to_string(package->GetId()) + ": " +
     picojson::value(package->GetNotification(0)).serialize();
    notification_string2 = "Entity #2: " + std::to_string(package->GetId()) + ": " +
     picojson::value(package->GetNotification(1)).serialize();
    notification_string3 = "Entity #2: " + std::to_string(package->GetId()) + ": " +
     picojson::value(package->GetNotification(2)).serialize();
    while (!customer->RecievedPackage()) {
        system->Update(0.01);
    }
    std::cout.rdbuf(stream_buffer_cout);
    file.close();
    file.open("cout.txt", std::ios::in);
    getline(file, notification1);
    getline(file, notification2);
    getline(file, notification3);
    file.close();
    EXPECT_EQ(notification1, notification_string1);
    EXPECT_EQ(notification2, notification_string2);
    EXPECT_EQ(notification3, notification_string3);
}

TEST_F(DroneSimulationTests, DroneConsoleObserverTest) {
    Package* package = system->CreateEntity(packageJson)->AsType<Package>();
    Customer* customer = system->CreateEntity(customerJson)->AsType<Customer>();
    Drone* drone = system->CreateEntity(droneJson)->AsType<Drone>();
    system->AddEntity(package);
    system->AddEntity(customer);
    system->AddEntity(drone);
    float remaining_battery;
    std::string notification1;
    std::string notification2;
    std::string notification_string1;
    std::string notification_string2;
    std::fstream file;
    file.open("cout.txt", std::ios::out);
    // Backup streambuffers of  cout
    std::streambuf* stream_buffer_cout = std::cout.rdbuf();
    std::streambuf* stream_buffer_cin = std::cin.rdbuf();
    // Get the streambuffer of the file
    std::streambuf* stream_buffer_file = file.rdbuf();
    // Redirect cout to file
    std::cout.rdbuf(stream_buffer_file);
    // // Redirect cout back to screen
    // cout.rdbuf(stream_buffer_cout);
    EntityConsoleLogger* entity_console_logger_0_ = new EntityConsoleLogger;
    system->AddObserver(drone, entity_console_logger_0_);
    system->ScheduleDelivery(package, customer, droneJson);
    notification_string1 = "Entity #2: " + std::to_string(drone->GetId()) + ": " +
     picojson::value(drone->GetStatus(0)).serialize();
    while (!customer->RecievedPackage()) {
        system->Update(0.01);
        remaining_battery = drone->GetRemainingBattery();
    }
    std::cout.rdbuf(stream_buffer_cout);
    file.close();
    file.open("cout.txt", std::ios::in);
    getline(file, notification1);
    getline(file, notification2);
    getline(file, notification2);
    file.close();
    notification_string2 = "Entity #2: " + std::to_string(drone->GetId()) + ": " +
     picojson::value(drone->GetStatus(1)).serialize();
    EXPECT_EQ(notification1, notification_string1);
    // EXPECT_EQ(notification2, notification_string2);
}


}  // namespace csci3081
