#include "window.hpp"
#include <stdexcept>
#include "GLFW/glfw3.h"

Window::Window() {
  if (!glfwInit()) {
    throw std::runtime_error{"Could not initialize GLFW"};
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  m_window = glfwCreateWindow(640, 480, "Terra", nullptr, nullptr);
  if (!m_window) {
    throw std::runtime_error{"Could not create window"};
  }
}
