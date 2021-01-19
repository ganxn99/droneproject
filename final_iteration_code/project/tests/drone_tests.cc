#include "gtest/gtest.h"
#include "EntityProject/ANVIL/drone_delivery_system.h"
#include "src/drone.h"
#include "src/customer.h"
#include "src/package.h"
#include <EntityProject/project_settings.h>
#include <cmath>

namespace csci3081 {

using entity_project::DroneDeliverySystem;
using entity_project::Entity;
using csci3081::Drone;
using csci3081::Customer;
using csci3081::Package;

class DroneTests : public ::testing::Test {
 protected:
  virtual void SetUp() {
    system = GetDroneDeliverySystem("default");
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
};
/*******************************************************************************
 * Test Cases
 ******************************************************************************/

// default constructor test
TEST_F(DroneTests, DefaultConstructorTest) {
  Drone* defaultDrone = new Drone();

  EXPECT_NE(defaultDrone->AsType<Drone>(), nullptr);

  delete defaultDrone;
}

// parameter constructor test
TEST_F(DroneTests, ParameterConstructorTest) {
  Drone* parameterDrone = new Drone(droneJson);

  EXPECT_EQ(picojson::value(parameterDrone->GetDetails()).serialize(),
            picojson::value(droneJson).serialize());

  delete parameterDrone;
}

// add delivery test SOMETHING WRONG WITH THIS TEST, SEG FAULT
TEST_F(DroneTests, AddDeliveryTest) {
  Drone* drone = new Drone(droneJson);
  Package* package1 = new Package(packageJson);
  Package* package2 = new Package(packageJson);
  Customer* customer1 = new Customer(customerJson);
  Customer* customer2 = new Customer(customerJson);
  package1->SetDestination(customer1);
  package2->SetDestination(customer2);

  EXPECT_EQ(drone->CurrentlyDelivering(), false);
  EXPECT_EQ(drone->NumberOfStackedTargets(), 0);

  drone->AddDelivery(package1);

  EXPECT_EQ(drone->CurrentlyDelivering(), true);
  EXPECT_EQ(drone->NumberOfStackedTargets(), 2);

  drone->AddDelivery(package2);

  EXPECT_EQ(drone->CurrentlyDelivering(), true);
  EXPECT_EQ(drone->NumberOfStackedTargets(), 4);

  delete drone;
  delete package1;
  delete package2;
  delete customer1;
  delete customer2;
}


TEST_F(DroneTests, ModelTest) {
  // initialize a drone for each model
  picojson::object model1;
  model1["model"] = picojson::value("Q-36-01");
  picojson::object model2;
  model2["model"] = picojson::value("Q-36-02");
  picojson::object model3;
  model3["model"] = picojson::value("Q-36-03");
  picojson::object model4;
  model4["model"] = picojson::value("Q-36-04");
  picojson::object model5;
  model5["model"] = picojson::value("Q-36-05");
  Drone *Drone1 = new Drone(model1);
  Drone *Drone2 = new Drone(model2);
  Drone *Drone3 = new Drone(model3);
  Drone *Drone4 = new Drone(model4);
  Drone *Drone5 = new Drone(model5);

  // model "Q-36-01"
  EXPECT_EQ(Drone1->GetWeight(), 50);
  EXPECT_EQ(Drone1->GetMaxSpeed(), 60);
  EXPECT_EQ(Drone1->GetBaseAcceleration(), 4);
  EXPECT_EQ(Drone1->GetMaxCapacity(), 10);
  EXPECT_EQ(Drone1->GetMaxBattery(), 300);

  // model "Q-36-02"
  EXPECT_EQ(Drone2->GetWeight(), 200);
  EXPECT_EQ(Drone2->GetMaxSpeed(), 55);
  EXPECT_EQ(Drone2->GetBaseAcceleration(), 4.5);
  EXPECT_EQ(Drone2->GetMaxCapacity(), 20);
  EXPECT_EQ(Drone2->GetMaxBattery(), 10);

  // model "Q-36-03"
  EXPECT_EQ(Drone3->GetWeight(), 5);
  EXPECT_EQ(Drone3->GetMaxSpeed(), 40);
  EXPECT_EQ(Drone3->GetBaseAcceleration(), 5);
  EXPECT_EQ(Drone3->GetMaxCapacity(), 10);
  EXPECT_EQ(Drone3->GetMaxBattery(), 30);

  // model "Q-36-04"
  EXPECT_EQ(Drone4->GetWeight(), 120);
  EXPECT_EQ(Drone4->GetMaxSpeed(), 30);
  EXPECT_EQ(Drone4->GetBaseAcceleration(), 6);
  EXPECT_EQ(Drone4->GetMaxCapacity(), 19);
  EXPECT_EQ(Drone4->GetMaxBattery(), 600);

  // model "Q-36-05"
  EXPECT_EQ(Drone5->GetWeight(), 150);
  EXPECT_EQ(Drone5->GetMaxSpeed(), 60);
  EXPECT_EQ(Drone5->GetBaseAcceleration(), 3);
  EXPECT_EQ(Drone5->GetMaxCapacity(), 15);
  EXPECT_EQ(Drone5->GetMaxBattery(), 120);
}

TEST_F(DroneTests, PhysicsModelTests) {
  // initialize a drone for each model
  picojson::object model1;
  model1["physics-model"] = picojson::value("velocity");
  picojson::object model2;
  model2["physics-model"] = picojson::value("acceleration");
  picojson::object model3;
  model3["physics-model"] = picojson::value("force");
  picojson::object model4;
  model4["physics-model"] = picojson::value("solar");
  picojson::object model5;
  model5["model"] = picojson::value("force");
  Drone *Drone1 = new Drone(model1);
  Drone *Drone2 = new Drone(model2);
  Drone *Drone3 = new Drone(model3);
  Drone *Drone4 = new Drone(model4);
  Drone *Drone5 = new Drone(model5);

  EXPECT_EQ(Drone1->GetPhysicsModel(), "velocity");
  EXPECT_EQ(Drone2->GetPhysicsModel(), "acceleration");
  EXPECT_EQ(Drone3->GetPhysicsModel(), "force");
  EXPECT_EQ(Drone4->GetPhysicsModel(), "solar");
  EXPECT_EQ(Drone5->GetPhysicsModel(), "no physics model");
}

}  // namespace csci3081
