#ifndef A_STAR_NODE_H_
#define A_STAR_NODE_H_

#include <limits>

namespace csci3081 {

struct AStarNode {
	/**
     * @brief Parameter constructor.
     *
     * @param[in] ign IGraphNode - IGraphNode we want to base this node off of
     */
    explicit AStarNode(entity_project::IGraphNode* ign) : iGraphNode(ign) {}

    entity_project::IGraphNode* iGraphNode;  //!< IGraphNode this AStar node is based off of.
    bool visited = false;  //!< Whether or not this node has been visited in the algorithm.
    /**
     * @brief Used in AStar to determine best node. 
     */
    float globalGoal = std::numeric_limits<float>::infinity();
    /**
     * @brief Used in AStar to determine distance from start node.
     */
    float localGoal = std::numeric_limits<float>::infinity();
    AStarNode* parent = nullptr;  //!< Parent of this node in the currently determined route.
};

}  // namespace csci3081

#endif  // A_STAR_NODE_H_
