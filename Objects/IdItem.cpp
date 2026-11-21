#include "IdItem.h"

using namespace std;

std::uint32_t IdItem::s_idCounter = 0;

IdItem::IdItem()
{
  m_id = new std::uint32_t(++s_idCounter);
}

IdItem::~IdItem()
{
  delete m_id;
}

IdItem& IdItem::operator=(const IdItem& other)
{
  // don't overide the id of the object, as it is unique to each object
  // using assignment means both objects already exist and have ids
  // so don't change them
  if (this != &other) {
    // do nothing
  }
  return *this;
}

int IdItem::getId() const
{
  return *m_id;
}