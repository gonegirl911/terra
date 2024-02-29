#include "terrain.hpp"
#include <array>
#include <webgpu/webgpu.hpp>
#include "glm/ext/vector_float3.hpp"
#include "renderer/layout.hpp"
#include "renderer/renderer.hpp"
#include "renderer/shader.hpp"

Terrain::Terrain(Renderer& renderer, wgpu::BindGroupLayout cameraBindGroupLayout)
    : m_vertices{renderer, 6},
      m_generator{renderer,
                  Shader{renderer, "../assets/shaders/generator.wgsl"},
                  {m_vertices.bindGroupLayout()}},
      m_program{renderer,
                Shader{renderer, "../assets/shaders/terrain.wgsl"},
                {TerrainVertex::DESC},
                {cameraBindGroupLayout}} {}

void Terrain::draw(wgpu::TextureView view, wgpu::CommandEncoder encoder,
                   wgpu::BindGroup cameraBindGroup) {
  if (m_shouldGenerate) {
    generate(encoder);
    m_shouldGenerate = false;
  }

  wgpu::RenderPassColorAttachment renderPassColorAttachment{wgpu::Default};
  renderPassColorAttachment.view = view;
  renderPassColorAttachment.loadOp = wgpu::LoadOp::Clear;
  renderPassColorAttachment.storeOp = wgpu::StoreOp::Store;

  wgpu::RenderPassDescriptor renderPassDesc{wgpu::Default};
  renderPassDesc.colorAttachmentCount = 1;
  renderPassDesc.colorAttachments = &renderPassColorAttachment;

  auto renderPass = encoder.beginRenderPass(renderPassDesc);
  m_program.bind(renderPass, cameraBindGroup);
  m_vertices.draw(renderPass);
  renderPass.end();

  renderPass.release();
}

void Terrain::generate(wgpu::CommandEncoder encoder) const {
  auto computePass = encoder.beginComputePass(wgpu::Default);
  m_generator.bind(computePass, m_vertices.bindGroup());
  computePass.dispatchWorkgroups(1, 1, 1);
  computePass.end();
  computePass.release();
}
