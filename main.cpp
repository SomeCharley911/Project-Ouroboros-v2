#include <GLFW/glfw3.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <vector>

#include "Headers/Cube.h"
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
constexpr int floor_half_extent = 20;
constexpr float floor_elevation = -0.5F;

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

void drawCube(const Cube& cube, float aspect_ratio, const Camera& camera)
{
  const auto position = cube.getPosition();
  const auto color = cube.getColor();
  array<array<float, 3>, cube_offsets.size()> camera_vertices{};
  array<array<float, 3>, cube_offsets.size()> screen_vertices{};
  array<bool, cube_offsets.size()> visible{};

  for (size_t i = 0; i < cube_offsets.size(); ++i) {
    const array<float, 3> world_vertex{
        static_cast<float>(position[0]) + cube_offsets[i][0],
        static_cast<float>(position[1]) + cube_offsets[i][1],
        static_cast<float>(position[2]) + cube_offsets[i][2],
    };
    camera_vertices[i] = toCameraSpace(world_vertex, camera);
    visible[i] = projectPoint(camera_vertices[i], aspect_ratio, screen_vertices[i]);
  }

  glColor3f(color[0], color[1], color[2]);
  glBegin(GL_TRIANGLES);
  for (const auto& triangle : cube_triangles) {
    if (!visible[triangle[0]] || !visible[triangle[1]] || !visible[triangle[2]]) {
      continue;
    }
    for (const size_t vertex : triangle) {
      glVertex3f(screen_vertices[vertex][0], screen_vertices[vertex][1], screen_vertices[vertex][2]);
    }
  }
  glEnd();
}

vector<Triangle> makeFloor()
{
  vector<Triangle> floor;
  floor.reserve(static_cast<size_t>(floor_half_extent * floor_half_extent * 8));

  for (int x = -floor_half_extent; x < floor_half_extent; ++x) {
    for (int z = -floor_half_extent; z < floor_half_extent; ++z) {
      const float left = static_cast<float>(x);
      const float right = static_cast<float>(x + 1);
      const float near_edge = static_cast<float>(z);
      const float far_edge = static_cast<float>(z + 1);
      const Point near_left({left, floor_elevation, near_edge});
      const Point near_right({right, floor_elevation, near_edge});
      const Point far_left({left, floor_elevation, far_edge});
      const Point far_right({right, floor_elevation, far_edge});
      const array<float, 3> color = (x + z) % 2 == 0 ? array<float, 3>{0.24F, 0.27F, 0.30F} : array<float, 3>{0.30F, 0.33F, 0.36F};

      floor.emplace_back(array<Point, 3>{near_left, near_right, far_right}, color);
      floor.emplace_back(array<Point, 3>{near_left, far_right, far_left}, color);
    }
  }

  return floor;
}

void drawFloor(const vector<Triangle>& floor, float aspect_ratio, const Camera& camera)
{
  glBegin(GL_TRIANGLES);
  for (const Triangle& triangle : floor) {
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
      continue;
    }

    const auto color = triangle.getColor();
    glColor3f(color[0], color[1], color[2]);
    for (const auto& vertex : screen_vertices) {
      glVertex3f(vertex[0], vertex[1], vertex[2]);
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

  const array<Cube, 3> cubes{
      Cube({0, 0, 4}, {1.0F, 0.2F, 0.2F}),
      Cube({1, 0, 6}, {0.2F, 0.8F, 0.3F}),
      Cube({0, 1, 7}, {0.2F, 0.4F, 1.0F}),
  };
  const vector<Triangle> floor = makeFloor();

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
      drawFloor(floor, aspect_ratio, camera);
      for (const Cube& cube : cubes) {
        drawCube(cube, aspect_ratio, camera);
      }
    }

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
