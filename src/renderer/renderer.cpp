#define WEBGPU_CPP_IMPLEMENTATION
#include "renderer/renderer.hpp"
#include <format>
#include <stdexcept>
#include <webgpu/webgpu.hpp>
#include "glfw3webgpu.h"
#include "window.hpp"

Renderer::Renderer(const Window& window) : instance{wgpu::createInstance(wgpu::Default)} {
  if (!instance) {
    throw std::runtime_error{"Could not initialize WebGPU"};
  }

  surface = glfwGetWGPUSurface(instance, window);
  if (!surface) {
    throw std::runtime_error{"Could not get surface"};
  }

  wgpu::RequestAdapterOptions requestAdapterOpts{wgpu::Default};
  requestAdapterOpts.powerPreference = wgpu::PowerPreference::HighPerformance;
  requestAdapterOpts.compatibleSurface = surface;
  auto adapter = instance.requestAdapter(requestAdapterOpts);
  if (!adapter) {
    throw std::runtime_error{"Adapter not available"};
  }

  device = adapter.requestDevice(wgpu::Default);
  if (!device) {
    throw std::runtime_error{"Device not available"};
  }
  device.setUncapturedErrorCallback([](auto, auto message) {
    throw std::runtime_error{std::format("Uncaptured device error: {}", message)};
  });

  queue = device.getQueue();

  wgpu::SurfaceCapabilities caps;
  surface.getCapabilities(adapter, &caps);
  if (!caps.formatCount || !caps.presentModeCount) {
    throw std::runtime_error{"Surface not supported by adapter"};
  }

  config.device = device;
  config.usage = wgpu::TextureUsage::RenderAttachment;
  config.format = caps.formats[0];
  config.presentMode = caps.presentModes[0];

  adapter.release();
}

Renderer::~Renderer() {
  queue.release();
  device.release();
  surface.release();
  instance.release();
}

void Renderer::update(const Window& window) {
  if (shouldResize) {
    const auto [width, height] = window.size();
    isResized = resize(width, height);
    shouldResize = false;
  }
}

void Renderer::recreateSurface() { surface.configure(config); }

bool Renderer::resize(int width, int height) {
  if (width && height) {
    config.width = width;
    config.height = height;
    recreateSurface();
    return true;
  } else {
    return false;
  }
}
