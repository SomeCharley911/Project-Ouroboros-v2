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
  // constructors
  /**
   * @brief Construct a new triangle object
   *
   */
  Triangle(const std::array<Point*, 3>& vertices);

  /**
   * @brief Construct a new triangle object with the provided position and color
   *
   * @param const array<Point, 3>& The initial vertices of the triangle
   * @param const array<float, 3>& The initial color of the triangle
   */
  Triangle(const std::array<Point*, 3>& vertices, const std::array<float, 3>& color);

  // operators
  /**
   * @brief equality operator for the triangle object
   *
   * @param const Triangle& The triangle object to compare with
   *
   * @return true If the triangles are equal
   * @return false Otherwise
   */
  bool operator==(const Triangle& other) const;

  // getters
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

  // setters
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
   * @brief Get the unique identifier of the triangle
   *
   * @return long The unique identifier of the triangle
   */
  // long getId() const; //not implemented yet, but may be used for future features

private:
  // inline static long s_idCounter = 0;                /// Static counter for unique triangle IDs
  std::array<Point*, 3> m_position; /// Current position of the triangle
  std::array<float, 3> m_color;     /// current color of the triangle
  // long m_id;                                         /// Unique identifier for the triangle
};

#endif // TRIANGLE_H