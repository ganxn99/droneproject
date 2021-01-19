#include "gtest/gtest.h"
#include "EntityProject/ANVIL/drone_delivery_system.h"
#include "src/package.h"
#include <EntityProject/project_settings.h>

namespace csci3081 {

using entity_project::DroneDeliverySystem;
using csci3081::Package;

class PackageTests : public ::testing::Test {
 protected:
  virtual void SetUp() {
    system = GetDroneDeliverySystem("default");
    const char* packageJsonText =
    "{\"direction\":[1,0,0],\"mesh\":\"models\\/cardboardBox.glb\","
    "\"name\":\"package\",\"position\":[-255.994,257.798,-296.85899999999998],"
    "\"radius\":1,\"rotation\":[0,0,0,0],\"scale\":"
    "[0.02,0.02,0.01],\"type\":\"package\"}";
    picojson::value packageV;
    picojson::parse(packageV, packageJsonText);
    packageJson = packageV.get<picojson::object>();
  }
  virtual void TearDown() {}

  DroneDeliverySystem* system;
  picojson::value::object packageJson;
};
/*******************************************************************************
 * Test Cases
 ******************************************************************************/

// default constructor test
TEST_F(PackageTests, DefaultConstructorTest) {
  Package* defaultPackage = new Package();

  EXPECT_NE(defaultPackage->AsType<Package>(), nullptr);

  delete defaultPackage;
}

// parameter constructor test
TEST_F(PackageTests, ParameterConstructorTest) {
  Package* parameterPackage = new Package(packageJson);

  EXPECT_EQ(picojson::value(parameterPackage->GetDetails()).serialize(),
            picojson::value(packageJson).serialize());

  delete parameterPackage;
}

// test that the expected messages are returned when calling GetNotification()
TEST_F(PackageTests, PackageGetNotificationTest) {
    Package* package = new Package(packageJson);
    const char* notification_text_1 ="{\"type\":\"notify\", \"value\": \"scheduled\"}";
    const char* notification_text_2 ="{\"type\":\"notify\", \"value\": \"en route\"}";
    const char* notification_text_3 ="{\"type\":\"notify\", \"value\": \"delivered\"}";
    picojson::value notification0;
    picojson::value notification1;
    picojson::value notification2;
    picojson::parse(notification0, notification_text_1);
    picojson::parse(notification1, notification_text_2);
    picojson::parse(notification2, notification_text_3);
    EXPECT_EQ(picojson::value(package->GetNotification(0)).serialize(), notification0.serialize());
    EXPECT_EQ(picojson::value(package->GetNotification(1)).serialize(), notification1.serialize());
    EXPECT_EQ(picojson::value(package->GetNotification(2)).serialize(), notification2.serialize());
}

}  // namespace csci3081
