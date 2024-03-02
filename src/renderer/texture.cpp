#include "renderer/texture.hpp"
#include <utility>
#include <webgpu/webgpu.hpp>
#include "renderer/renderer.hpp"

DepthBuffer::DepthBuffer(Renderer& renderer) {
  wgpu::TextureDescriptor textureDesc{wgpu::Default};
  textureDesc.size = {renderer.config.width, renderer.config.height, 1};
  textureDesc.mipLevelCount = 1;
  textureDesc.sampleCount = 1;
  textureDesc.dimension = wgpu::TextureDimension::_2D;
  textureDesc.format = FORMAT;
  textureDesc.usage = wgpu::TextureUsage::RenderAttachment;

  m_texture = renderer.device.createTexture(textureDesc);

  wgpu::TextureViewDescriptor textureViewDesc{wgpu::Default};
  textureViewDesc.mipLevelCount = 1;
  textureViewDesc.arrayLayerCount = 1;

  m_view = m_texture.createView(textureViewDesc);
}

DepthBuffer& DepthBuffer::operator=(DepthBuffer&& other) {
  std::swap(m_texture, other.m_texture);
  std::swap(m_view, other.m_view);
  return *this;
}

DepthBuffer::~DepthBuffer() {
  m_view.release();
  m_texture.destroy();
  m_texture.release();
}

wgpu::TextureView DepthBuffer::view() const { return m_view; }
