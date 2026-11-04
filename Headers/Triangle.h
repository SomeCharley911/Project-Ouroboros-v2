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
   * @param const array<Point, 3>& The initial vertices of the triangle
   * @param const array<float, 3>& The initial color of the triangle
   */
  Triangle(const std::array<Point*, 3>& vertices, const std::array<float, 3>& color);
  /**
   * @brief Get the Current Color of the triangle
   *
   * @return const array<float, 3>& The current color of the triangle
   */
  const std::array<float, 3>& getColor() const;

  /**
   * @brief Get the Vertices of the triangle
   *
   * @return const array<Point, 3>& The current position of the triangle
   */
  const std::array<Point*, 3>& getPosition() const;

  /**
   * @brief Get the unique identifier of the triangle
   *
   * @return long The unique identifier of the triangle
   */
  long getId() const;

  /**
   * @brief Set the Color of the triangle on screen
   *
   * @param const array<float, 3>& The color to be used for the triangle
   *
   * @return Triangle* A pointer to the current triangle object
   */
  Triangle* setColor(const std::array<float, 3>& color);

  /**
   * @brief Set the Position of the triangle
   *
   * @param const array<Point, 3>& The new position of the triangle
   *
   * @return Triangle* A pointer to the current triangle object
   */
  Triangle* setPosition(const std::array<Point*, 3>& position);

  /**
   * @brief Compares the IDs of triangles for ordered containers
   *
   * @param other The other triangle to compare to
   * @return true if this triangle has a smaller ID
   */
  bool operator<(const Triangle& other) const;

  /**
   * @brief Compares the IDs of triangles
   *
   * @param other The other triangle to compare to
   * @return true if this triangle has a greater ID
   */
  bool Triangle::operator>(const Triangle& other) const;

private:
  std::array<Point*, 3> m_position;                  /// Current position of the triangle
  std::array<float, 3> m_color = {1.0f, 1.0f, 1.0f}; /// current color of the triangle
  long m_id = reinterpret_cast<long>(this);          /// Unique identifier for the triangle
};

#endif // TRIANGLE_H