#include "triangle.hpp"
#include <array>
#include <webgpu/webgpu.hpp>
#include "glm/vec2.hpp"
#include "renderer/layout.hpp"
#include "renderer/renderer.hpp"
#include "renderer/shader.hpp"

constexpr std::array<glm::vec2, 4> VERTICES{{
  {-0.5f, -0.5f},
  {0.5f, -0.5f},
  {-0.5f, 0.5f},
  {0.5f, 0.5f},
}};

Triangle::Triangle(Renderer& renderer, wgpu::BindGroupLayout cameraBindGroupLayout)
    : m_program{renderer, Shader{renderer}, {LAYOUT<glm::vec2>}, {cameraBindGroupLayout}},
      m_vertices{renderer, VERTICES} {}

void Triangle::draw(wgpu::TextureView view, wgpu::CommandEncoder encoder,
                    wgpu::BindGroup cameraBindGroup) {
  wgpu::RenderPassColorAttachment renderPassColorAttachment{wgpu::Default};
  renderPassColorAttachment.view = view;
  renderPassColorAttachment.loadOp = wgpu::LoadOp::Clear;
  renderPassColorAttachment.clearValue = wgpu::Color{0.9, 0.1, 0.2, 1.0};
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
