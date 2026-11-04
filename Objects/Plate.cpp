#include "../Headers/Plate.h"

#include <utility>

using namespace std;

Plate::Plate() = default;

Plate::Plate(set<Triangle> composition)
    : m_composition(move(composition))
{
}

const set<Triangle>& Plate::getComposition() const
{
  return m_composition;
}

Plate* Plate::setComposition(set<Triangle> composition)
{
  m_composition = move(composition);
  return this;
}

Plate* Plate::addTriangle(Triangle triangle)
{
  m_composition.insert(move(triangle));
  return this;
}

bool Plate::removeTriangle(long id)
{
  size_t temp = m_composition.size();
  erase_if(m_composition, [id](const Triangle& item) { return item.getId() == id; });
  if (temp > m_composition.size())
    return true;
  return false;
}