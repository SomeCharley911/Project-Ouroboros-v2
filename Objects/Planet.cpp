
#include "Planet.h"

Planet::Planet(Point position, std::vector<Point> surfacePoints): IdItem()
{
  m_corePosition = position;
  m_radius = 1.0F;
  m_surfacePoints = surfacePoints;
}

Planet::Planet(Point position, float radius, std::vector<Point> surfacePoints): IdItem()
{
  m_corePosition = position;
  m_radius = radius;
  m_surfacePoints = surfacePoints;
}

const Point& Planet::getCorePosition() const
{
  return m_corePosition;
}