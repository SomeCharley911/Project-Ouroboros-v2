
#include "Planet.h"
#include <cmath>

using namespace std;

// constructors
Planet::Planet(Point position): IdItem()
{
  m_corePosition = position;
  m_radius = 1.0F;
  m_surfacePoints = distributePoints(4);
}

Planet::Planet(Point position, float radius, size_t numSurfacePoints): IdItem()
{
  m_corePosition = position;
  m_radius = radius;
  m_surfacePoints = distributePoints(numSurfacePoints);
}

// getters
const Point& Planet::getCorePosition() const
{
  return m_corePosition;
}

const float& Planet::getRadius() const
{
  return m_radius;
}

vector<Point>& Planet::getTopology()
{
  return m_surfacePoints;
}

// private function
vector<Point> Planet::distributePoints(size_t numPoints)
{
  vector<Point> points(numPoints);
  // distribute points evenly across the surface of the planet
  // using the golden spiral algorithm
  const float phi = (1.0f + sqrt(5.0f)) / 2.0f; // golden ratio
  for (size_t i = 0; i < numPoints; ++i) {
    float y = m_radius; // y will be modified later

    float theta = phi * static_cast<float>(i); // golden angle increment

    float x = cos(theta) * m_radius;
    float z = sin(theta) * m_radius;

    points.at(i) = Point({x, y, z});
  }
  return points;
}