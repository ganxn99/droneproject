#include "src/route_manager.h"
#include "src/float_vector_functions.h"
#include <limits>

using entity_project::SimpleUMNRoutePoint;

namespace csci3081 {

typedef std::pair<float, std::string> uPair;

RouteManager* RouteManager::instance_;

RouteManager* RouteManager::Instance() {
  if (!instance_) {
    instance_ = new RouteManager();
  }
  return instance_;
}

void RouteManager::SetNodes(const std::vector<entity_project::IGraphNode*>& nodes) {
  nodes_ = nodes;
  routeCache_.clear();
  aStarNodes_.clear();
  for (int i = 0; i < nodes.size(); i++) {
    aStarNodes_.push_back(new AStarNode(nodes[i]));
  }
}

float RouteManager::RouteDistance(entity_project::Entity* src, entity_project::Entity* dest) {
  float routeDistance = 0;
  // generate a route from src to dest
  std::vector<SimpleUMNRoutePoint> route = GenerateRouteAStar(src, dest).first;
  // starting point of the drone (isn't included in generated route)
  const std::vector<float> srcPosition = src->GetPosition();
  SimpleUMNRoutePoint srcPoint(srcPosition[0], srcPosition[1], srcPosition[2]);
  // add distance from starting point of drone to starting point of route
  routeDistance += srcPoint.DistanceBetween(route[0]);
  // sum the distance of the entire route
  for (int i = 0; i < route.size()-1; i++) {
    routeDistance += route[i].DistanceBetween(route[i+1]);
  }
  // return the route distance
  return routeDistance;
}

std::pair<std::vector<SimpleUMNRoutePoint>, std::vector<std::string>>
  RouteManager::GenerateRouteDjikstra(entity_project::Entity* src, entity_project::Entity* dest) {
  // // After function call
  // auto start = high_resolution_clock::now();

  SimpleUMNRouteManager simpleUMNRouteManager;
  std::vector<SimpleUMNRoutePoint> route;
  // route start point
  const std::vector<float> srcPosition = src->GetPosition();
  SimpleUMNRoutePoint srcPoint(srcPosition[0], srcPosition[1], srcPosition[2]);
  // route end point
  const std::vector<float> destPosition = dest->GetPosition();
  SimpleUMNRoutePoint destPoint(destPosition[0], destPosition[1], destPosition[2]);
  // create the route


  std::vector<float> startpos = {srcPosition[0], srcPosition[1], srcPosition[2]};
  std::vector<float> endpos = {destPosition[0], destPosition[1], destPosition[2]};
  float shortestdis_startnode = std::numeric_limits<float>::max();
  float shortestdis_endnode = std::numeric_limits<float>::max();

  std::string start_node_name;
  std::string end_node_name;
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


    for ( int i = 0; i < nodes_.size(); i++ ) {
      std::string temp_node_name = nodes_[i]->GetName();
      thenodes_visited_map[temp_node_name] = false;
      thenodes_shortest_dist_map[temp_node_name] = 10000000000;
      thenodes_shortest_dist_map_removeable[temp_node_name] = 10000000000;
      original_index[temp_node_name] = i;
      original_string[i] = temp_node_name;
    }
    for (int i = 0; i < nodes_.size(); i++) {
      float distostart = sqrt(pow((startpos[0] - nodes_[i]->GetPosition()[0]), 2)
                            + pow((startpos[1] - nodes_[i]->GetPosition()[1]), 2)
                            + pow((startpos[2] - nodes_[i]->GetPosition()[2]), 2));

      float distoend = sqrt(pow((endpos[0] - nodes_[i]->GetPosition()[0]), 2)
                          + pow((endpos[1] - nodes_[i]->GetPosition()[1]), 2)
                          + pow((endpos[2] - nodes_[i]->GetPosition()[2]), 2));

      if (distostart < shortestdis_startnode) {
        shortestdis_startnode = distostart;
        start_node_name = nodes_[i]->GetName();
      }
      if (distoend < shortestdis_endnode) {
        shortestdis_endnode = distoend;
        end_node_name = nodes_[i]->GetName();
      }
    }
    thenodes_shortest_dist_map[start_node_name] = 0;
    thenodes_shortest_dist_map_removeable[start_node_name] = 0;
    std::string current_node_name = start_node_name;

    uPair tempUpair;
    for ( int i = 0; i < nodes_.size(); i++ ) {
      if (thenodes_visited_map[current_node_name] != true) {
          std::vector<entity_project::IGraphNode*> neighbors
          = nodes_[original_index[current_node_name]]->GetNeighbors();

          for ( int i = 0; i < neighbors.size(); i++ ) {
                  float current_to_current_neighbor = sqrt(
                          pow((nodes_[original_index[current_node_name]]->GetPosition()[0]
                          - neighbors[i]->GetPosition()[0]), 2)
                          + pow((nodes_[original_index[current_node_name]]->GetPosition()[1]
                          - neighbors[i]->GetPosition()[1]), 2)
                          + pow((nodes_[original_index[current_node_name]]->GetPosition()[2]
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
                    = nodes_[original_index[current_node_name]]->GetName();

                    // shortest_dist_from_origin_map1[current_to_current_neighbor
                    // + thenodes_shortest_dist_map[current_node_name]] = neighbors[i]->GetName();
                    //
                    // shortest_dist_from_origin_map2[current_to_current_neighbor
                    // + thenodes_shortest_dist_map[current_node_name]] = neighbors[i]->GetName();
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

      // while (itt != thenodes_shortest_dist_map_removeable.end()) {
      //       if ( (itt->second < next_node_float) &&
      //        (thenodes_visited_map[itt->first] != true) ) {
      //         next_node_float = itt->second;
      //         next_node = itt->first;
      //       }
      //       itt++;
      //    }
         current_node_name = nextnodepair.second;
    }
  route.push_back(destPoint);
  std::string node_iter = end_node_name;
  // set the ending point
  std::vector<std::string> routeNodeNames;
  routeNodeNames.push_back(end_node_name);
  while ( node_iter != start_node_name ) {
    SimpleUMNRoutePoint nextPoint(nodes_[original_index[node_iter]]->GetPosition()[0],
      nodes_[original_index[node_iter]]->GetPosition()[1],
      nodes_[original_index[node_iter]]->GetPosition()[2]);
    route.push_back(nextPoint);
    node_iter = thenodes_prev_node_name_map[node_iter];
    routeNodeNames.push_back(node_iter);
  }

  std::reverse(routeNodeNames.begin(), routeNodeNames.end());
  std::reverse(route.begin(), route.end());

  std::unordered_map<std::string, bool>::iterator it;

  std::pair<std::vector<SimpleUMNRoutePoint>, std::vector<std::string>> route_cRoute;
  route_cRoute.first = route;
  route_cRoute.second = routeNodeNames;

  // std::cout << current_route_.size() << std::endl;

  return route_cRoute;
}

std::pair<std::vector<SimpleUMNRoutePoint>, std::vector<std::string>>
  RouteManager::GenerateRouteAStar(entity_project::Entity* src, entity_project::Entity* dest) {
  /*************************************************************************************************
   * Initialization & Reset
   ************************************************************************************************/

  // initialize an unordered map of A Star nodes for fast lookup,
  //  also reset all the A Star nodes to default status
  std::unordered_map<std::string, AStarNode*> unorderedNodes;
  for (int i = 0; i < aStarNodes_.size(); i++) {
    aStarNodes_[i]->visited = false;
    aStarNodes_[i]->globalGoal = std::numeric_limits<float>::infinity();
    aStarNodes_[i]->localGoal = std::numeric_limits<float>::infinity();
    aStarNodes_[i]->parent = nullptr;

    unorderedNodes[aStarNodes_[i]->iGraphNode->GetName()] = aStarNodes_[i];
  }

  /*************************************************************************************************
   * Start & End Node Determination
   ************************************************************************************************/

  // determine which nodes on the graph are closest to the src and dest positions
  std::pair<int, int> minIdxs = NearestNodes(src->GetPosition(), dest->GetPosition());
  // set the start and end nodes to the nearest nodes to them respectively
  AStarNode* startNode = aStarNodes_[minIdxs.first];
  AStarNode* endNode = aStarNodes_[minIdxs.second];

  /*************************************************************************************************
   * Cache Lookup
   ************************************************************************************************/

  // determine name of route between star and end node for easy cache lookup
  std::string routeName = startNode->iGraphNode->GetName() + " " + endNode->iGraphNode->GetName();
  // if the cache already contains a route between them, return it after adding the dest point
  if (routeCache_.find(routeName) != routeCache_.end()) {
    std::pair<std::vector<SimpleUMNRoutePoint>, std::vector<std::string>> route =
      routeCache_[routeName];
    std::vector<float> destPosition = dest->GetPosition();
    SimpleUMNRoutePoint destPoint(destPosition[0], destPosition[1], destPosition[2]);
    route.first.push_back(destPoint);
    return route;
  }

  /*************************************************************************************************
   * A Star Algorithm
   ************************************************************************************************/

  // set up starting conditions
  startNode->localGoal = 0;
  startNode->globalGoal = Heuristic(startNode, endNode);

  // add start node to not tested priority queue
  auto aStarNodeCmp = [](AStarNode* a, AStarNode* b) {return (a->globalGoal > b->globalGoal); };
  std::priority_queue<AStarNode*, std::vector<AStarNode*>, decltype(aStarNodeCmp)> pq(aStarNodeCmp);
  pq.push(startNode);

  // run A Star (without searching every node)
  AStarNode* currentNode = startNode;
  while (!pq.empty() && currentNode != endNode) {  // comment out after && for accuracy
    // remove all nodes that have already been visited
    while (!pq.empty() && pq.top()->visited)
      pq.pop();

    // queue could now technically be empty, in which case we want to terminate
    if (pq.empty())
      break;

    // front of the priority queue is now our best candidate, so make it our current node
    currentNode = pq.top();
    currentNode->visited = true;

    // check each of the current node's neighbors
    for (int i = 0; i < currentNode->iGraphNode->GetNeighbors().size(); i++) {
      // lookup the neighbor in the unordered map based on its igraphnode name
      AStarNode* neighborNode =
        unorderedNodes[currentNode->iGraphNode->GetNeighbors()[i]->GetName()];

      // if the neighbor hasn't been visited before, add it to the queue
      if (!neighborNode->visited)
        pq.push(neighborNode);

      // calculate what the new local goal for this neighbor would be along this route
      float possiblyLowerGoal = currentNode->localGoal +
        FloatVectorFunctions::Distance(currentNode->iGraphNode->GetPosition(),
        neighborNode->iGraphNode->GetPosition());

      // if this new local goal is less than its previous, update the neighbor to use this current
      //  node as its parent / source and update its local/global goals
      if (possiblyLowerGoal < neighborNode->localGoal) {
        neighborNode->parent = currentNode;
        neighborNode->localGoal = possiblyLowerGoal;
        neighborNode->globalGoal = neighborNode->localGoal + Heuristic(neighborNode, endNode);
      }
    }
  }

  /*************************************************************************************************
   * Final Route Creation & Cache Logging
   ************************************************************************************************/

  // retrace the route by traversing through the parent of end node and onwards
  //  and save the backwards retraced route in the following vectors
  std::vector<SimpleUMNRoutePoint> routePoints;
  std::vector<std::string> routeNodeNames;

  // iterate through all parents
  AStarNode* p = endNode;
  while (p->parent != nullptr) {
    std::vector<float> nodePosition = p->iGraphNode->GetPosition();
    SimpleUMNRoutePoint nodePoint(nodePosition[0], nodePosition[1], nodePosition[2]);
    routePoints.push_back(nodePoint);
    routeNodeNames.push_back(p->iGraphNode->GetName());
    p = p->parent;
  }
  // reverse the route because it was constructed from back to front
  std::reverse(routePoints.begin(), routePoints.end());
  std::reverse(routeNodeNames.begin(), routeNodeNames.end());

  // return it in proper format
  std::pair<std::vector<SimpleUMNRoutePoint>, std::vector<std::string>> route;
  route.first = routePoints;
  route.second = routeNodeNames;

  // log it in the cache before adding the final node
  routeCache_[routeName] = route;

  // finally add the destination point which isn't a node
  std::vector<float> destPosition = dest->GetPosition();
  SimpleUMNRoutePoint destPoint(destPosition[0], destPosition[1], destPosition[2]);
  route.first.push_back(destPoint);

  return route;
}

float RouteManager::Heuristic(AStarNode* a, AStarNode* b) {
  return FloatVectorFunctions::Distance(a->iGraphNode->GetPosition(),
      b->iGraphNode->GetPosition());
}

std::pair<int, int> RouteManager::NearestNodes(std::vector<float> src, std::vector<float> dest) {
  // set initial min distances
  float minDistToStart =
    FloatVectorFunctions::SqrDistance(aStarNodes_[0]->iGraphNode->GetPosition(), src);
  float minDistToEnd =
    FloatVectorFunctions::SqrDistance(aStarNodes_[0]->iGraphNode->GetPosition(), dest);
  int minDistToStartIdx = 0;
  int minDistToEndIdx = 0;
  // iterate through remaining nodes to find any closer nodes
  for (int i = 1; i < aStarNodes_.size(); i++) {
    // calculate distances from current node
    float distToStart =
      FloatVectorFunctions::SqrDistance(aStarNodes_[i]->iGraphNode->GetPosition(), src);
    float distToEnd =
      FloatVectorFunctions::SqrDistance(aStarNodes_[i]->iGraphNode->GetPosition(), dest);
    // if distance from current node is smaller, update min distance
    if (distToStart < minDistToStart) {
      minDistToStart = distToStart;
      minDistToStartIdx = i;
    }
    if (distToEnd < minDistToEnd) {
      minDistToEnd = distToEnd;
      minDistToEndIdx = i;
    }
  }

  // return the indexes of the nearest nodes to the src and dest position
  std::pair<int, int> minIdxs(minDistToStartIdx, minDistToEndIdx);
  return minIdxs;
}

}  // namespace csci3081
