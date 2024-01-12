#pragma once

#include <cstring>
#include <span>
#include <webgpu/webgpu.hpp>
#include "renderer.hpp"

template <typename T>
class Buffer {
 public:
  Buffer(Renderer& renderer, std::span<const T> data, wgpu::BufferUsageFlags usage)
      : Buffer{renderer, data.size_bytes(), usage, true} {
    const auto size = sizeBytes();
    std::memcpy(m_buffer.getMappedRange(0, size), data.data(), size);
    m_buffer.unmap();
  }

  Buffer(Renderer& renderer, std::size_t size, wgpu::BufferUsageFlags usage)
      : Buffer{renderer, size, usage, false} {}

  Buffer(const Buffer&) = delete;

  Buffer& operator=(const Buffer&) = delete;

  ~Buffer() {
    m_buffer.destroy();
    m_buffer.release();
  }

  operator wgpu::Buffer() const { return m_buffer; }

  std::size_t sizeBytes() { return m_buffer.getSize(); }

  std::size_t size() { return sizeBytes() / sizeof(T); }

 private:
  wgpu::Buffer m_buffer{nullptr};

  Buffer(Renderer& renderer, std::size_t size, wgpu::BufferUsageFlags usage,
         bool mappedAtCreation) {
    wgpu::BufferDescriptor desc{wgpu::Default};
    desc.size = size;
    desc.usage = usage;
    desc.mappedAtCreation = mappedAtCreation;
    m_buffer = renderer.device.createBuffer(desc);
  }
};

template <typename T>
class VertexBuffer {
 public:
  VertexBuffer(Renderer& renderer, std::span<const T> data)
      : m_buffer{renderer, data, wgpu::BufferUsage::Vertex} {}

  void draw(wgpu::RenderPassEncoder renderPass) {
    renderPass.setVertexBuffer(0, m_buffer, 0, m_buffer.sizeBytes());
    renderPass.draw(m_buffer.size(), 1, 0, 0);
  }

 private:
  Buffer<T> m_buffer;
};

template <typename T>
class UniformBuffer {
 public:
  UniformBuffer(Renderer& renderer)
      : m_buffer{renderer, sizeof(T), wgpu::BufferUsage::Uniform | wgpu::BufferUsage::CopyDst} {}

  wgpu::Buffer operator*() const { return m_buffer; }

  void set(Renderer& renderer, const T& value) const {
    renderer.queue.writeBuffer(m_buffer, 0, &value, sizeof(T));
  }

 private:
  Buffer<T> m_buffer;
};
