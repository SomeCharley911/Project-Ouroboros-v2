#include "../Headers/Triangle.h"

Triangle::Triangle() = default;

Triangle::Triangle(std::array<Point, 3> vertices, std::array<float, 3> color)
    : m_position(vertices), m_color(color)
{
}

std::array<float, 3> Triangle::getColor() const
{
  return m_color;
}

std::array<Point, 3> Triangle::getPosition() const
{
  return m_position;
}

Triangle* Triangle::setColor(std::array<float, 3> color)
{
  m_color = color;
  return this;
}

Triangle* Triangle::setPosition(std::array<Point, 3> position)
{
  m_position = position;
  return this;
}