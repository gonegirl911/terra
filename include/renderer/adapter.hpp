#pragma once

#include <webgpu/webgpu.hpp>

class Adapter {
 public:
  Adapter(wgpu::Instance instance, wgpu::Surface surface);
  Adapter(const Adapter&) = delete;
  Adapter& operator=(const Adapter&) = delete;
  ~Adapter();

  wgpu::Device requestDevice(const wgpu::DeviceDescriptor& desc);

  operator wgpu::Adapter() const;

 private:
  wgpu::Adapter m_adapter{nullptr};
};
