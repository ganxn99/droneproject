#ifndef FLOAT_VECTOR_FUNCTIONS_H_
#define FLOAT_VECTOR_FUNCTIONS_H_

#include <cmath>
#include <vector>

namespace csci3081 {

/// @brief A class used to run operations on float* vectors.
/**
 * This class can compute distance between and magnitude of `vector<float>`s.
 */
class FloatVectorFunctions {
 public:
  /**
  * @brief Distance between two vector<float> positions.
  *
  * @param[in] pos1 vector<float> - x, y, z, first position
  * @param[in] pos2 vector<float> - x, y, z, second position
  *
  * @return float distance between pos1 and pos2.
  */
  static float Distance(const std::vector<float>& pos1, const std::vector<float>& pos2)
    { return sqrt(pow(pos1[0]-pos2[0], 2) + pow(pos1[1]-pos2[1], 2) + pow(pos1[2]-pos2[2], 2)); }

  /**
  * @brief Squared distance between two vector<float> positions.
  *  Useful when actual distance doesn't need to be known (i.e. distances are only being compared) 
  *  so that the unefficient sqrt calculation can be removed.
  *
  * @param[in] pos1 vector<float> - x, y, z, first position
  * @param[in] pos2 vector<float> - x, y, z, second position
  *
  * @return float squared distance between pos1 and pos2.
  */
  static float SqrDistance(const std::vector<float>& pos1, const std::vector<float>& pos2)
    { return pow(pos1[0]-pos2[0], 2) + pow(pos1[1]-pos2[1], 2) + pow(pos1[2]-pos2[2], 2); }

  /**
  * @brief Magnitude of a direction vector.
  *
  * @param[in] dir vector<float> - x, y, z, direction vector
  *
  * @return float magnitude of the direction vector.
  */
  static float Magnitude(const std::vector<float>& dir)
    { return sqrt(pow(dir[0], 2) + pow(dir[1], 2) + pow(dir[2], 2)); }
};

}  // namespace csci3081

#endif  // FLOAT_VECTOR_FUNCTIONS_H_
