#pragma once

#include <concepts>
#include <utility>
#include "GLFW/glfw3.h"

class Window {
 public:
  Window();
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
  ~Window();

  operator GLFWwindow*() const;

  std::pair<int, int> size() const;

  void setUserData(auto& data) const { glfwSetWindowUserPointer(m_window, &data); }

  void setResizeCallback(GLFWframebuffersizefun f) const;

  void setMouseButtonCallback(GLFWmousebuttonfun f) const;

  void setKeyCallback(GLFWkeyfun f) const;

  void setCursorPosCallback(GLFWcursorposfun f) const;

  void onMouseButton(int button, int action) const;

  void onKey(int key, int action);

  std::pair<double, double> onCursorPos(double xpos, double ypos);

  void run(std::invocable<float> auto f) const {
    auto prev = glfwGetTime();
    while (glfwPollEvents(), !glfwWindowShouldClose(m_window)) {
      const auto now = glfwGetTime();
      f(now - prev);
      prev = now;
    }
  }

  template <typename T>
  static T& getUserData(GLFWwindow* window) {
    return *(T*)glfwGetWindowUserPointer(window);
  }

 private:
  GLFWwindow* m_window;
  double m_xpos;
  double m_ypos;
};
