#include "../Headers/Cube.h"

using namespace std;

Cube::Cube()
{
  m_position = {0, 0, 0};
  m_color = {1.0f, 1.0f, 1.0f};
}

Cube::Cube(array<unsigned int, 3> position, array<float, 3> color)
{
  m_position = position;
  m_color = color;
}

array<float, 3> Cube::getColor() const
{
  return m_color;
}

array<unsigned int, 3> Cube::getPosition() const
{
  return m_position;
}

Cube* Cube::setColor(array<float, 3> color)
{
  m_color = color;
  return this;
}

Cube* Cube::setPosition(array<unsigned int, 3> position)
{
  m_position = position;
  return this;
}