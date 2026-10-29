#ifndef POINT_H
#define POINT_H

#include <array>
#include <cstddef>
#include <vector>

/**
 * @brief A simple vector point for displaying to the screen using OpenGL.
 *
 */
class Point {
public:
  /**
   * @brief Construct a new point object
   *
   */
  Point();

  /**
   * @brief Construct a new point object with the provided position and color
   *
   * @param array<float, 3> The initial position of the point
   */
  Point(std::array<float, 3> position);

  /**
   * @brief Get the Position of the point
   *
   * @return array<float, 3> The current position of the point
   */
  std::array<float, 3> getPosition() const;

  /**
   * @brief Set the Position of the point
   *
   * @param array<float, 3> The new position of the point
   *
   * @return Point* A pointer to the current point object
   */
  Point* setPosition(std::array<float, 3> position);

private:
  std::array<float, 3> m_position; /// Current position of the point
};

#endif // POINT_H