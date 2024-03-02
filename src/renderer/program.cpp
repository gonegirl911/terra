#include "renderer/program.hpp"
#include <webgpu/webgpu.h>
#include <initializer_list>
#include <webgpu/webgpu.hpp>
#include "renderer/renderer.hpp"
#include "renderer/texture.hpp"

ComputeProgram::ComputeProgram(Renderer& renderer, wgpu::ShaderModule shader,
                               std::initializer_list<WGPUBindGroupLayout> bindGroupLayouts) {
  wgpu::PipelineLayoutDescriptor pipelineLayoutDesc{wgpu::Default};
  pipelineLayoutDesc.bindGroupLayoutCount = bindGroupLayouts.size();
  pipelineLayoutDesc.bindGroupLayouts = bindGroupLayouts.begin();

  auto pipelineLayout = renderer.device.createPipelineLayout(pipelineLayoutDesc);

  wgpu::ComputePipelineDescriptor computePipelineDesc{wgpu::Default};
  computePipelineDesc.layout = pipelineLayout;
  computePipelineDesc.compute.module = shader;
  computePipelineDesc.compute.entryPoint = "main";

  m_computePipeline = renderer.device.createComputePipeline(computePipelineDesc);

  pipelineLayout.release();
}

ComputeProgram::~ComputeProgram() { m_computePipeline.release(); }

RenderProgram::RenderProgram(Renderer& renderer, wgpu::ShaderModule shader,
                             std::initializer_list<wgpu::VertexBufferLayout> buffers,
                             std::initializer_list<WGPUBindGroupLayout> bindGroupLayouts) {
  wgpu::PipelineLayoutDescriptor pipelineLayoutDesc{wgpu::Default};
  pipelineLayoutDesc.bindGroupLayoutCount = bindGroupLayouts.size();
  pipelineLayoutDesc.bindGroupLayouts = bindGroupLayouts.begin();

  auto pipelineLayout = renderer.device.createPipelineLayout(pipelineLayoutDesc);

  wgpu::DepthStencilState depthStencilState{wgpu::Default};
  depthStencilState.format = DepthBuffer::FORMAT;
  depthStencilState.depthWriteEnabled = true;
  depthStencilState.depthCompare = wgpu::CompareFunction::Less;

  wgpu::ColorTargetState colorTargetState{wgpu::Default};
  colorTargetState.format = renderer.config.format;
  colorTargetState.writeMask = wgpu::ColorWriteMask::All;

  wgpu::FragmentState fragmentState{wgpu::Default};
  fragmentState.module = shader;
  fragmentState.entryPoint = "fs_main";
  fragmentState.targetCount = 1;
  fragmentState.targets = &colorTargetState;

  wgpu::RenderPipelineDescriptor renderPipelineDesc{wgpu::Default};
  renderPipelineDesc.layout = pipelineLayout;
  renderPipelineDesc.vertex.module = shader;
  renderPipelineDesc.vertex.entryPoint = "vs_main";
  renderPipelineDesc.vertex.bufferCount = buffers.size();
  renderPipelineDesc.vertex.buffers = buffers.begin();
  renderPipelineDesc.primitive.cullMode = wgpu::CullMode::Back;
  renderPipelineDesc.depthStencil = &depthStencilState;
  renderPipelineDesc.fragment = &fragmentState;

  m_renderPipeline = renderer.device.createRenderPipeline(renderPipelineDesc);

  pipelineLayout.release();
}

RenderProgram::~RenderProgram() { m_renderPipeline.release(); }
