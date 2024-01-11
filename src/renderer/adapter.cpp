#include "renderer/adapter.hpp"
#include <webgpu/webgpu.hpp>

Adapter::Adapter(wgpu::Instance instance, wgpu::Surface surface) {
  wgpu::RequestAdapterOptions requestAdapterOpts{wgpu::Default};
  requestAdapterOpts.powerPreference = wgpu::PowerPreference::HighPerformance;
  requestAdapterOpts.compatibleSurface = surface;
  m_adapter = instance.requestAdapter(requestAdapterOpts);
  if (!m_adapter) {
    throw std::runtime_error{"Adapter not available"};
  }
}

Adapter::~Adapter() { m_adapter.release(); }

wgpu::Device Adapter::requestDevice(const wgpu::DeviceDescriptor& desc) {
  return m_adapter.requestDevice(desc);
}

Adapter::operator wgpu::Adapter() const { return m_adapter; }
