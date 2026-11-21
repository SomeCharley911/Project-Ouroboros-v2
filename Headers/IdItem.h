#ifndef IDITEM_H
#define IDITEM_H

#include <cstdint>
/**
 * @brief A simple class that provides a unique identifier for objects that inherit from it.
 *
 */
class IdItem {
public:
  /**
   * @brief Construct a new IdItem object and assign a unique identifier to it
   *
   */
  IdItem();

  /**
   * @brief Copy constructor for the IdItem object [DO NOT USE]
   *
   * @param IdItem& const The IdItem object to copy from
   */
  IdItem(const IdItem& other) = delete; // Disable copy constructor to prevent copying of IdItem objects

  /**
   * @brief Destroy the IdItem object
   *
   */
  virtual ~IdItem();

  /**
   * @brief Assignment operator for the IdItem object
   *
   * @param  IdItem& const The IdItem object to assign from
   *
   * @return IdItem& A reference to the current IdItem object
   */
  virtual IdItem& operator=(const IdItem& other);

  /**
   * @brief Get the unique identifier of the IdItem object
   *
   * @return int The unique identifier of the IdItem object
   */
  int getId() const;

private:
  static std::uint32_t s_idCounter; /// Static counter to generate unique identifiers
  std::uint32_t* m_id;              /// Unique identifier for the IdItem object
};

#endif // IDITEM_H
