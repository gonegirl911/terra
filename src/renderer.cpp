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

  wgpu::RequestAdapterOptions requestAdapterOpts{wgpu::Default};
  requestAdapterOpts.powerPreference = wgpu::PowerPreference::HighPerformance;
  requestAdapterOpts.compatibleSurface = m_surface;
  auto adapter = m_instance.requestAdapter(requestAdapterOpts);
  if (!adapter) {
    throw std::runtime_error{"Adapter not available"};
  }
  m_device = adapter.requestDevice(wgpu::Default);
  if (!m_device) {
    throw std::runtime_error{"Device not available"};
  }
  m_device.setUncapturedErrorCallback([](auto type, auto message) {
    std::cout << "Uncaptured device error of type " << type << ":\n" << message << '\n';
  });
  adapter.release();

  m_queue = m_device.getQueue();
}
