#include "renderer/program.hpp"
#include <initializer_list>
#include <webgpu/webgpu.hpp>
#include "renderer/renderer.hpp"

Program::Program(Renderer& renderer, wgpu::ShaderModule shader,
                 std::initializer_list<wgpu::VertexBufferLayout> buffers) {
  wgpu::ColorTargetState colorTargetState{wgpu::Default};
  colorTargetState.format = renderer.config.format;
  colorTargetState.writeMask = wgpu::ColorWriteMask::All;

  wgpu::FragmentState fragmentState{wgpu::Default};
  fragmentState.module = shader;
  fragmentState.entryPoint = "fs_main";
  fragmentState.targetCount = 1;
  fragmentState.targets = &colorTargetState;

  wgpu::RenderPipelineDescriptor renderPipelineDesc{wgpu::Default};
  renderPipelineDesc.vertex.module = shader;
  renderPipelineDesc.vertex.entryPoint = "vs_main";
  renderPipelineDesc.vertex.bufferCount = buffers.size();
  renderPipelineDesc.vertex.buffers = buffers.begin();
  renderPipelineDesc.fragment = &fragmentState;
  renderPipelineDesc.primitive.cullMode = wgpu::CullMode::Back;

  m_renderPipeline = renderer.device.createRenderPipeline(renderPipelineDesc);
}

Program::~Program() { m_renderPipeline.release(); }
