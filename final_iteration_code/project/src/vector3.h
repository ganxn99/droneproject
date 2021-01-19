#ifndef VECTOR3_H_
#define VECTOR3_H_

#include <cmath>
#include <vector>

namespace csci3081 {

/// @brief A class built solely to handle 3D vector mathematics.
class Vector3 {
 public:
  /**
   * @brief Default constructor.
   */
  Vector3() : x(0), y(0), z(0) {}
  /**
   * @brief Parameter constructor.
   */
  Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
  /**
   * @brief Gets x value.
   */
  float GetX() { return x; }
  /**
   * @brief Gets y value.
   */
  float GetY() { return y; }
  /**
   * @brief Gets z value.
   */
  float GetZ() { return z; }
  /**
   * @brief Distance between two Vector3.
   */
  float Distance(const Vector3& v)
    { return sqrt(pow(this->x-v.x, 2) + pow(this->y-v.y, 2) + pow(this->z-v.z, 2)); }
  /**
   * @brief Magnitude of a Vector3.
   */
  float Magnitude()
    { return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2)); }
  /**
   * @brief Converts float[3] to Vector3D.
   */
  static Vector3 FloatToVector3(const float* v) { return Vector3(v[0], v[1], v[2]); }
  /**
   * @brief Converts vector<float> to Vector3D.
   */
  static Vector3 FloatToVector3(const std::vector<float>& v) { return Vector3(v[0], v[1], v[2]); }
  /**
   * @brief Overrides + operator.
   */
  Vector3 operator+(const Vector3& v)
    { return Vector3(this->x + v.x, this->y + v.y, this->z + v.z); }
  /**
   * @brief Overrides - operator.
   */
  Vector3 operator-(const Vector3& v)
    { return Vector3(this->x - v.x, this->y - v.y, this->z - v.z); }
  /**
   * @brief Overrides * operator.
   */
  Vector3 operator*(float m) { return Vector3(m * this->x, m * this->y, m * this->z); }
  /**
   * @brief Overrides / operator.
   */
  Vector3 operator/(float m) { return Vector3(this->x / m, this->y / m, this->z / m); }

 private:
  float x = 0;  //!< x-coordinate
  float y = 0;  //!< y-coordinate
  float z = 0;  //!< z-coordinate
};

}  // namespace csci3081

#endif  // VECTOR3_H_
