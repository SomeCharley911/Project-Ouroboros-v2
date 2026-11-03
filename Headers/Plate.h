#ifndef PLATE_H
#define PLATE_H

#include "Triangle.h"
#include <cstddef>
#include <set>

/**
 * @brief A simple tectonic plate for world generation
 *
 */
class Plate {
public:
  /**
   * @brief Construct a new plate object
   *
   */
  Plate();

  /**
   * @brief Construct a new plate object with the provided composition and color
   *
   * @param vector<Triangle> The initial composition of the plate
   */
  Plate(std::set<Triangle> composition);
  /**
   * @brief Get the Vertices of the plate
   *
   * @return vector<Triangle> The current composition of the plate
   */
  const std::set<Triangle>& getComposition() const;

  /**
   * @brief Set the Composition of the plate
   *
   * @param vector<Triangle> The new composition of the plate
   *
   * @return Plate* A pointer to the current plate object
   */
  Plate* setComposition(std::set<Triangle> composition);

  /**
   * @brief Add a single triangle to the plate's composition
   *
   * @param Triangle The triangle to be added
   *
   * @return Plate* A pointer to the current plate object
   */
  Plate* addTriangle(Triangle triangle);

  /**
   * @brief Remove a single triangle from the plate's composition
   *
   * @param long The ID of the triangle to be removed
   *
   * @return bool True if the triangle was removed, false otherwise
   */
  bool removeTriangle(long id);

private:
  std::vector<Triangle> m_composition; /// Current composition of the plate
};

#endif // PLATE_H