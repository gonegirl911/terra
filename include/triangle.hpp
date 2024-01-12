#pragma once

#include <webgpu/webgpu.hpp>
#include "renderer/program.hpp"
#include "renderer/renderer.hpp"

class Triangle {
 public:
  Triangle(Renderer& renderer);
  Triangle(const Triangle&) = delete;
  Triangle& operator=(const Triangle&) = delete;

  void draw(wgpu::TextureView view, wgpu::CommandEncoder encoder);

 private:
  Program m_program;
};
