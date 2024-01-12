#pragma once

#include <webgpu/webgpu.hpp>
#include "renderer.hpp"

class Program {
 public:
  Program(Renderer& renderer, wgpu::ShaderModule shader);
  Program(const Program&) = delete;
  Program& operator=(const Program&) = delete;
  ~Program();

  void bind(wgpu::RenderPassEncoder renderPass) const;

 private:
  wgpu::RenderPipeline m_renderPipeline{nullptr};
};
