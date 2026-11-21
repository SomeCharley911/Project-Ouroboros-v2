#ifndef PLANET_H
#define PLANET_H

#include "IdItem.h"
#include "Point.h"
#include <cstddef>
#include <vector>

class Planet: public IdItem {
  // don't actually use this please, it is just here to make the compiler happy
  using IdItem::operator=;

public:
  /**
   * @brief Construct a new planet object
   *
   * @param Point position The initial position of the planet
   */
  Planet(Point position);

  /**
   * @brief Construct a new planet object with the provided position and radius
   *
   * @param Point position The initial position of the planet
   * @param float radius The initial radius of the planet
   * @param size_t numPoints The number of points to distribute across the surface
   */
  Planet(Point position, float radius, size_t numPoints);

  /**
   * @brief Get the Position of the planet
   *
   * @return const Point& The current position of the planet
   */
  const Point& getCorePosition() const;

  /**
   * @brief Get the radius of the planet
   *
   * @return float The current radius of the planet
   */
  const float& getRadius() const;

  /**
   * @brief Get the surface points of the planet
   *
   * @return const std::vector<Point>& The current surface points of the planet
   */
  std::vector<Point>& getTopology();

  std::vector<Point> m_surfacePoints; /// Collection of points that make up the surface of the planet

private:
  /**
   * @brief Distribute points evenly across the planet's surface
   *
   * @param size_t The number of points to distribute
   */
  std::vector<Point> distributePoints(size_t numPoints);

  Point m_corePosition; /// Current position of the planet
  float m_radius;       /// general radius of the planet
};
#endif // PLANET_H
