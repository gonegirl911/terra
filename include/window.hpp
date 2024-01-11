#pragma once

#include "GLFW/glfw3.h"

class Window {
 public:
  Window();
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
  ~Window();

  void run() const;

 private:
  GLFWwindow* m_window;
};
