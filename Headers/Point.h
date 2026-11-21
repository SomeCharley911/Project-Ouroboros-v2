#ifndef POINT_H
#define POINT_H

#include "IdItem.h"
#include <array>

/**
 * @brief A simple vector point
 *
 */
class Point: public IdItem {
public:
  // don't actually use this please, it is just here to make the compiler happy
  using IdItem::operator=;

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

  // rule of three for the point class
  /**
   * @brief Copy constructor for the point object
   *
   * @param Point& const The point object to copy from
   */
  Point(const Point& other);

  /**
   * @brief Destroy the point object and its heap-allocated members
   *
   */
  ~Point();

  /**
   * @brief Assignment operator for the point object
   *
   * @param Point& const The point object to assign from
   *
   * @return Point* A pointer to the current point object
   */
  Point* operator=(const Point& other);

  // normal operations
  /**
   * @brief Less-than operator for the point object
   *
   * @param Point& const The point object to compare with
   *
   * @return true If the current point's ID is less than the other point's ID
   * @return false Otherwise
   */
  bool operator<(const Point& other); // useful standard algorithms that require sorting.
  /**
   * @brief Get the Position of the point
   *
   * @return array<float, 3>& const The current position of the point
   */
  std::array<float, 3> getPosition() const;

  /**
   * @brief Set the Position of the point
   *
   * @param array<float, 3>& const The new position of the point
   *
   * @return Point* A pointer to the current point object
   */
  Point* setPosition(const std::array<float, 3>& position);

  /**
   * @brief Get a pointer to the coordinate of the point at the specified index
   *
   * @param unsigned_short The index of the coordinate to retrieve (0 for x, 1 for y, 2 for z)
   *
   * @return float* A pointer to the requested coordinate
   */
  float* at(unsigned short index);

  /**
   * @brief Get a pointer to the coordinate of the point at the specified index
   *
   * @param unsigned_short The index of the coordinate to retrieve (0 for x, 1 for y, 2 for z)
   *
   * @return float* A pointer to the requested coordinate
   */
  float* at(unsigned short index) const;

private:
  float* m_x; /// Pointer to the x coordinate of the point
  float* m_y; /// Pointer to the y coordinate of the point
  float* m_z; /// Pointer to the z coordinate of the point
};

#endif // POINT_H