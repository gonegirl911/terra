#pragma once

#include <webgpu/webgpu.h>
#include <concepts>
#include <webgpu/webgpu.hpp>
#include "window.hpp"

class Renderer {
 public:
  wgpu::Instance instance;
  wgpu::Surface surface{nullptr};
  wgpu::Device device{nullptr};
  wgpu::Queue queue{nullptr};
  wgpu::SurfaceConfiguration config{wgpu::Default};
  bool isResized{};

  explicit Renderer(const Window& window);
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;
  ~Renderer();

  void update(const Window& window);

  void draw(std::invocable<wgpu::TextureView, wgpu::CommandEncoder> auto f) {
    wgpu::SurfaceTexture texture;
    surface.getCurrentTexture(&texture);
    switch (texture.status) {
      case WGPUSurfaceGetCurrentTextureStatus_Success: {
        const auto view = wgpuTextureCreateView(texture.texture, nullptr);
        auto encoder = device.createCommandEncoder(wgpu::Default);
        f(view, encoder);
        queue.submit(encoder.finish());
        surface.present();
        encoder.release();
        wgpuTextureViewRelease(view);
        wgpuTextureRelease(texture.texture);
        break;
      }
      case WGPUSurfaceGetCurrentTextureStatus_Lost:
        recreateSurface();
        break;
      default:
        break;
    }
  }

 private:
  bool shouldResize{true};

  void recreateSurface();

  bool resize(int width, int height);

  friend class Application;
};
