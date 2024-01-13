#include "renderer/program.hpp"
#include <webgpu/webgpu.h>
#include <initializer_list>
#include <webgpu/webgpu.hpp>
#include "renderer/renderer.hpp"

Program::Program(Renderer& renderer, wgpu::ShaderModule shader,
                 std::initializer_list<wgpu::VertexBufferLayout> buffers,
                 std::initializer_list<WGPUBindGroupLayout> bindGroupLayouts) {
  wgpu::PipelineLayoutDescriptor pipelineLayoutDesc{wgpu::Default};
  pipelineLayoutDesc.bindGroupLayoutCount = bindGroupLayouts.size();
  pipelineLayoutDesc.bindGroupLayouts = bindGroupLayouts.begin();

  auto pipelineLayout = renderer.device.createPipelineLayout(pipelineLayoutDesc);

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
  renderPipelineDesc.primitive.topology = wgpu::PrimitiveTopology::TriangleStrip;
  renderPipelineDesc.primitive.cullMode = wgpu::CullMode::Back;
  renderPipelineDesc.fragment = &fragmentState;

  m_renderPipeline = renderer.device.createRenderPipeline(renderPipelineDesc);

  pipelineLayout.release();
}

Program::~Program() { m_renderPipeline.release(); }
