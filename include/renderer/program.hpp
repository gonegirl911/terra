#pragma once

#include <webgpu/webgpu.h>
#include <concepts>
#include <cstddef>
#include <initializer_list>
#include <webgpu/webgpu.hpp>
#include "renderer.hpp"

class RenderProgram {
 public:
  RenderProgram(Renderer& renderer, wgpu::ShaderModule shader,
                std::initializer_list<wgpu::VertexBufferLayout> buffers,
                std::initializer_list<WGPUBindGroupLayout> bindGroupLayouts);
  RenderProgram(const RenderProgram&) = delete;
  RenderProgram& operator=(const RenderProgram&) = delete;
  ~RenderProgram();

  void bind(wgpu::RenderPassEncoder renderPass,
            std::same_as<wgpu::BindGroup> auto... bindGroups) const {
    renderPass.setPipeline(m_renderPipeline);
    std::size_t i{};
    (renderPass.setBindGroup(i++, bindGroups, 0, nullptr), ...);
  }

 private:
  wgpu::RenderPipeline m_renderPipeline{nullptr};
};

class ComputeProgram {
 public:
  ComputeProgram(Renderer& renderer, wgpu::ShaderModule shader,
                 std::initializer_list<WGPUBindGroupLayout> bindGroupLayouts);
  ComputeProgram(const ComputeProgram&) = delete;
  ComputeProgram& operator=(const ComputeProgram&) = delete;
  ~ComputeProgram();

  void bind(wgpu::ComputePassEncoder computePass,
            std::same_as<wgpu::BindGroup> auto... bindGroups) const {
    computePass.setPipeline(m_computePipeline);
    std::size_t i{};
    (computePass.setBindGroup(i++, bindGroups, 0, nullptr), ...);
  }

 private:
  wgpu::ComputePipeline m_computePipeline{nullptr};
};
