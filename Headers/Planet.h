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
   * @param std::vector<Point> surfacePoints The points that make up the surface of the planet
   */
  Planet(Point position, std::vector<Point> surfacePoints);

  /**
   * @brief Construct a new planet object with the provided position and radius
   *
   * @param Point position The initial position of the planet
   * @param float radius The initial radius of the planet
   * @param std::vector<Point> surfacePoints The points that make up the surface of the planet
   */
  Planet(Point position, float radius, std::vector<Point> surfacePoints);

  /**
   * @brief Get the Position of the planet
   *
   * @return const Point& The current position of the planet
   */
  const Point& getCorePosition() const;

private:
  Point m_corePosition;               /// Current position of the planet
  float m_radius;                     /// general radius of the planet
  std::vector<Point> m_surfacePoints; /// Collection of points that make up the surface of the planet
};
#endif // PLANET_H
