#include <webgpu/webgpu.hpp>
#include "renderer/renderer.hpp"

class DepthBuffer {
 public:
  static constexpr auto FORMAT = wgpu::TextureFormat::Depth32Float;

  explicit DepthBuffer(Renderer& renderer) {
    wgpu::TextureDescriptor textureDesc{wgpu::Default};
    textureDesc.size = {renderer.config.width, renderer.config.height, 1};
    textureDesc.mipLevelCount = 1;
    textureDesc.sampleCount = 1;
    textureDesc.dimension = wgpu::TextureDimension::_2D;
    textureDesc.format = FORMAT;
    textureDesc.usage = wgpu::TextureUsage::RenderAttachment;

    m_texture = renderer.device.createTexture(textureDesc);

    wgpu::TextureViewDescriptor textureViewDesc{wgpu::Default};
    textureViewDesc.mipLevelCount = textureDesc.mipLevelCount;
    textureViewDesc.arrayLayerCount = 1;

    m_view = m_texture.createView(textureViewDesc);
  }

  DepthBuffer(const DepthBuffer&) = delete;

  DepthBuffer& operator=(const DepthBuffer&) = delete;

  ~DepthBuffer() {
    m_view.release();
    m_texture.destroy();
    m_texture.release();
  }

  wgpu::TextureView view() const { return m_view; }

 private:
  wgpu::Texture m_texture{nullptr};
  wgpu::TextureView m_view{nullptr};
};
