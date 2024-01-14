#pragma once

#include <webgpu/webgpu.h>
#include <concepts>
#include <cstddef>
#include <initializer_list>
#include <webgpu/webgpu.hpp>
#include "renderer.hpp"

class Program {
 public:
  Program(Renderer& renderer, wgpu::ShaderModule shader,
          std::initializer_list<wgpu::VertexBufferLayout> buffers,
          std::initializer_list<WGPUBindGroupLayout> bindGroupLayouts);
  Program(const Program&) = delete;
  Program& operator=(const Program&) = delete;
  ~Program();

  void bind(wgpu::RenderPassEncoder renderPass,
            std::same_as<wgpu::BindGroup> auto... bindGroups) const {
    renderPass.setPipeline(m_renderPipeline);
    std::size_t i{};
    (renderPass.setBindGroup(i++, bindGroups, 0, nullptr), ...);
  }

 private:
  wgpu::RenderPipeline m_renderPipeline{nullptr};
};
