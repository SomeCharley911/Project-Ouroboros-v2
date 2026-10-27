#include "../Triangle.h"

#include <utility>

using namespace std;

Triangle::Triangle() = default;

Triangle::Triangle(array<array<float, 2>, 3> vertices, array<float, 3> color)
{
  m_vertices = move(vertices);
  m_color = move(color);
}

array<float, 3> Triangle::getColor() const
{
  return m_color;
}

array<array<float, 2>, 3> Triangle::getVertices() const
{
  return m_vertices;
}

Triangle* Triangle::setColor(array<float, 3> color)
{
  m_color = color;
  return this;
}

Triangle* Triangle::setVertex(size_t location, array<float, 2> vertex)
{
  m_vertices.at(location) = vertex;
  return this;
}

Triangle* Triangle::setVertices(array<array<float, 2>, 3> new_vertices)
{
  m_vertices = new_vertices;
  return this;
}