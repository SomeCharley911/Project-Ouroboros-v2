#include "../Headers/Triangle.h"

using namespace std;

Triangle::Triangle(const array<Point*, 3>& vertices)
{
  m_position = vertices;
  m_color = {0.0f, 0.0f, 0.0f};
  // m_id = ++s_idCounter;
}

Triangle::Triangle(const array<Point*, 3>& vertices, const array<float, 3>& color)
{
  m_position = vertices;
  m_color = color;
  // m_id = ++s_idCounter;
}

bool Triangle::operator==(const Triangle& other) const
{
  for (size_t i = 0; i < 3; ++i) {
    if (m_position[i]->getId() != other.m_position[i]->getId()) {
      return false;
    }
  }
  return true;
}
// getters
const array<float, 3>& Triangle::getColor() const
{
  return m_color;
}

const array<Point*, 3>& Triangle::getPosition() const
{
  return m_position;
}

// setters
Triangle* Triangle::setColor(const array<float, 3>& color)
{
  m_color = color;
  return this;
}

Triangle* Triangle::setPosition(const array<Point*, 3>& position)
{
  m_position = position;
  return this;
}
/*
long Triangle::getId() const
{
  return m_id;
}
*/