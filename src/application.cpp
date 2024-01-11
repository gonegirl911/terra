#include "application.hpp"
#include <webgpu/webgpu.h>
#include <webgpu/webgpu.hpp>

Application::Application() : m_window{}, m_renderer{m_window} {
  m_window.setUserData(*this);

  m_window.setResizeCallback([](auto window, auto, auto) {
    Window::getUserData<Application>(window).m_renderer.shouldResize = true;
  });

  m_window.setMouseButtonCallback([](auto window, auto key, auto action, auto) {
    Window::getUserData<Application>(window).m_window.onMouseButton(key, action);
  });

  m_window.setKeyCallback([](auto window, auto key, auto, auto action, auto) {
    Window::getUserData<Application>(window).m_window.onKey(key, action);
  });
}

void Application::run() {
  m_window.run([&] {
    update();
    draw();
  });
}

void Application::update() { m_renderer.update(m_window); }

void Application::draw() {
  wgpu::SurfaceTexture texture;
  m_renderer.surface.getCurrentTexture(&texture);
  switch (texture.status) {
    case WGPUSurfaceGetCurrentTextureStatus_Success: {
      auto view = wgpuTextureCreateView(texture.texture, nullptr);
      auto encoder = m_renderer.device.createCommandEncoder({});

      wgpu::RenderPassColorAttachment renderPassColorAttachment{};
      renderPassColorAttachment.view = view;
      renderPassColorAttachment.loadOp = wgpu::LoadOp::Clear;
      renderPassColorAttachment.clearValue = wgpu::Color{0.9, 0.1, 0.2, 1.0};
      renderPassColorAttachment.storeOp = wgpu::StoreOp::Store;

      wgpu::RenderPassDescriptor renderPassDesc{};
      renderPassDesc.colorAttachmentCount = 1;
      renderPassDesc.colorAttachments = &renderPassColorAttachment;

      auto renderPass = encoder.beginRenderPass(renderPassDesc);
      renderPass.end();

      m_renderer.queue.submit(encoder.finish());
      m_renderer.surface.present();

      renderPass.release();
      encoder.release();
      wgpuTextureViewRelease(view);
      wgpuTextureRelease(texture.texture);
      break;
    }
    case WGPUSurfaceGetCurrentTextureStatus_Lost:
      m_renderer.recreateSurface();
      break;
    default:
      break;
  }
}
