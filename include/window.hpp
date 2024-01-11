#pragma once

#include <concepts>
#include "GLFW/glfw3.h"

class Window {
 public:
  Window();
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
  ~Window();

  operator GLFWwindow*() const { return m_window; }

  void run(std::invocable auto f) const {
    while (!glfwWindowShouldClose(m_window)) {
      glfwPollEvents();
      f();
    }
  }

 private:
  GLFWwindow* m_window;
};
