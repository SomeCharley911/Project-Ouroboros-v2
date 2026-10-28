#ifndef CUBE_H
#define CUBE_H

#include <array>
#include <cstddef>
#include <vector>

/**
 * @brief A simple vector cube for displaying to the screen using OpenGL.
 *
 */
class Cube {
public:
  /**
   * @brief Construct a new cube object
   *
   */
  Cube();

  /**
   * @brief Construct a new cube object with the provided position and color
   *
   * @param array<unsigned int, 3> The initial position of the cube
   * @param array<float, 3> The initial color of the cube
   */
  Cube(std::array<unsigned int, 3> position, std::array<float, 3> color);
  /**
   * @brief Get the Current Color of the cube
   *
   * @return array<float, 3> The current color of the cube
   */
  std::array<float, 3> getColor() const;

  /**
   * @brief Get the Position of the cube
   *
   * @return unsigned int The current position of the cube
   */
  std::array<unsigned int, 3> getPosition() const;

  /**
   * @brief Set the Color of the cube on screen
   *
   * @param array<float, 3> The color to be used for the cube
   *
   * @return Cube* A pointer to the current cube object
   */
  Cube* setColor(std::array<float, 3> color);

  /**
   * @brief Set the Position of the cube
   *
   * @param unsigned int The new position of the cube
   *
   * @return Cube* A pointer to the current cube object
   */
  Cube* setPosition(std::array<unsigned int, 3> position);

private:
  std::array<unsigned int, 3> m_position;            /// Current position of the cube
  std::array<float, 3> m_color = {1.0f, 1.0f, 1.0f}; /// current color of the cube
};

#endif // CUBE_H