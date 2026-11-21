#include "World.h"

#include <algorithm>
#include <array>
#include <bit>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;
namespace {
using PositionKey = array<uint32_t, 3>;
struct PositionKeyHash {
  size_t operator()(const PositionKey& position) const noexcept
  {
    size_t hash = 0;
    for (uint32_t coordinate : position) {
      hash ^= std::hash<uint32_t>{}(coordinate) + 0x9e3779b9U + (hash << 6) + (hash >> 2);
    }
    return hash;
  }
};

PositionKey makePositionKey(const array<float, 3>& position)
{
  return {
      bit_cast<uint32_t>(position[0]),
      bit_cast<uint32_t>(position[1]),
      bit_cast<uint32_t>(position[2]),
  };
}
} // namespace

bool World::readWorldFile(const string& filename)
{
  ifstream input(filename);
  // test if it opens
  if (!input) {
    cerr << "Failed to open " << filename << " for reading\n";
    return false;
  }

  string line;
  // Read the first line and check for the expected header.
  if (!getline(input, line)) {
    cerr << "Invalid or unsupported world file: " << filename << '\n';
    return false;
  }
  if (!line.empty() && line.back() == '\r') {
    line.pop_back();
  }
  if (line != "OUROBOROS_WORLD 1") {
    std::cerr << "Invalid or unsupported world file: " << filename << '\n';
    return false;
  }

  std::unordered_map<int, Point*> points_by_id;
  std::unordered_map<int, size_t> triangles_by_id;
  std::vector<bool> triangle_in_plate;
  std::string pending_section;
  size_t line_number = 1;
  bool has_pending_section = false;

  const auto read_section = [&](const std::string& expected, auto&& parse_record) {
    const std::string expected_header = "[" + expected + "]";
    if (!has_pending_section) {
      while (std::getline(input, line)) {
        ++line_number;
        const size_t first = line.find_first_not_of(" \t\r");
        if (first == std::string::npos || line[first] == '#') {
          continue;
        }
        const size_t last = line.find_last_not_of(" \t\r");
        pending_section = line.substr(first, last - first + 1);
        has_pending_section = true;
        break;
      }
    }

    if (!has_pending_section || pending_section != expected_header) {
      std::cerr << "Expected " << expected_header << " in " << filename << '\n';
      return false;
    }
    has_pending_section = false;

    while (std::getline(input, line)) {
      ++line_number;
      const size_t first = line.find_first_not_of(" \t\r");
      if (first == std::string::npos || line[first] == '#') {
        continue;
      }
      if (line[first] == '[') {
        const size_t last = line.find_last_not_of(" \t\r");
        pending_section = line.substr(first, last - first + 1);
        has_pending_section = true;
        return true;
      }

      std::istringstream record(line);
      if (!parse_record(record)) {
        std::cerr << "Invalid " << expected << " record at " << filename << ':' << line_number << '\n';
        return false;
      }
    }
    return true;
  };

  if (!read_section("Points", [&](std::istringstream& record) {
        int id = 0;
        std::array<float, 3> position{};
        if (!(record >> id >> position[0] >> position[1] >> position[2]) || id <= 0) {
          return false;
        }
        record >> std::ws;
        if (!record.eof() || points_by_id.contains(id)) {
          return false;
        }
        m_points.emplace_back(position);
        points_by_id.emplace(id, &m_points.back());
        return true;
      })) {
    return false;
  }

  if (!read_section("Triangles", [&](std::istringstream& record) {
        int id = 0;
        std::array<int, 3> point_ids{};
        std::array<float, 3> color{};
        if (!(record >> id >> point_ids[0] >> point_ids[1] >> point_ids[2] >> color[0] >> color[1] >> color[2]) || id <= 0) {
          return false;
        }
        record >> std::ws;
        if (!record.eof() || triangles_by_id.contains(id)) {
          return false;
        }

        std::array<Point*, 3> vertices{};
        for (size_t i = 0; i < vertices.size(); ++i) {
          const auto point = points_by_id.find(point_ids[i]);
          if (point == points_by_id.end()) {
            return false;
          }
          vertices[i] = point->second;
        }
        triangles_by_id.emplace(id, m_triangles.size());
        m_triangles.emplace_back(vertices, color);
        return true;
      })) {
    return false;
  }
  triangle_in_plate.resize(m_triangles.size(), false);

  if (!read_section("Plates", [&](std::istringstream& record) {
        int id = 0;
        size_t triangle_count = 0;
        if (!(record >> id >> triangle_count) || id <= 0) {
          return false;
        }

        std::vector<Triangle> composition;
        composition.reserve(triangle_count);
        for (size_t i = 0; i < triangle_count; ++i) {
          int triangle_id = 0;
          if (!(record >> triangle_id)) {
            return false;
          }
          const auto triangle = triangles_by_id.find(triangle_id);
          if (triangle == triangles_by_id.end()) {
            return false;
          }
          composition.push_back(m_triangles[triangle->second]);
          triangle_in_plate[triangle->second] = true;
        }
        record >> std::ws;
        if (!record.eof()) {
          return false;
        }
        m_plates.emplace_back(std::move(composition));
        return true;
      })) {
    return false;
  }

  if (!read_section("Planets", [&](std::istringstream& record) {
        int id = 0;
        int core_id = 0;
        float radius = 0.0F;
        size_t surface_point_count = 0;
        if (!(record >> id >> core_id >> radius >> surface_point_count) || id <= 0) {
          return false;
        }
        const auto core = points_by_id.find(core_id);
        if (core == points_by_id.end()) {
          return false;
        }

        auto planet = std::make_unique<Planet>(*core->second, radius, 0);
        for (size_t i = 0; i < surface_point_count; ++i) {
          int point_id = 0;
          if (!(record >> point_id)) {
            return false;
          }
          const auto surface_point = points_by_id.find(point_id);
          if (surface_point == points_by_id.end()) {
            return false;
          }
          planet->getTopology().emplace_back(surface_point->second->getPosition());
        }
        record >> std::ws;
        if (!record.eof()) {
          return false;
        }
        m_planets.emplace_back(std::move(planet));
        return true;
      })) {
    return false;
  }

  for (size_t i = 0; i < m_triangles.size(); ++i) {
    if (!triangle_in_plate[i]) {
      m_unplated_triangles.push_back(m_triangles[i]);
    }
  }
  return true;
}

bool World::loadFromFile(const std::string& filename)
{
  if (!readWorldFile(filename)) {
    return false;
  }
  return true;
}

bool World::saveToFile(const std::string& filename) const
{
  for (const auto& planet : m_planets) {
    if (planet == nullptr) {
      std::cerr << "Cannot save a world with a null planet\n";
      return false;
    }
  }

  std::ofstream output(filename);
  if (!output) {
    std::cerr << "Failed to open " << filename << " for writing\n";
    return false;
  }

  output << std::setprecision(std::numeric_limits<float>::max_digits10);
  output << "OUROBOROS_WORLD 1\n\n[Points]\n# point_id x y z\n";

  std::unordered_map<const Point*, int> point_ids;
  std::unordered_map<PositionKey, int, PositionKeyHash> point_ids_by_position;
  int next_point_id = 1;
  const auto write_point = [&](const Point& point) {
    const int id = next_point_id++;
    const auto position = point.getPosition();
    output << id << ' ' << position[0] << ' ' << position[1] << ' ' << position[2] << '\n';
    return id;
  };

  for (const Point& point : m_points) {
    const int id = write_point(point);
    point_ids.emplace(&point, id);
    point_ids_by_position.emplace(makePositionKey(point.getPosition()), id);
  }

  const auto find_or_write_point = [&](const Point& point) {
    const PositionKey key = makePositionKey(point.getPosition());
    const auto found = point_ids_by_position.find(key);
    if (found != point_ids_by_position.end()) {
      return found->second;
    }
    const int id = write_point(point);
    point_ids_by_position.emplace(key, id);
    return id;
  };

  std::vector<int> planet_core_ids;
  std::vector<std::vector<int>> planet_surface_ids;
  planet_core_ids.reserve(m_planets.size());
  planet_surface_ids.reserve(m_planets.size());
  for (const auto& planet : m_planets) {
    planet_core_ids.push_back(find_or_write_point(planet->getCorePosition()));
    std::vector<int> surface_ids;
    const auto& topology = planet->getTopology();
    surface_ids.reserve(topology.size());
    for (const Point& point : topology) {
      surface_ids.push_back(find_or_write_point(point));
    }
    planet_surface_ids.push_back(std::move(surface_ids));
  }

  using VertexIds = std::array<int, 3>;
  std::map<VertexIds, std::vector<size_t>> triangle_indices_by_vertices;
  output << "\n[Triangles]\n# triangle_id point_id_1 point_id_2 point_id_3 red green blue\n";
  for (size_t i = 0; i < m_triangles.size(); ++i) {
    const Triangle& triangle = m_triangles[i];
    VertexIds vertex_ids{};
    output << i + 1;
    for (size_t vertex = 0; vertex < vertex_ids.size(); ++vertex) {
      const auto point = point_ids.find(triangle.getPosition()[vertex]);
      if (point == point_ids.end()) {
        std::cerr << "Cannot save a triangle whose point is not in the world\n";
        return false;
      }
      vertex_ids[vertex] = point->second;
      output << ' ' << point->second;
    }
    for (float color : triangle.getColor()) {
      output << ' ' << color;
    }
    output << '\n';
    triangle_indices_by_vertices[vertex_ids].push_back(i);
  }

  output << "\n[Plates]\n# plate_id triangle_count triangle_id...\n";
  for (size_t i = 0; i < m_plates.size(); ++i) {
    const auto& composition = m_plates[i].getComposition();
    output << i + 1 << ' ' << composition.size();
    for (const Triangle& triangle : composition) {
      VertexIds vertex_ids{};
      for (size_t vertex = 0; vertex < vertex_ids.size(); ++vertex) {
        const auto point = point_ids.find(triangle.getPosition()[vertex]);
        if (point == point_ids.end()) {
          std::cerr << "Cannot save a plate with a triangle point outside the world\n";
          return false;
        }
        vertex_ids[vertex] = point->second;
      }

      const auto matches = triangle_indices_by_vertices.find(vertex_ids);
      if (matches == triangle_indices_by_vertices.end()) {
        std::cerr << "Cannot save a plate with a triangle absent from the world\n";
        return false;
      }
      const auto match = std::find_if(matches->second.begin(), matches->second.end(), [&](size_t index) { return m_triangles[index].getColor() == triangle.getColor(); });
      if (match == matches->second.end()) {
        std::cerr << "Cannot save a plate with a triangle absent from the world\n";
        return false;
      }
      output << ' ' << *match + 1;
    }
    output << '\n';
  }

  output << "\n[Planets]\n# planet_id core_point_id radius surface_point_count surface_point_id...\n";
  for (size_t i = 0; i < m_planets.size(); ++i) {
    const Planet& planet = *m_planets[i];
    output << i + 1 << ' ' << planet_core_ids[i] << ' ' << planet.getRadius() << ' ' << planet_surface_ids[i].size();
    for (int point_id : planet_surface_ids[i]) {
      output << ' ' << point_id;
    }
    output << '\n';
  }

  if (!output) {
    std::cerr << "Failed while writing " << filename << '\n';
    return false;
  }
  return true;
}
