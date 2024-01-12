#pragma once

#include "renderer/renderer.hpp"
#include "triangle.hpp"
#include "window.hpp"

class Application {
 public:
  Application();

  void run();

 private:
  Window m_window;
  Renderer m_renderer;
  Triangle m_triangle;

  void update();
  void draw();
};
