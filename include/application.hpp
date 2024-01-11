#pragma once

#include "renderer/renderer.hpp"
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
};
