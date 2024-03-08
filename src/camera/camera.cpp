#include "camera/camera.hpp"
#include <webgpu/webgpu.hpp>
#include "renderer/renderer.hpp"

Camera::Camera(Renderer& renderer) : m_uniform{renderer, wgpu::ShaderStage::Vertex} {}

void Camera::onCursorPos(double dx, double dy) { m_controller.onCursorPos(dx, dy); }

void Camera::onKey(int key, int action) { m_controller.onKey(key, action); }

void Camera::update(Renderer& renderer, float dt) {
  if (renderer.isResized) {
    m_projection.m_aspect = static_cast<float>(renderer.config.width) / renderer.config.height;
  }

  if (m_controller.applyUpdates(m_view, dt) || renderer.isResized) {
    m_uniform.set(renderer, {m_projection.matrix() * m_view.matrix(), m_view.origin()});
  }
}

wgpu::BindGroupLayout Camera::bindGroupLayout() const { return m_uniform.bindGroupLayout(); }

wgpu::BindGroup Camera::bindGroup() const { return m_uniform.bindGroup(); }
