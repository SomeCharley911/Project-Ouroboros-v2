
#include "Triangle.h"
#include <GLFW/glfw3.h>
#include <array>
#include <iostream>

using namespace std;

int main()
{
#if defined(__linux__)
  glfwInitHint(GLFW_WAYLAND_LIBDECOR, GLFW_WAYLAND_DISABLE_LIBDECOR);
#endif

  const Triangle triangle(std::array<std::array<float, 2>, 3>{{{0.0F, 0.65F}, {-0.6F, -0.5F}, {0.6F, -0.5F}}}, std::array<float, 3>{1.0F, 0.2F, 0.2F});
  const auto vertices = triangle.getVertices();
  const auto color = triangle.getColor();

  if (glfwInit() == GLFW_FALSE) {
    cerr << "Failed to initialize GLFW\n";
    return 1;
  }

  GLFWwindow* window = glfwCreateWindow(800, 600, "Game Window", nullptr, nullptr);
  if (window == nullptr) {
    cerr << "Failed to create GLFW window\n";
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(window);

  while (!glfwWindowShouldClose(window)) {
    int framebuffer_width = 0;
    int framebuffer_height = 0;
    glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);
    glViewport(0, 0, framebuffer_width, framebuffer_height);

    glClearColor(0.08F, 0.08F, 0.1F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
    glColor3f(color[0], color[1], color[2]);
    for (const auto& vertex : vertices) {
      glVertex2f(vertex[0], vertex[1]);
    }
    glEnd();

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
