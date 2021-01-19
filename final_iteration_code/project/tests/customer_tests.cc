#include "gtest/gtest.h"
#include "EntityProject/ANVIL/drone_delivery_system.h"
#include "src/customer.h"
#include <EntityProject/project_settings.h>

namespace csci3081 {

using entity_project::DroneDeliverySystem;
using csci3081::Customer;

class CustomerTests : public ::testing::Test {
 protected:
  virtual void SetUp() {
    system = GetDroneDeliverySystem("default");
    const char* customerJsonText =
    "{\"direction\":[1,0,0],\"mesh\":\"models\\/RobotExpressive.glb\","
    "\"name\":\"customer\",\"position\":[-951.41200000000003,254.66499999999999,"
    "298.27100000000002],\"radius\":1,\"rotation\":[0,0,0,0],\"scale\":"
    "[0.25,0.25,0.25],\"type\":\"customer\"}";
    picojson::value customerV;
    picojson::parse(customerV, customerJsonText);
    customerJson = customerV.get<picojson::object>();
  }
  virtual void TearDown() {}

  DroneDeliverySystem* system;
  picojson::value::object customerJson;
};
/*******************************************************************************
 * Test Cases
 ******************************************************************************/

// default constructor test
TEST_F(CustomerTests, DefaultConstructorTest) {
  Customer* defaultCustomer = new Customer();

  EXPECT_NE(defaultCustomer->AsType<Customer>(), nullptr);

  delete defaultCustomer;
}

// parameter constructor test
TEST_F(CustomerTests, ParameterConstructorTest) {
  Customer* parameterCustomer = new Customer(customerJson);

  EXPECT_EQ(picojson::value(parameterCustomer->GetDetails()).serialize(),
            picojson::value(customerJson).serialize());

  delete parameterCustomer;
}

}  // namespace csci3081
