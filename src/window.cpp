#include "window.hpp"
#include <stdexcept>
#include <utility>
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

  if (glfwRawMouseMotionSupported()) {
    glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
  }

  glfwGetCursorPos(m_window, &m_xpos, &m_ypos);
}

Window::~Window() { glfwTerminate(); }

Window::operator GLFWwindow*() const { return m_window; }

std::pair<int, int> Window::size() const {
  int width, height;
  glfwGetFramebufferSize(m_window, &width, &height);
  return {width, height};
}

void Window::setResizeCallback(GLFWframebuffersizefun f) const {
  glfwSetFramebufferSizeCallback(m_window, f);
}

void Window::setMouseButtonCallback(GLFWmousebuttonfun f) const {
  glfwSetMouseButtonCallback(m_window, f);
}

void Window::setCursorPosCallback(GLFWcursorposfun f) const {
  glfwSetCursorPosCallback(m_window, f);
}

void Window::setKeyCallback(GLFWkeyfun f) const { glfwSetKeyCallback(m_window, f); }

void Window::onMouseButton(int button, int action) const {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }
}

void Window::onKey(int key, int action) const {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }
}

std::pair<double, double> Window::onCursorPos(double xpos, double ypos) {
  const auto dx = xpos - m_xpos;
  const auto dy = ypos - m_ypos;
  m_xpos = xpos;
  m_ypos = ypos;
  return {dx, dy};
}
