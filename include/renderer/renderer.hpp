#pragma once

#include <webgpu/webgpu.hpp>
#include "GLFW/glfw3.h"
#include "window.hpp"

class Renderer {
 public:
  wgpu::Instance instance;
  wgpu::Surface surface{nullptr};
  wgpu::Device device{nullptr};
  wgpu::Queue queue{nullptr};
  wgpu::SurfaceConfiguration config;
  bool shouldResize = true;
  bool isResized = false;

  explicit Renderer(const Window& window);
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;
  ~Renderer();

  void recreateSurface();

  void update(const Window& window);

 private:
  wgpu::SurfaceConfiguration getDefaultConfig(const Window& window, wgpu::Adapter adapter);

  bool resize(int width, int height);

  wgpu::SurfaceCapabilities getCapabilities(wgpu::Adapter adapter);
};
