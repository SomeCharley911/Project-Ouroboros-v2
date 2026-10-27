
#include "glfw3.h"
#include <iostream>

using namespace std;

int main()
{
  if (glfwInit() == GLFW_FALSE) {
    cerr << "Failed to initialize GLFW\n";
    return 1;
  }

  GLFWwindow* window =
      glfwCreateWindow(800, 600, "GLFW Window", nullptr, nullptr);
  if (window == nullptr) {
    cerr << "Failed to create GLFW window\n";
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(window);

  while (glfwWindowShouldClose(window) == GLFW_FALSE) {
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
