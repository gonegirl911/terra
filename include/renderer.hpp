#pragma once

#include <webgpu/webgpu.hpp>
#include "GLFW/glfw3.h"

class Renderer {
 public:
  explicit Renderer(GLFWwindow* window);
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;
  ~Renderer();

 private:
  wgpu::Instance m_instance{nullptr};
  wgpu::Surface m_surface{nullptr};
  wgpu::Device m_device{nullptr};
  wgpu::Queue m_queue{nullptr};

  class Adapter {
   public:
    explicit Adapter(wgpu::Instance instance, wgpu::Surface surface);
    Adapter(const Adapter&) = delete;
    Adapter& operator=(const Adapter&) = delete;
    ~Adapter();

    wgpu::Device requestDevice(const wgpu::DeviceDescriptor& desc);

   private:
    wgpu::Adapter m_adapter{nullptr};
  };
};
