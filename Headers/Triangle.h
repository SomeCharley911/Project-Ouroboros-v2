#ifndef TRIANGLE_H
#define TRIANGLE_H

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
   * @brief Construct a new Triangle object with an empty program
   *
   */
  Triangle();

  /**
   * @brief Construct a new Triangle object with the provided program
   *
   * @param array<std::array<float, 2>, 3> The vertices of the triangle
   * @param array<float, 3> The color to be used for the triangle
   */
  Triangle(std::array<std::array<float, 2>, 3> vertices, std::array<float, 3> color);
  /**
   * @brief Get the Current Color of the triangle
   *
   * @return array<float, 3> The current color of the triangle
   */
  std::array<float, 3> getColor() const;

  /**
   * @brief Get the Vertices of the triangle
   *
   * @return array<std::array<float, 2>, 3> The vertices of the triangle
   */
  std::array<std::array<float, 2>, 3> getVertices() const;

  /**
   * @brief Set the Color of the triangle on screen
   *
   * @param std::array<float, 3> The color to be used for the triangle
   *
   * @return Triangle* A pointer to the current triangle object
   */
  Triangle* setColor(std::array<float, 3> color);

  /**
   * @brief Set the Vertex of the triangle
   *
   * @param size_t The location of the vertex to be set (0, 1, or 2)
   * @param std::array<float, 2> The vertex to be set at the specified location
   *
   * @return Triangle* A pointer to the current triangle object
   */
  Triangle* setVertex(std::size_t location, std::array<float, 2> vertex);

  /**
   * @brief Set the Vertices of the triangle
   *
   * @param std::array<std::array<float, 2>, 3> The vertices to be set
   *
   * @return Triangle* A pointer to the current triangle object
   */
  Triangle* setVertices(std::array<std::array<float, 2>, 3> vertices);

private:
  std::array<std::array<float, 2>, 3> m_vertices = {{{-1, 0}, {0, 1}, {1, 0}}}; /// Current vertices of the triangle
  std::array<float, 3> m_color = {1.0f, 1.0f, 1.0f};                            /// current color of the triangle
};

#endif // TRIANGLE_H