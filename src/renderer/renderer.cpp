#define WEBGPU_CPP_IMPLEMENTATION
#include "renderer/renderer.hpp"
#include <stdexcept>
#include <webgpu/webgpu.hpp>
#include "glfw3webgpu.h"
#include "renderer/adapter.hpp"
#include "window.hpp"

Renderer::Renderer(const Window& window) : instance{wgpu::createInstance(wgpu::Default)} {
  if (!instance) {
    throw std::runtime_error{"Could not initialize WebGPU"};
  }

  surface = glfwGetWGPUSurface(instance, window);
  if (!surface) {
    throw std::runtime_error{"Could not get surface"};
  }

  Adapter adapter{instance, surface};

  device = adapter.requestDevice(wgpu::Default);
  if (!device) {
    throw std::runtime_error{"Device not available"};
  }
  device.setUncapturedErrorCallback([](auto type, auto message) {
    std::cout << "Uncaptured device error of type " << type << ":\n" << message << '\n';
  });

  queue = device.getQueue();

  config = getDefaultConfig(window, adapter);
}

Renderer::~Renderer() {
  instance.release();
  surface.release();
  device.release();
  queue.release();
}

void Renderer::recreateSurface() { surface.configure(config); }

void Renderer::update(const Window& window) {
  if (shouldResize) {
    const auto [width, height] = window.size();
    isResized = resize(width, height);
    shouldResize = false;
  }
}

wgpu::SurfaceConfiguration Renderer::getDefaultConfig(const Window& window, wgpu::Adapter adapter) {
  const auto caps = getCapabilities(adapter);
  const auto [width, height] = window.size();
  wgpu::SurfaceConfiguration config{};
  config.device = device;
  config.usage = wgpu::TextureUsage::RenderAttachment;
  config.format = caps.formats[0];
  config.width = width;
  config.height = height;
  config.presentMode = caps.presentModes[0];
  return config;
}

bool Renderer::resize(int width, int height) {
  if (width != 0 && height != 0) {
    config.width = width;
    config.height = height;
    recreateSurface();
    return true;
  } else {
    return false;
  }
}

wgpu::SurfaceCapabilities Renderer::getCapabilities(wgpu::Adapter adapter) {
  wgpu::SurfaceCapabilities caps;
  surface.getCapabilities(adapter, &caps);
  if (caps.formatCount == 0 || caps.presentModeCount == 0) {
    throw std::runtime_error{"Surface not supported by adapter"};
  }
  return caps;
}
