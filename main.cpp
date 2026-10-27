
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

int main()
{
#if defined(__linux__)
  glfwInitHint(GLFW_WAYLAND_LIBDECOR, GLFW_WAYLAND_DISABLE_LIBDECOR);
#endif

  if (glfwInit() == GLFW_FALSE) {
    cerr << "Failed to initialize GLFW\n";
    return 1;
  }

  GLFWwindow* window =
      glfwCreateWindow(800, 600, "Game Window", nullptr, nullptr);
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
    glColor3f(1.0F, 0.2F, 0.2F);
    glVertex2f(0.0F, 0.65F);
    glColor3f(0.2F, 0.9F, 0.4F);
    glVertex2f(-0.6F, -0.5F);
    glColor3f(0.2F, 0.5F, 1.0F);
    glVertex2f(0.6F, -0.5F);
    glEnd();

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
