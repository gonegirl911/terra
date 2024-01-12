#include "triangle.hpp"
#include <webgpu/webgpu.hpp>
#include "renderer/renderer.hpp"
#include "renderer/shader.hpp"

Triangle::Triangle(Renderer& renderer) : m_program{renderer, Shader{renderer}} {}

void Triangle::draw(wgpu::TextureView view, wgpu::CommandEncoder encoder) {
  wgpu::RenderPassColorAttachment renderPassColorAttachment{wgpu::Default};
  renderPassColorAttachment.view = view;
  renderPassColorAttachment.loadOp = wgpu::LoadOp::Clear;
  renderPassColorAttachment.clearValue = wgpu::Color{0.9, 0.1, 0.2, 1.0};
  renderPassColorAttachment.storeOp = wgpu::StoreOp::Store;

  wgpu::RenderPassDescriptor renderPassDesc{wgpu::Default};
  renderPassDesc.colorAttachmentCount = 1;
  renderPassDesc.colorAttachments = &renderPassColorAttachment;

  auto renderPass = encoder.beginRenderPass(renderPassDesc);
  m_program.bind(renderPass);
  renderPass.draw(3, 1, 0, 0);
  renderPass.end();

  renderPass.release();
}
