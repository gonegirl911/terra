#include "triangle.hpp"
#include <array>
#include <webgpu/webgpu.hpp>
#include "glm/ext/vector_float3.hpp"
#include "renderer/layout.hpp"
#include "renderer/renderer.hpp"
#include "renderer/shader.hpp"

constexpr std::array<TriangleVertex, 6> VERTICES{{
  {{-0.5, 0.0, -0.5}, {0.0, 1.0, 0.0}},
  {{0.5, 0.0, -0.5}, {0.0, 1.0, 0.0}},
  {{-0.5, 0.0, 0.5}, {0.0, 1.0, 0.0}},
  {{0.5, 0.0, -0.5}, {0.0, 1.0, 0.0}},
  {{0.5, 0.0, 0.5}, {0.0, 1.0, 0.0}},
  {{-0.5, 0.0, 0.5}, {0.0, 1.0, 0.0}},
}};

Triangle::Triangle(Renderer& renderer, wgpu::BindGroupLayout cameraBindGroupLayout)
    : m_program{renderer,
                Shader{renderer, "../assets/shaders/triangle.wgsl"},
                {TriangleVertex::DESC},
                {cameraBindGroupLayout}},
      m_vertices{renderer, VERTICES} {}

void Triangle::draw(wgpu::TextureView view, wgpu::CommandEncoder encoder,
                    wgpu::BindGroup cameraBindGroup) {
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
