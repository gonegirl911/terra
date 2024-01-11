#pragma once

#include <webgpu/webgpu.hpp>

class Renderer {
 public:
  Renderer();
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;
  ~Renderer();

 private:
  wgpu::Instance m_instance{nullptr};
};
