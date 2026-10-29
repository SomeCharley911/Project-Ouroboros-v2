#include "../Headers/Plate.h"

using namespace std;

Plate::Plate() = default;

Plate::Plate(vector<Triangle> composition)
{
  m_composition = composition;
}

const vector<Triangle>& Plate::getComposition() const
{
  return m_composition;
}

Plate* Plate::setComposition(vector<Triangle> composition)
{
  m_composition = composition;
  return this;
}

Plate* Plate::addTriangle(Triangle triangle)
{
  m_composition.push_back(triangle);
  return this;
}

bool Plate::removeTriangle(long id)
{
  for (size_t i = 0; i < m_composition.size(); ++i) {
    if (m_composition[i].getId() == id) {
      m_composition.erase(m_composition.begin() + i);
      return true;
    }
  }
  return false;
}