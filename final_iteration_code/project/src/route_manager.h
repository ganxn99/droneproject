#ifndef ROUTE_MANAGER_H_
#define ROUTE_MANAGER_H_

#include <EntityProject/ANVIL/drone_delivery_system.h>
#include "EntityProject/simple_UMN_route_manager.h"
#include "EntityProject/entity.h"
#include <vector>
#include <cmath>
#include <unordered_map>
#include <map>
#include <EntityProject/osm_graph_parser.h>
#include <EntityProject/graph.h>
#include <EntityProject/entity_console_logger.h>
#include "src/json_helper.h"
#include <string>
#include <bits/stdc++.h>
#include <queue>
#include <utility>
#include <functional>
#include "src/a_star_node.h"

using entity_project::SimpleUMNRoutePoint;
using entity_project::SimpleUMNRouteManager;

namespace csci3081 {

/// @brief A singleton class used to generate routes and determine/store info about them.
/**
 * This class can generate routes of SimpleUMNPoints using either the Djikstra or A Star pathfinding
 *  algorithms, as well as cache previously created routes, and determine the distance along routes. 
 */
class RouteManager {
 public:
  /**
   * @brief Returns the single, static instance of this class.
   */
  static RouteManager* Instance();
  /**
   * @brief Sets the nodes.
   */
  void SetNodes(const std::vector<entity_project::IGraphNode*>& nodes);
  /**
  * @brief Returns total distance along route generated from src to dest.
  *
  * @param[in] src entity_project::Entity - source entity of route
  * @param[in] dest entity_project::Entity - destination entity of route
  *
  * @return float distance of route between source and destination entities.
  */
  float RouteDistance(entity_project::Entity* src, entity_project::Entity* dest);

  /**
  * @brief Generates a travel route (of SimpleUMNRoutePoint objects) from src to dest using
  *  Djikstra's pathfinding algorithm.
  *
  * The route generated does not contain the src point given, but does contain the dest.
  *
  * @param[in] src entity_project::Entity - source entity of route
  * @param[in] dest entity_project::Entity - destination entity of route
  *
  * @return vector array of SimpleUMNRoutePoint objects that make up the route from src to dest.
  */
  std::pair<std::vector<SimpleUMNRoutePoint>, std::vector<std::string>> GenerateRouteDjikstra(
    entity_project::Entity* src, entity_project::Entity* dest);

  /**
  * @brief Generates a travel route (of SimpleUMNRoutePoint objects) from src to dest using
  *  the A Star pathfinding algorithm.
  *
  * The route generated does not contain the src point given, but does contain the dest.
  *
  * @param[in] src entity_project::Entity - source entity of route
  * @param[in] dest entity_project::Entity - destination entity of route
  *
  * @return vector array of SimpleUMNRoutePoint objects that make up the route from src to dest.
  */
  std::pair<std::vector<SimpleUMNRoutePoint>, std::vector<std::string>> GenerateRouteAStar(
    entity_project::Entity* src, entity_project::Entity* dest);

 private:
  /*
   * @brief The private constructor for the static Singleton.
   */
  RouteManager() {}
  static RouteManager* instance_;  //!< The singleton instance.
  /*
   * @brief The vector of nodes on the map graph.
   */
  std::vector<entity_project::IGraphNode*> nodes_;
  /*
   * @brief Stores the nodes in a format more accessible to the AStar algortihm.
   */
  std::vector<AStarNode*> aStarNodes_;
  /**
   * @brief A cache that stores previously generated routes for fast lookup.
   *  The key for lookup is a contactanation of the points' names.
   */
  std::unordered_map<std::string, std::pair<std::vector<SimpleUMNRoutePoint>,
    std::vector<std::string>>> routeCache_;
  /**
   * @brief Returns a value between two nodes based on a heuristic (usually euclidean distance) for 
   *  use in the a star algorithm.
   */
  float Heuristic(AStarNode* a, AStarNode* b);
  /**
   * @brief Determines the indices of the nearest A Star nodes to the start and end positions
   *  passed.
   */
  std::pair<int, int> NearestNodes(std::vector<float> src, std::vector<float> dest);
};

}  // namespace csci3081

#endif
