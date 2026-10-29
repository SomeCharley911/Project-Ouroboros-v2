#include "../Headers/Triangle.h"

using namespace std;

Triangle::Triangle() = default;

Triangle::Triangle(const array<Point, 3>& vertices, const array<float, 3>& color)
{
  m_position = vertices;
  m_color = color;
}

const array<float, 3>& Triangle::getColor() const
{
  return m_color;
}

const array<Point, 3>& Triangle::getPosition() const
{
  return m_position;
}

Triangle* Triangle::setColor(const array<float, 3>& color)
{
  m_color = color;
  return this;
}

Triangle* Triangle::setPosition(const array<Point, 3>& position)
{
  m_position = position;
  return this;
}

long Triangle::getId() const
{
  return m_id;
}