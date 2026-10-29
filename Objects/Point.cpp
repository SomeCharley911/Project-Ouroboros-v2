#include "../Headers/Point.h"

using namespace std;
Point::Point()
{
  m_position = {0, 0, 0};
}

Point::Point(array<float, 3> position)
{
  m_position = position;
}

array<float, 3> Point::getPosition() const
{
  return m_position;
}

Point* Point::setPosition(array<float, 3> position)
{
  m_position = position;
  return this;
}