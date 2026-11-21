#ifndef WORLD_H
#define WORLD_H

#include "Planet.h"
#include "Plate.h"
#include "Point.h"
#include "Triangle.h"
#include <deque>
#include <memory>
#include <string>
#include <vector>

class World {
public:
  /** @brief Loads the world from a file.
   *  @param filename The name of the file to load.
   *  @return True if the file was loaded successfully, false otherwise.
   */
  bool loadFromFile(const std::string& filename);

  /** @brief Saves the world to a file.
   *  @param filename The name of the file to save.
   *  @return True if the file was saved successfully, false otherwise.
   */
  bool saveToFile(const std::string& filename) const;

  std::deque<Point> m_points;                     /// Collection of points that make up the world
  std::vector<Triangle> m_triangles;              /// Collection of triangles that make up the world
  std::vector<Plate> m_plates;                    /// Collection of plates that make up the world
  std::vector<std::unique_ptr<Planet>> m_planets; /// Collection of planets that make up the world
  std::vector<Triangle> m_unplated_triangles;     /// Collection of triangles that are not part of any plate in the world

private:
  /** @brief Reads the world from a file.
   *  @param filename The name of the file to read.
   *  @param world The world to populate.
   *  @return True if the file was read successfully, false otherwise.
   */
  bool readWorldFile(const std::string& filename);
  /** @brief Writes the world to a file.
   *  @param filename The name of the file to write.
   *  @param world The world to write.
   *  @return True if the file was written successfully, false otherwise.
   */
  bool writeWorldFile(const std::string& filename) const;
};

#endif // WORLD_H
