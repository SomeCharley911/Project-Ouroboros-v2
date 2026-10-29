#include <GLFW/glfw3.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <utility>
#include <vector>

#include "Headers/Plate.h"
#include "Headers/Point.h"
#include "Headers/Triangle.h"

using namespace std;

namespace {
constexpr float near_plane = 0.1F;
constexpr float far_plane = 100.0F;
constexpr float focal_length = 1.7320508F;
constexpr float movement_speed = 3.0F;
constexpr float rotation_speed = 1.5F;
constexpr float max_pitch = 1.5F;
constexpr float mouse_sensitivity = 0.0025F;
constexpr int floor_half_extent = 200;
constexpr float floor_elevation = -0.5F;
constexpr int plate_side_length = 20;

constexpr array<array<float, 3>, 8> plate_colors{{
    {{0.82F, 0.30F, 0.22F}},
    {{0.20F, 0.57F, 0.78F}},
    {{0.88F, 0.69F, 0.20F}},
    {{0.39F, 0.68F, 0.27F}},
    {{0.68F, 0.35F, 0.75F}},
    {{0.18F, 0.68F, 0.57F}},
    {{0.91F, 0.45F, 0.24F}},
    {{0.34F, 0.46F, 0.82F}},
}};

struct Camera {
  array<float, 3> position{0.0F, 0.0F, 0.0F};
  float yaw = 0.0F;
  float pitch = 0.0F;
};

struct MouseLookState {
  Camera* camera;
  bool captured = true;
  bool first_event = true;
  double last_x = 0.0;
  double last_y = 0.0;
};

void updateMouseLook(GLFWwindow* window, double x_position, double y_position)
{
  auto* state = static_cast<MouseLookState*>(glfwGetWindowUserPointer(window));
  if (state == nullptr || !state->captured) {
    return;
  }

  if (state->first_event) {
    state->last_x = x_position;
    state->last_y = y_position;
    state->first_event = false;
    return;
  }

  const double x_offset = x_position - state->last_x;
  const double y_offset = state->last_y - y_position;
  state->last_x = x_position;
  state->last_y = y_position;

  state->camera->yaw += static_cast<float>(x_offset) * mouse_sensitivity;
  state->camera->pitch = clamp(state->camera->pitch + static_cast<float>(y_offset) * mouse_sensitivity, -max_pitch, max_pitch);
}

constexpr array<array<float, 3>, 8> cube_offsets{{
    {{-0.5F, -0.5F, -0.5F}},
    {{0.5F, -0.5F, -0.5F}},
    {{0.5F, 0.5F, -0.5F}},
    {{-0.5F, 0.5F, -0.5F}},
    {{-0.5F, -0.5F, 0.5F}},
    {{0.5F, -0.5F, 0.5F}},
    {{0.5F, 0.5F, 0.5F}},
    {{-0.5F, 0.5F, 0.5F}},
}};

constexpr array<array<size_t, 3>, 12> cube_triangles{{
    {{0, 1, 2}},
    {{0, 2, 3}}, // Near face
    {{4, 6, 5}},
    {{4, 7, 6}}, // Far face
    {{0, 4, 5}},
    {{0, 5, 1}}, // Bottom face
    {{3, 2, 6}},
    {{3, 6, 7}}, // Top face
    {{0, 3, 7}},
    {{0, 7, 4}}, // Left face
    {{1, 5, 6}},
    {{1, 6, 2}}, // Right face
}};

bool projectPoint(const array<float, 3>& point, float aspect_ratio, array<float, 3>& projected)
{
  const float depth = point[2];
  if (depth <= near_plane) {
    return false;
  }

  projected = {
      point[0] * focal_length / (depth * aspect_ratio),
      point[1] * focal_length / depth,
      (far_plane + near_plane) / (far_plane - near_plane) - (2.0F * far_plane * near_plane) / ((far_plane - near_plane) * depth),
  };
  return true;
}

void updateCamera(GLFWwindow* window, Camera& camera, float delta_time)
{
  const auto pressed = [window](int key) { return glfwGetKey(window, key) == GLFW_PRESS; };

  const float turn = rotation_speed * delta_time;
  if (pressed(GLFW_KEY_LEFT)) {
    camera.yaw -= turn;
  }
  if (pressed(GLFW_KEY_RIGHT)) {
    camera.yaw += turn;
  }
  if (pressed(GLFW_KEY_UP)) {
    camera.pitch = min(camera.pitch + turn, max_pitch);
  }
  if (pressed(GLFW_KEY_DOWN)) {
    camera.pitch = max(camera.pitch - turn, -max_pitch);
  }

  float forward = 0.0F;
  float strafe = 0.0F;
  float vertical = 0.0F;
  if (pressed(GLFW_KEY_W)) {
    forward += 1.0F;
  }
  if (pressed(GLFW_KEY_S)) {
    forward -= 1.0F;
  }
  if (pressed(GLFW_KEY_D)) {
    strafe += 1.0F;
  }
  if (pressed(GLFW_KEY_A)) {
    strafe -= 1.0F;
  }
  if (pressed(GLFW_KEY_E)) {
    vertical += 1.0F;
  }
  if (pressed(GLFW_KEY_Q)) {
    vertical -= 1.0F;
  }

  const float horizontal_length = sqrt(forward * forward + strafe * strafe);
  if (horizontal_length > 0.0F) {
    forward /= horizontal_length;
    strafe /= horizontal_length;
  }

  const float yaw_cos = cos(camera.yaw);
  const float yaw_sin = sin(camera.yaw);
  const float distance = movement_speed * delta_time;
  camera.position[0] += (yaw_sin * forward + yaw_cos * strafe) * distance;
  camera.position[1] += vertical * distance;
  camera.position[2] += (yaw_cos * forward - yaw_sin * strafe) * distance;
}

array<float, 3> toCameraSpace(const array<float, 3>& point, const Camera& camera)
{
  const float relative_x = point[0] - camera.position[0];
  const float relative_y = point[1] - camera.position[1];
  const float relative_z = point[2] - camera.position[2];
  const float yaw_cos = cos(camera.yaw);
  const float yaw_sin = sin(camera.yaw);
  const float pitch_cos = cos(camera.pitch);
  const float pitch_sin = sin(camera.pitch);
  const float yaw_x = yaw_cos * relative_x - yaw_sin * relative_z;
  const float yaw_z = yaw_sin * relative_x + yaw_cos * relative_z;

  return {
      yaw_x,
      pitch_cos * relative_y - pitch_sin * yaw_z,
      pitch_sin * relative_y + pitch_cos * yaw_z,
  };
}

vector<Triangle> makeCube(const array<float, 3>& center, const array<float, 3>& color)
{
  array<Point, cube_offsets.size()> vertices{};
  for (size_t i = 0; i < cube_offsets.size(); ++i) {
    vertices[i] = Point({
        center[0] + cube_offsets[i][0],
        center[1] + cube_offsets[i][1],
        center[2] + cube_offsets[i][2],
    });
  }

  vector<Triangle> triangles;
  triangles.reserve(cube_triangles.size());
  for (const auto& triangle : cube_triangles) {
    triangles.emplace_back(
        array<Point, 3>{vertices[triangle[0]], vertices[triangle[1]], vertices[triangle[2]]},
        color);
  }
  return triangles;
}

vector<Plate> makePlates()
{
  constexpr int plates_per_side = (floor_half_extent * 2) / plate_side_length;
  vector<Plate> plates;
  plates.reserve(static_cast<size_t>(plates_per_side * plates_per_side));

  for (int plate_x = 0; plate_x < plates_per_side; ++plate_x) {
    for (int plate_z = 0; plate_z < plates_per_side; ++plate_z) {
      vector<Triangle> composition;
      composition.reserve(static_cast<size_t>(plate_side_length * plate_side_length * 2));
      const size_t color_index = static_cast<size_t>((plate_x * 3 + plate_z * 5) % static_cast<int>(plate_colors.size()));
      const auto color = plate_colors[color_index];
      const int start_x = -floor_half_extent + plate_x * plate_side_length;
      const int start_z = -floor_half_extent + plate_z * plate_side_length;

      for (int x = start_x; x < start_x + plate_side_length; ++x) {
        for (int z = start_z; z < start_z + plate_side_length; ++z) {
          const float left = static_cast<float>(x);
          const float right = static_cast<float>(x + 1);
          const float near_edge = static_cast<float>(z);
          const float far_edge = static_cast<float>(z + 1);
          const Point near_left({left, floor_elevation, near_edge});
          const Point near_right({right, floor_elevation, near_edge});
          const Point far_left({left, floor_elevation, far_edge});
          const Point far_right({right, floor_elevation, far_edge});

          composition.emplace_back(array<Point, 3>{near_left, near_right, far_right}, color);
          composition.emplace_back(array<Point, 3>{near_left, far_right, far_left}, color);
        }
      }

      plates.emplace_back(std::move(composition));
    }
  }

  return plates;
}

void drawTriangle(const Triangle& triangle, float aspect_ratio, const Camera& camera)
{
  const auto vertices = triangle.getPosition();
  array<array<float, 3>, 3> screen_vertices{};
  bool visible = true;

  for (size_t i = 0; i < vertices.size(); ++i) {
    const auto position = vertices[i].getPosition();
    const auto camera_position = toCameraSpace(position, camera);
    if (!projectPoint(camera_position, aspect_ratio, screen_vertices[i])) {
      visible = false;
      break;
    }
  }

  if (!visible) {
    return;
  }

  const auto color = triangle.getColor();
  glColor3f(color[0], color[1], color[2]);
  for (const auto& vertex : screen_vertices) {
    glVertex3f(vertex[0], vertex[1], vertex[2]);
  }
}

void drawTriangles(const vector<Triangle>& triangles, float aspect_ratio, const Camera& camera)
{
  glBegin(GL_TRIANGLES);
  for (const Triangle& triangle : triangles) {
    drawTriangle(triangle, aspect_ratio, camera);
  }
  glEnd();
}

void drawPlates(const vector<Plate>& plates, float aspect_ratio, const Camera& camera)
{
  glBegin(GL_TRIANGLES);
  for (const Plate& plate : plates) {
    for (const Triangle& triangle : plate.getComposition()) {
      drawTriangle(triangle, aspect_ratio, camera);
    }
  }
  glEnd();
}

void drawTriangleMeshes(const vector<vector<Triangle>>& meshes, float aspect_ratio, const Camera& camera)
{
  for (const vector<Triangle>& mesh : meshes) {
    drawTriangles(mesh, aspect_ratio, camera);
  }
}
} // namespace

int main()
{
#if defined(__linux__)
  glfwInitHint(GLFW_WAYLAND_LIBDECOR, GLFW_WAYLAND_DISABLE_LIBDECOR);
#endif

  const vector<vector<Triangle>> cubes{
      makeCube({0.0F, 0.0F, 4.0F}, {1.0F, 0.2F, 0.2F}),
      makeCube({1.0F, 0.0F, 6.0F}, {0.2F, 0.8F, 0.3F}),
      makeCube({0.0F, 1.0F, 7.0F}, {0.2F, 0.4F, 1.0F}),
  };
  const vector<Plate> plates = makePlates();

  if (glfwInit() == GLFW_FALSE) {
    cerr << "Failed to initialize GLFW\n";
    return 1;
  }

  glfwWindowHint(GLFW_DEPTH_BITS, 24);
  GLFWwindow* window = glfwCreateWindow(800, 600, "Game Window - WASD move, mouse look, Escape releases mouse", nullptr, nullptr);
  if (window == nullptr) {
    cerr << "Failed to create GLFW window\n";
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(window);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  Camera camera{{0.0F, 1.5F, 0.0F}};
  MouseLookState mouse_look{&camera};
  glfwSetWindowUserPointer(window, &mouse_look);
  glfwSetCursorPosCallback(window, updateMouseLook);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  double previous_frame_time = glfwGetTime();
  while (!glfwWindowShouldClose(window)) {
    if (mouse_look.captured && glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      mouse_look.captured = false;
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    const double current_frame_time = glfwGetTime();
    const float delta_time = static_cast<float>(current_frame_time - previous_frame_time);
    previous_frame_time = current_frame_time;
    updateCamera(window, camera, delta_time);

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
      drawPlates(plates, aspect_ratio, camera);
      drawTriangleMeshes(cubes, aspect_ratio, camera);
    }

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
