#include "gtest/gtest.h"
#include "EntityProject/ANVIL/drone_delivery_system.h"
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
using csci3081::Package;

class PackageObserverTests : public ::testing::Test {
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
// ensure package and observer_manager are accessing the same vectors
TEST_F(PackageObserverTests, MatchingObserverPointers) {
     std::vector<Package*> packages;
     packages.push_back(system->CreateEntity(packageJson)->AsType<Package>());
     packages.push_back(system->CreateEntity(packageJson)->AsType<Package>());
     system->AddEntity(packages[0]);
     system->AddEntity(packages[1]);
     WebSceneViewer* web_scene_viewer_0_;
     WebSceneViewer* web_scene_viewer_1_;
     EntityConsoleLogger* entity_console_logger_0_;
     EntityConsoleLogger* entity_console_logger_1_;
     system->AddObserver(packages[0], web_scene_viewer_0_);
     system->AddObserver(packages[0], web_scene_viewer_1_);
     system->AddObserver(packages[0], entity_console_logger_0_);
     system->AddObserver(packages[0], entity_console_logger_1_);
     EXPECT_EQ((*(packages[0]->GetObservers()))[0], web_scene_viewer_0_);
     EXPECT_EQ((*(packages[0]->GetObservers()))[1], web_scene_viewer_1_);
     EXPECT_EQ((*(packages[0]->GetObservers()))[2], entity_console_logger_0_);
     EXPECT_EQ((*(packages[0]->GetObservers()))[3], entity_console_logger_1_);
     WebSceneViewer* web_scene_viewer_2_;
     WebSceneViewer* web_scene_viewer_3_;
     EntityConsoleLogger* entity_console_logger_2_;
     EntityConsoleLogger* entity_console_logger_3_;
     system->AddObserver(packages[1], web_scene_viewer_2_);
     system->AddObserver(packages[1], web_scene_viewer_3_);
     system->AddObserver(packages[1], entity_console_logger_2_);
     system->AddObserver(packages[1], entity_console_logger_3_);
     EXPECT_EQ((*(packages[1]->GetObservers()))[0], web_scene_viewer_2_);
     EXPECT_EQ((*(packages[1]->GetObservers()))[1], web_scene_viewer_3_);
     EXPECT_EQ((*(packages[1]->GetObservers()))[2], entity_console_logger_2_);
     EXPECT_EQ((*(packages[1]->GetObservers()))[3], entity_console_logger_3_);
     // EXPECT_EQ((*(packages[0]->GetObservers())).size(), 0);
}

// package add and remove observer test
TEST_F(PackageObserverTests, AddingRemovingPackageObservers) {
     std::vector<Package*> packages;
     packages.push_back(system->CreateEntity(packageJson)->AsType<Package>());
     system->AddEntity(packages[0]);

     std::fstream file;
     file.open("cout.txt", std::ios::out);
     std::streambuf* stream_buffer_cout = std::cout.rdbuf();
     std::streambuf* stream_buffer_cin = std::cin.rdbuf();
     std::streambuf* stream_buffer_file = file.rdbuf();
     std::cout.rdbuf(stream_buffer_file);

     WebSceneViewer* web_scene_viewer_0_ = new WebSceneViewer(9000, "web");
     EntityConsoleLogger* entity_console_logger_0_ = new EntityConsoleLogger;
     EntityConsoleLogger* entity_console_logger_1_ = new EntityConsoleLogger;
     EntityConsoleLogger* entity_console_logger_2_ = new EntityConsoleLogger;

     std::cout.rdbuf(stream_buffer_cout);
     file.close();

     // std::cout << web_scene_viewer_0_ << std::endl;
     // std::cout << web_scene_viewer_1_ << std::endl;
     // std::cout << entity_console_logger_0_ << std::endl;
     // std::cout << entity_console_logger_1_ << std::endl;
     system->AddObserver(packages[0], web_scene_viewer_0_);
     system->AddObserver(packages[0], entity_console_logger_0_);
     system->AddObserver(packages[0], entity_console_logger_1_);
     system->AddObserver(packages[0], entity_console_logger_2_);
     EXPECT_EQ((*(packages[0]->GetObservers())).size(), 4);
     system->RemoveObserver(packages[0], web_scene_viewer_0_);
     EXPECT_EQ((*(packages[0]->GetObservers())).size(), 3);
     system->RemoveObserver(packages[0], entity_console_logger_0_);
     EXPECT_EQ((*(packages[0]->GetObservers())).size(), 2);
     system->RemoveObserver(packages[0], entity_console_logger_1_);
     EXPECT_EQ((*(packages[0]->GetObservers())).size(), 1);
     system->RemoveObserver(packages[0], entity_console_logger_2_);
     EXPECT_EQ((*(packages[0]->GetObservers())).size(), 0);
}

}  // namespace csci3081
