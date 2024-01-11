#include "renderer.hpp"
#include <stdexcept>
#include <webgpu/webgpu.hpp>

Renderer::Renderer() {
  wgpu::InstanceDescriptor desc{};
  desc.nextInChain = nullptr;
  m_instance = wgpu::createInstance(desc);
  if (!m_instance) {
    throw std::runtime_error{"Could not initialize WebGPU"};
  }
}

Renderer::~Renderer() { m_instance.release(); }
