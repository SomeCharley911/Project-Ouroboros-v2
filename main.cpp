#include <GLFW/glfw3.h>
#include <array>
#include <iostream>

#include "Headers/Cube.h"

using namespace std;

namespace
{
constexpr float near_plane = 0.1F;
constexpr float far_plane = 100.0F;
constexpr float focal_length = 1.7320508F;

constexpr std::array<std::array<float, 3>, 8> cube_offsets{{
    {{-0.5F, -0.5F, -0.5F}},
    {{0.5F, -0.5F, -0.5F}},
    {{0.5F, 0.5F, -0.5F}},
    {{-0.5F, 0.5F, -0.5F}},
    {{-0.5F, -0.5F, 0.5F}},
    {{0.5F, -0.5F, 0.5F}},
    {{0.5F, 0.5F, 0.5F}},
    {{-0.5F, 0.5F, 0.5F}},
}};

constexpr std::array<std::array<std::size_t, 3>, 12> cube_triangles{{
    {{0, 1, 2}}, {{0, 2, 3}}, // Near face
    {{4, 6, 5}}, {{4, 7, 6}}, // Far face
    {{0, 4, 5}}, {{0, 5, 1}}, // Bottom face
    {{3, 2, 6}}, {{3, 6, 7}}, // Top face
    {{0, 3, 7}}, {{0, 7, 4}}, // Left face
    {{1, 5, 6}}, {{1, 6, 2}}, // Right face
}};

bool projectPoint(const std::array<float, 3>& point, float aspect_ratio, std::array<float, 3>& projected)
{
  const float depth = point[2];
  if (depth <= near_plane) {
    return false;
  }

  projected = {
      point[0] * focal_length / (depth * aspect_ratio),
      point[1] * focal_length / depth,
      (far_plane + near_plane) / (far_plane - near_plane) -
          (2.0F * far_plane * near_plane) / ((far_plane - near_plane) * depth),
  };
  return true;
}

void drawCube(const Cube& cube, float aspect_ratio)
{
  const auto position = cube.getPosition();
  const auto color = cube.getColor();
  std::array<std::array<float, 3>, cube_offsets.size()> world_vertices{};
  std::array<std::array<float, 3>, cube_offsets.size()> screen_vertices{};
  std::array<bool, cube_offsets.size()> visible{};

  for (std::size_t i = 0; i < cube_offsets.size(); ++i) {
    world_vertices[i] = {
        static_cast<float>(position[0]) + cube_offsets[i][0],
        static_cast<float>(position[1]) + cube_offsets[i][1],
        static_cast<float>(position[2]) + cube_offsets[i][2],
    };
    visible[i] = projectPoint(world_vertices[i], aspect_ratio, screen_vertices[i]);
  }

  glColor3f(color[0], color[1], color[2]);
  glBegin(GL_TRIANGLES);
  for (const auto& triangle : cube_triangles) {
    if (!visible[triangle[0]] || !visible[triangle[1]] || !visible[triangle[2]]) {
      continue;
    }
    for (const std::size_t vertex : triangle) {
      glVertex3f(screen_vertices[vertex][0], screen_vertices[vertex][1], screen_vertices[vertex][2]);
    }
  }
  glEnd();
}
} // namespace

int main()
{
#if defined(__linux__)
  glfwInitHint(GLFW_WAYLAND_LIBDECOR, GLFW_WAYLAND_DISABLE_LIBDECOR);
#endif

  const std::array<Cube, 3> cubes{
      Cube({0, 0, 4}, {1.0F, 0.2F, 0.2F}),
      Cube({1, 0, 6}, {0.2F, 0.8F, 0.3F}),
      Cube({0, 1, 7}, {0.2F, 0.4F, 1.0F}),
  };

  if (glfwInit() == GLFW_FALSE) {
    cerr << "Failed to initialize GLFW\n";
    return 1;
  }

  glfwWindowHint(GLFW_DEPTH_BITS, 24);
  GLFWwindow* window = glfwCreateWindow(800, 600, "Game Window", nullptr, nullptr);
  if (window == nullptr) {
    cerr << "Failed to create GLFW window\n";
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(window);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  while (!glfwWindowShouldClose(window)) {
    int framebuffer_width = 0;
    int framebuffer_height = 0;
    glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);
    if (framebuffer_width > 0 && framebuffer_height > 0) {
      glViewport(0, 0, framebuffer_width, framebuffer_height);
    }

    glClearColor(0.08F, 0.08F, 0.1F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (framebuffer_width > 0 && framebuffer_height > 0) {
      const float aspect_ratio = static_cast<float>(framebuffer_width) / static_cast<float>(framebuffer_height);
      for (const Cube& cube : cubes) {
        drawCube(cube, aspect_ratio);
      }
    }

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
