#include <iostream>
#include "EntityProject/ANVIL/drone_delivery_system.h"
#include "src/drone.h"
#include "src/package.h"
#include "gtest/gtest.h"
#include "src/route_manager.h"
#include <EntityProject/osm_graph_parser.h>
#include <EntityProject/graph.h>
#include <EntityProject/project_settings.h>
#include "EntityProject/simple_UMN_route_manager.h"
#include <bits/stdc++.h>
#include <queue>
#include <utility>
#include <functional>
#include <cmath>
#include <unordered_map>
#include <map>

namespace csci3081 {
  using entity_project::DroneDeliverySystem;
  using entity_project::Entity;
  using csci3081::Drone;
  using csci3081::Package;
  typedef std::pair<float, std::string> uPair;


class RouteManagerTests : public ::testing::Test {
 protected:
  virtual void SetUp() {
    system = GetDroneDeliverySystem("default");
    const char* droneJsonText_pos0 =
    "{\"direction\":[1,0,0],\"duration\":2,\"mesh\":\"models\\/s9drone.glb\","
    "\"name\":\"drone\",\"position\":[0.0,0.0,"
    "0.0],\"radius\":1,\"rotation\":[0,0,0,0],\"scale\":"
    "[0.10000000000000001,0.10000000000000001,0.10000000000000001],"
      "\"speed\":30,\"start\":2,\"type\":\"drone\"}";
    const char* packageJsonText_pos0 =
      "{\"direction\":[1,0,0],\"mesh\":\"models\\/cardboardBox.glb\","
      "\"name\":\"package\",\"position\":[0.0,0.0,"
      "0.0],"
      "\"radius\":1,\"rotation\":[0,0,0,0],\"scale\":"
      "[0.02,0.02,0.01],\"type\":\"package\"}";
    const char* droneJsonText =
      "{\"direction\":[1,0,0],\"duration\":2,\"mesh\":\"models\\/s9drone.glb\","
      "\"name\":\"drone\",\"position\":[4548.29199999999997,223.88300000000001,"
      "-22.62299999999999],\"radius\":1,\"rotation\":[0,0,0,0],\"scale\":"
      "[0.10000000000000001,0.10000000000000001,0.10000000000000001],"
        "\"speed\":30,\"start\":2,\"type\":\"drone\"}";
    const char* packageJsonText =
        "{\"direction\":[1,0,0],\"mesh\":\"models\\/cardboardBox.glb\","
        "\"name\":\"package\",\"position\":[100,100,100],"
        "\"radius\":1,\"rotation\":[0,0,0,0],\"scale\":"
        "[0.02,0.02,0.01],\"type\":\"package\"}";

    const char* droneJsonText_max_pos =
        "{\"direction\":[1,0,0],\"duration\":2,\"mesh\":\"models\\/s9drone.glb\","
        "\"name\":\"drone\",\"position\":[3.40282e+38,3.40282e+38,"
        "3.40282e+38],\"radius\":1,\"rotation\":[0,0,0,0],\"scale\":"
        "[0.10000000000000001,0.10000000000000001,0.10000000000000001],"
          "\"speed\":30,\"start\":2,\"type\":\"drone\"}";

      picojson::value drone_pos0;
      picojson::value package_pos0;
      picojson::value drone_max_pos;

      picojson::parse(drone_pos0, droneJsonText_pos0);
      picojson::parse(package_pos0, packageJsonText_pos0);
      picojson::parse(drone_max_pos, droneJsonText_max_pos);

      droneJson_pos0 = drone_pos0.get<picojson::object>();
      packageJson_pos0 = package_pos0.get<picojson::object>();
      droneJson_pos_max = drone_max_pos.get<picojson::object>();


      picojson::value droneV;
      picojson::value packageV;
      picojson::parse(droneV, droneJsonText);
      picojson::parse(packageV, packageJsonText);
      droneJson = droneV.get<picojson::object>();
      packageJson = packageV.get<picojson::object>();

      graph = parser.CreateGraph("data/umn.osm",
        "data/umn-height.csv");

      system->SetGraph(graph);
      nodes = graph->GetNodes();
      // Drone* parameterDrone = new Drone(droneJson);
      // Package* package = new Package(packageJson);
  }
  virtual void TearDown() {
       delete graph;
      // delete parameterDrone;
      // delete package;


      // for (auto p : nodes)
      //   {
      //       delete p;
      //   }
      //   nodes.clear();
  }
  DroneDeliverySystem* system;
  picojson::value::object droneJson;
  picojson::value::object packageJson;
  picojson::value::object droneJson_pos0;
  picojson::value::object packageJson_pos0;
  picojson::value::object droneJson_pos_max;


  const entity_project::IGraph* graph;

  entity_project::OSMGraphParser parser;

  std::vector<entity_project::IGraphNode*> nodes;
  std::vector<SimpleUMNRoutePoint> currentRoute_;
  SimpleUMNRouteManager simpleUMNRouteManager;
  std::pair<std::vector<SimpleUMNRoutePoint>, std::vector<std::string>> route_cRoute_;
  std::vector<std::string> current_route_;
  std::string start_node_name;
  std::string end_node_name;
};
/*******************************************************************************
 * TEST CASES
 ******************************************************************************/
// checking that from point {0,0,0} to {0,0,0} matches up with google calc of euclid distance

TEST_F(RouteManagerTests, test_RouteDistance_0s) {
  Drone* parameterDrone_pos0 = new Drone(droneJson_pos0);
  Package* package_pos0 = new Package(packageJson_pos0);

  EXPECT_EQ(RouteManager::Instance()->RouteDistance(parameterDrone_pos0, package_pos0), 0);

  delete parameterDrone_pos0;
  delete package_pos0;
}

// Check that GenerateRoute functions when passed entities both at {0,0,0}
TEST_F(RouteManagerTests, test_drone_and_package_at_start) {
  Drone* parameterDrone_pos0 = new Drone(droneJson_pos0);
  Package* package_pos0 = new Package(packageJson_pos0);

  route_cRoute_ = RouteManager::Instance()->GenerateRouteDjikstra(parameterDrone_pos0,
    package_pos0);
  currentRoute_ = route_cRoute_.first;
  current_route_ = route_cRoute_.second;

  EXPECT_LT(currentRoute_.size(), 2);

  delete parameterDrone_pos0;
  delete package_pos0;
}
// Check that GenerateRoute functions when passed two entites both
// at some identical arbitrary position
TEST_F(RouteManagerTests, test_GenerateRoute_same_point) {
  Drone* parameterDrone = new Drone(droneJson);

  route_cRoute_ = RouteManager::Instance()->GenerateRouteDjikstra(parameterDrone, parameterDrone);
  currentRoute_ = route_cRoute_.first;
  current_route_ = route_cRoute_.second;

  EXPECT_LT(currentRoute_.size(), 2);

  delete parameterDrone;
}
// Confirm that all nodes have neighbors
TEST_F(RouteManagerTests, test_all_nodes_neighbors) {
  bool no_unconected_graphs = true;

  for (int i = 0; i < nodes.size(); i++) {
    if (nodes[i]->GetNeighbors().size() == 0) {
      no_unconected_graphs = false;
    }
  }

  EXPECT_EQ(no_unconected_graphs, true);
}
// confirm graph is connected
TEST_F(RouteManagerTests, test_graph_connected) {
// the majority of this test is a copy paste of dijkstras implemented in RouteManager.GenerateRoute
// at the end there will be additional code which loops through all the nodes
// and confrirms that they are all reachable

// // After function call
// auto start = high_resolution_clock::now();

std::vector<SimpleUMNRoutePoint> route;


// a dummy drone to act stand in for the start/end points
Drone* parameterDrone = new Drone(droneJson);

// route start point
const std::vector<float> srcPosition = parameterDrone->GetPosition();
SimpleUMNRoutePoint srcPoint(srcPosition[0], srcPosition[1], srcPosition[2]);
// route end point
const std::vector<float> destPosition = parameterDrone->GetPosition();
SimpleUMNRoutePoint destPoint(destPosition[0], destPosition[1], destPosition[2]);
// create the route


std::vector<float> startpos = {srcPosition[0], srcPosition[1], srcPosition[2]};
std::vector<float> endpos = {destPosition[0], destPosition[1], destPosition[2]};
float shortestdis_startnode = std::numeric_limits<float>::max();
float shortestdis_endnode = std::numeric_limits<float>::max();


bool found_path_to_end = false;

std::unordered_map<std::string, bool> thenodes_visited_map;
std::unordered_map<std::string, float> thenodes_shortest_dist_map;
std::unordered_map<std::string, float> thenodes_shortest_dist_map_removeable;
std::unordered_map<std::string, std::string> thenodes_prev_node_name_map;
std::unordered_map<std::string, int> original_index;
std::unordered_map<std::string, int> visited_map;
std::multimap<float, std::string> thefixer;
std::priority_queue<   uPair, vector<uPair> , std::greater<uPair> > pq;
std::unordered_map<int, std::string> original_string;
std::map<float, std::string> shortest_dist_from_origin_map1;
std::map<float, std::string> shortest_dist_from_origin_map2;

int listiter = 0;


  for ( int i = 0; i < nodes.size(); i++ ) {
    std::string temp_node_name = nodes[i]->GetName();
    thenodes_visited_map[temp_node_name] = false;
    thenodes_shortest_dist_map[temp_node_name] = 10000000000;
    thenodes_shortest_dist_map_removeable[temp_node_name] = 10000000000;
    original_index[temp_node_name] = i;
    original_string[i] = temp_node_name;
  }
  for (int i = 0; i < nodes.size(); i++) {
    float distostart = sqrt(pow((startpos[0] - nodes[i]->GetPosition()[0]), 2)
                          + pow((startpos[1] - nodes[i]->GetPosition()[1]), 2)
                          + pow((startpos[2] - nodes[i]->GetPosition()[2]), 2));

    float distoend = sqrt(pow((endpos[0] - nodes[i]->GetPosition()[0]), 2)
                        + pow((endpos[1] - nodes[i]->GetPosition()[1]), 2)
                        + pow((endpos[2] - nodes[i]->GetPosition()[2]), 2));

    if (distostart < shortestdis_startnode) {
      shortestdis_startnode = distostart;
      start_node_name = nodes[i]->GetName();
    }
    if (distoend < shortestdis_endnode) {
      shortestdis_endnode = distoend;
      end_node_name = nodes[i]->GetName();
    }
  }
  thenodes_shortest_dist_map[start_node_name] = 0;
  thenodes_shortest_dist_map_removeable[start_node_name] = 0;
  std::string current_node_name = start_node_name;

  uPair tempUpair;
  for ( int i = 0; i < nodes.size(); i++ ) {
    if (thenodes_visited_map[current_node_name] != true) {
        std::vector<entity_project::IGraphNode*> neighbors
        = nodes[original_index[current_node_name]]->GetNeighbors();

        for ( int i = 0; i < neighbors.size(); i++ ) {
                float current_to_current_neighbor = sqrt(
                        pow((nodes[original_index[current_node_name]]->GetPosition()[0]
                        - neighbors[i]->GetPosition()[0]), 2)
                        + pow((nodes[original_index[current_node_name]]->GetPosition()[1]
                        - neighbors[i]->GetPosition()[1]), 2)
                        + pow((nodes[original_index[current_node_name]]->GetPosition()[2]
                        - neighbors[i]->GetPosition()[2]), 2));

                if (current_to_current_neighbor
                + thenodes_shortest_dist_map[current_node_name]
                < thenodes_shortest_dist_map[neighbors[i]->GetName()]) {
                  thenodes_shortest_dist_map[neighbors[i]->GetName()]
                  = current_to_current_neighbor + thenodes_shortest_dist_map[current_node_name];

                  tempUpair.first = current_to_current_neighbor +
                   thenodes_shortest_dist_map[current_node_name];
                  tempUpair.second = neighbors[i]->GetName();
                  pq.push(tempUpair);

                  thenodes_shortest_dist_map_removeable[neighbors[i]->GetName()]
                  = current_to_current_neighbor + thenodes_shortest_dist_map[current_node_name];

                  thenodes_prev_node_name_map[neighbors[i]->GetName()]
                  = nodes[original_index[current_node_name]]->GetName();
                }
        }
        thenodes_visited_map[current_node_name] = true;
        // thenodes_shortest_dist_map_removeable.erase(current_node_name);
        listiter++;
    } else {
        i--;
    }

    std::unordered_map<std::string, float>::iterator itt =
    thenodes_shortest_dist_map_removeable.begin();
    std::string next_node = " ";
    float next_node_float = 100000000000;
    uPair nextnodepair;
    bool foundnext = 0;

    while (foundnext == 0 && !pq.empty()) {
      nextnodepair = pq.top();
      pq.pop();
      if (thenodes_visited_map[nextnodepair.second] == false) {
      foundnext = 1;
      }
    }


       current_node_name = nextnodepair.second;
  }

bool no_unconected_graphs = true;
std::unordered_map<std::string, bool>::iterator it;
for (it = thenodes_visited_map.begin(); it != thenodes_visited_map.end(); it++) {
     if (it->second == false) {
       no_unconected_graphs = false;
     }
  }
std::unordered_map<std::string, float>::iterator ittt;

for (ittt = thenodes_shortest_dist_map.begin(); ittt != thenodes_shortest_dist_map.end(); ittt++) {
       if (ittt->second == 10000000000) {
         no_unconected_graphs = false;
       }
     }

  EXPECT_EQ(no_unconected_graphs, true);
  delete parameterDrone;
}
// loadtest, GenerateRoute calls in quick succession
TEST_F(RouteManagerTests, GenerateRoute_loadtest) {
  Drone* parameterDrone = new Drone(droneJson);
  Drone* parameterDrone_pos0 = new Drone(droneJson_pos0);

  route_cRoute_ = RouteManager::Instance()->GenerateRouteDjikstra(parameterDrone_pos0,
    parameterDrone);
  route_cRoute_ = RouteManager::Instance()->GenerateRouteDjikstra(parameterDrone,
    parameterDrone_pos0);
  route_cRoute_ = RouteManager::Instance()->GenerateRouteDjikstra(parameterDrone_pos0,
    parameterDrone);
  route_cRoute_ = RouteManager::Instance()->GenerateRouteDjikstra(parameterDrone_pos0,
    parameterDrone_pos0);

  currentRoute_ = route_cRoute_.first;
  current_route_ = route_cRoute_.second;

  EXPECT_LT(currentRoute_.size(), 2);

  delete parameterDrone_pos0;
  delete parameterDrone;
}
// Test Route String passing to Drone
TEST_F(RouteManagerTests, Test_max_possition) {
  Drone* parameterDrone_max_pos = new Drone(droneJson_pos_max);

  route_cRoute_ = RouteManager::Instance()->GenerateRouteDjikstra(parameterDrone_max_pos,
                                    parameterDrone_max_pos);

  currentRoute_ = route_cRoute_.first;
  current_route_ = route_cRoute_.second;
  EXPECT_LT(currentRoute_.size(), 2);

  delete parameterDrone_max_pos;
}

TEST_F(RouteManagerTests, Test_basic_route_functionality) {
  Drone* parameterDrone_pos0 = new Drone(droneJson_pos0);
  Drone* parameterDrone = new Drone(droneJson);

  route_cRoute_ = RouteManager::Instance()->GenerateRouteDjikstra(parameterDrone_pos0,
    parameterDrone);

  currentRoute_ = route_cRoute_.first;
  current_route_ = route_cRoute_.second;
  EXPECT_LT(currentRoute_.size(), 9000);

  delete parameterDrone_pos0;
  delete parameterDrone;
}
}  // namespace csci3081
