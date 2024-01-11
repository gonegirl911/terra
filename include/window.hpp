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

  void onResize(GLFWframebuffersizefun f) const;

  void run(std::invocable auto f) const {
    while (!glfwWindowShouldClose(m_window)) {
      glfwPollEvents();
      f();
    }
  }

  template <typename T>
  static T& getUserData(GLFWwindow* window) {
    return *(T*)glfwGetWindowUserPointer(window);
  }

 private:
  GLFWwindow* m_window;
};
