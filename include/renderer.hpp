#pragma once

#include <webgpu/webgpu.hpp>
#include "GLFW/glfw3.h"

class Renderer {
 public:
  explicit Renderer(GLFWwindow* window);
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;

 private:
  wgpu::Instance m_instance{nullptr};
  wgpu::Surface m_surface{nullptr};
  wgpu::Device m_device{nullptr};
  wgpu::Queue m_queue{nullptr};
};
