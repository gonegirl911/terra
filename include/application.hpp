#pragma once

#include "camera/camera.hpp"
#include "renderer/renderer.hpp"
#include "terrain.hpp"
#include "window.hpp"

class Application {
 public:
  Application();

  void run();

 private:
  Window m_window;
  Renderer m_renderer;
  Camera m_camera;
  Terrain m_terrain;

  void update(float dt);

  void draw();
};
