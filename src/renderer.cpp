#include "renderer.hpp"
#include <stdexcept>
#include <webgpu/webgpu.hpp>
#include "glfw3webgpu.h"

Renderer::Renderer(GLFWwindow* window) {
  m_instance = wgpu::createInstance(wgpu::Default);
  if (!m_instance) {
    throw std::runtime_error{"Could not initialize WebGPU"};
  }

  m_surface = glfwGetWGPUSurface(m_instance, window);
  if (!m_surface) {
    throw std::runtime_error{"Could not get surface"};
  }

  Adapter adapter{m_instance, m_surface};
  m_device = adapter.requestDevice(wgpu::Default);
  if (!m_device) {
    throw std::runtime_error{"Device not available"};
  }
  m_device.setUncapturedErrorCallback([](auto type, auto message) {
    std::cout << "Uncaptured device error of type " << type << ":\n" << message << '\n';
  });

  m_queue = m_device.getQueue();
}

Renderer::~Renderer() {
  m_instance.release();
  m_surface.release();
  m_device.release();
  m_queue.release();
}

Renderer::Adapter::Adapter(wgpu::Instance instance, wgpu::Surface surface) {
  wgpu::RequestAdapterOptions requestAdapterOpts{wgpu::Default};
  requestAdapterOpts.powerPreference = wgpu::PowerPreference::HighPerformance;
  requestAdapterOpts.compatibleSurface = surface;
  m_adapter = instance.requestAdapter(requestAdapterOpts);
  if (!m_adapter) {
    throw std::runtime_error{"Adapter not available"};
  }
}

Renderer::Adapter::~Adapter() { m_adapter.release(); }

wgpu::Device Renderer::Adapter::requestDevice(const wgpu::DeviceDescriptor& desc) {
  return m_adapter.requestDevice(desc);
}
