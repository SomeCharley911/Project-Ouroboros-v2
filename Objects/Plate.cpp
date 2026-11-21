#include "../Headers/Plate.h"

#include <utility>

using namespace std;

Plate::Plate() = default;

Plate::Plate(vector<Triangle> composition): m_composition(move(composition))
{
}

const vector<Triangle>& Plate::getComposition() const
{
  return m_composition;
}

Plate* Plate::setComposition(vector<Triangle> composition)
{
  m_composition = move(composition);
  return this;
}

Plate* Plate::addTriangle(Triangle triangle)
{
  m_composition.emplace_back(triangle);
  return this;
}

bool Plate::removeTriangle(Triangle triangle)
{
  for (size_t i = 0; i < m_composition.size(); ++i) {
    if (m_composition.at(i) == triangle) {
      m_composition.erase(m_composition.begin() + i);
      return true;
    }
  }
  return false;
}
