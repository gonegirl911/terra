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

  void onMouseButton(int button, int action);

  void onKey(int key, int action);

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
