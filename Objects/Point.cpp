#include "../Headers/Point.h"
#include <stdexcept>

using namespace std;

// constructors
Point::Point(): IdItem()
{
  m_x = new float(0);
  m_y = new float(0);
  m_z = new float(0);
}

Point::Point(const array<float, 3>& position): IdItem()
{
  m_x = new float(position[0]);
  m_y = new float(position[1]);
  m_z = new float(position[2]);
}

Point::Point(const Point& other): IdItem()
{
  m_x = new float(*other.m_x);
  m_y = new float(*other.m_y);
  m_z = new float(*other.m_z);
}

// destructor
Point::~Point()
{
  delete m_x;
  delete m_y;
  delete m_z;
}

Point* Point::operator=(const Point& other)
{
  if (this != &other) {
    *m_x = *other.m_x;
    *m_y = *other.m_y;
    *m_z = *other.m_z;
  }
  return this;
}

bool Point::operator<(const Point& other)
{
  if (getId() < other.getId())
    return true;
  return false;
}

array<float, 3> Point::getPosition() const
{
  return {*m_x, *m_y, *m_z};
}

Point* Point::setPosition(const array<float, 3>& position)
{
  *m_x = position[0];
  *m_y = position[1];
  *m_z = position[2];
  return this;
}

float* Point::at(unsigned short index)
{
  switch (index) {
  case 0:
    return m_x;
  case 1:
    return m_y;
  case 2:
    return m_z;
  default:
    throw out_of_range("Index must be 0, 1, or 2 for x, y, z coordinates.");
  }
}
float* Point::at(unsigned short index) const
{
  // just use the nonconst version and assume the compiler will complain at the implementation mess up
  return const_cast<Point*>(this)->at(index);
}