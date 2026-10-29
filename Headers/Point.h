#ifndef POINT_H
#define POINT_H

#include <array>
#include <cstddef>
#include <vector>

/**
 * @brief A simple vector point
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
  Point(const std::array<float, 3>& position);

  /**
   * @brief Get the Position of the point
   *
   * @return const array<float, 3>& The current position of the point
   */
  const std::array<float, 3>& getPosition() const;

  /**
   * @brief Set the Position of the point
   *
   * @param const array<float, 3>& The new position of the point
   *
   * @return Point* A pointer to the current point object
   */
  Point* setPosition(const std::array<float, 3>& position);

private:
  std::array<float, 3> m_position{0.0F, 0.0F, 0.0F}; /// Current position of the point
};

#endif // POINT_H