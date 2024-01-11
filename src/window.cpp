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

Window::~Window() { glfwTerminate(); }

Window::operator GLFWwindow*() const { return m_window; }

std::pair<int, int> Window::size() const {
  int width, height;
  glfwGetFramebufferSize(m_window, &width, &height);
  return {width, height};
}

void Window::onResize(GLFWframebuffersizefun f) const {
  glfwSetFramebufferSizeCallback(m_window, f);
}
