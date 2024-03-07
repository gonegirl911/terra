#include <webgpu/webgpu.hpp>
#include "renderer/renderer.hpp"

class DepthBuffer {
 public:
  static constexpr auto FORMAT = wgpu::TextureFormat::Depth32Float;

  explicit DepthBuffer(Renderer& renderer);
  DepthBuffer(const DepthBuffer&) = delete;
  DepthBuffer& operator=(const DepthBuffer&) = delete;
  DepthBuffer& operator=(DepthBuffer&& other) noexcept;
  ~DepthBuffer();

  wgpu::TextureView view() const;

 private:
  wgpu::Texture m_texture{nullptr};
  wgpu::TextureView m_view{nullptr};
};
