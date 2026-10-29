#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Point.h"
#include <array>
#include <cstddef>
#include <vector>

/**
 * @brief A simple vector triangle for displaying to the screen using OpenGL.
 *
 */
class Triangle {
public:
  /**
   * @brief Construct a new triangle object
   *
   */
  Triangle();

  /**
   * @brief Construct a new triangle object with the provided position and color
   *
   * @param array<Point, 3> The initial vertices of the triangle
   * @param array<float, 3> The initial color of the triangle
   */
  Triangle(std::array<Point, 3> vertices, std::array<float, 3> color);
  /**
   * @brief Get the Current Color of the triangle
   *
   * @return array<float, 3> The current color of the triangle
   */
  std::array<float, 3> getColor() const;

  /**
   * @brief Get the Vertices of the triangle
   *
   * @return array<Point, 3> The current position of the triangle
   */
  std::array<Point, 3> getPosition() const;

  /**
   * @brief Set the Color of the triangle on screen
   *
   * @param array<float, 3> The color to be used for the triangle
   *
   * @return Triangle* A pointer to the current triangle object
   */
  Triangle* setColor(std::array<float, 3> color);

  /**
   * @brief Set the Position of the triangle
   *
   * @param unsigned int The new position of the triangle
   *
   * @return Triangle* A pointer to the current triangle object
   */
  Triangle* setPosition(std::array<Point, 3> position);

private:
  std::array<Point, 3> m_position;                   /// Current position of the triangle
  std::array<float, 3> m_color = {1.0f, 1.0f, 1.0f}; /// current color of the triangle
};

#endif // TRIANGLE_H