#include <GLFW/glfw3.h>
#include <iostream>

#include "Camera.h"
#include "Plate.h"
#include "World.h"

using namespace std;

namespace {
// Projection range and camera controls, expressed in world units and radians.
constexpr float near_plane = 0.1F;
constexpr float far_plane = 100.0F;
constexpr float focal_length = 1.7320508F;
constexpr float rotation_speed = 1.5F;
constexpr float mouse_sensitivity = 0.0025F;

// GLFW callback state keeps mouse history alongside the camera it controls.
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

  state->camera->rotate(static_cast<float>(x_offset) * mouse_sensitivity, static_cast<float>(y_offset) * mouse_sensitivity);
}

bool projectPoint(const array<float, 3>& point, float aspect_ratio, array<float, 3>& projected)
{
  const float depth = point[2];
  if (depth <= near_plane) {
    return false;
  }

  // Perspective projection maps camera-space coordinates into normalized device space.
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

  // Arrow keys turn the camera independently of its movement.
  const float turn = rotation_speed * delta_time;
  if (pressed(GLFW_KEY_LEFT)) {
    camera.rotate(-turn, 0.0F);
  }
  if (pressed(GLFW_KEY_RIGHT)) {
    camera.rotate(turn, 0.0F);
  }
  if (pressed(GLFW_KEY_UP)) {
    camera.rotate(0.0F, turn);
  }
  if (pressed(GLFW_KEY_DOWN)) {
    camera.rotate(0.0F, -turn);
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

  camera.move(forward, strafe, vertical, delta_time);
}

void drawTriangle(const Triangle& triangle, float aspect_ratio, const Camera& camera)
{
  const auto vertices = triangle.getPosition();
  array<array<float, 3>, 3> screen_vertices{};
  bool visible = true;

  // Transform and project every vertex; skip the triangle if any vertex is behind the near plane.
  for (size_t i = 0; i < vertices.size(); ++i) {
    const auto position = vertices[i]->getPosition();
    const auto camera_position = camera.toCameraSpace(position);
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
  // OpenGL is in triangle mode, so submit the projected vertices in order.
  for (const auto& vertex : screen_vertices) {
    glVertex3f(vertex[0], vertex[1], vertex[2]);
  }
}

void drawTriangles(const vector<Triangle>& triangles, float aspect_ratio, const Camera& camera)
{
  // Submit a vector mesh as one batch of immediate-mode triangles.
  glBegin(GL_TRIANGLES);
  for (const Triangle& triangle : triangles) {
    drawTriangle(triangle, aspect_ratio, camera);
  }
  glEnd();
}

void drawPlates(const vector<Plate>& plates, float aspect_ratio, const Camera& camera)
{
  // Traverse each plate's stored triangles directly.
  glBegin(GL_TRIANGLES);
  for (const Plate& plate : plates) {
    for (const Triangle& triangle : plate.getComposition()) {
      drawTriangle(triangle, aspect_ratio, camera);
    }
  }
  glEnd();
}

} // namespace

int main()
{
#if defined(__linux__)
  // Avoid the optional Wayland client-side decorations dependency where available.
  glfwInitHint(GLFW_WAYLAND_LIBDECOR, GLFW_WAYLAND_DISABLE_LIBDECOR);
#endif

  World world;
  if (!world.loadFromFile("TheWorld.txt")) {
    return 1;
  }

  // Initialize GLFW and create the window/context used by OpenGL.
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

  // Start above the ground, facing along the positive Z direction.
  Camera camera({0.0F, 1.5F, 0.0F});
  MouseLookState mouse_look{&camera};
  glfwSetWindowUserPointer(window, &mouse_look);
  glfwSetCursorPosCallback(window, updateMouseLook);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  double previous_frame_time = glfwGetTime();
  while (!glfwWindowShouldClose(window)) {
    // Escape releases the captured cursor so the user can interact with the window.
    if (mouse_look.captured && glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      mouse_look.captured = false;
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    const double current_frame_time = glfwGetTime();
    const float delta_time = static_cast<float>(current_frame_time - previous_frame_time);
    previous_frame_time = current_frame_time;
    updateCamera(window, camera, delta_time);

    // Use framebuffer dimensions (which may differ from window dimensions on HiDPI displays).
    int framebuffer_width = 0;
    int framebuffer_height = 0;
    glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);
    if (framebuffer_width > 0 && framebuffer_height > 0) {
      glViewport(0, 0, framebuffer_width, framebuffer_height);
    }

    glClearColor(0.08F, 0.08F, 0.1F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Skip projection and drawing while the framebuffer has no drawable area.
    if (framebuffer_width > 0 && framebuffer_height > 0) {
      const float aspect_ratio = static_cast<float>(framebuffer_width) / static_cast<float>(framebuffer_height);
      drawPlates(world.m_plates, aspect_ratio, camera);
      drawTriangles(world.m_unplated_triangles, aspect_ratio, camera);
    }

    glfwSwapBuffers(window);

    // Process input and window events after presenting the rendered frame.
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
