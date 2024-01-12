#pragma once

#include "renderer/renderer.hpp"
#include "triangle.hpp"
#include "window.hpp"

class Application {
 public:
  Application();
  Application(const Application&) = delete;
  Application& operator=(const Application&) = delete;

  void run();

 private:
  Window m_window;
  Renderer m_renderer;
  Triangle m_triangle;

  void update();
  void draw();
};
