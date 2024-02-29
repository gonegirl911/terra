#pragma once

#include <cstddef>
#include <webgpu/webgpu.hpp>
#include "renderer.hpp"

template <typename T>
class Buffer {
 public:
  Buffer(Renderer& renderer, std::size_t size, wgpu::BufferUsageFlags usage)
      : Buffer{renderer, size, usage, false} {}

  Buffer(const Buffer&) = delete;

  Buffer& operator=(const Buffer&) = delete;

  ~Buffer() {
    m_buffer.destroy();
    m_buffer.release();
  }

  operator wgpu::Buffer() const { return m_buffer; }

  wgpu::Buffer operator*() const { return m_buffer; }

  std::size_t size() { return sizeBytes() / sizeof(T); }

  std::size_t sizeBytes() { return m_buffer.getSize(); }

 private:
  wgpu::Buffer m_buffer{nullptr};

  Buffer(Renderer& renderer, std::size_t size, wgpu::BufferUsageFlags usage,
         bool mappedAtCreation) {
    wgpu::BufferDescriptor desc{wgpu::Default};
    desc.size = size * sizeof(T);
    desc.usage = usage;
    desc.mappedAtCreation = mappedAtCreation;
    m_buffer = renderer.device.createBuffer(desc);
  }
};

template <typename T>
class VertexBuffer {
 public:
  VertexBuffer(Renderer& renderer, std::size_t size)
      : m_buffer{renderer, size, wgpu::BufferUsage::Vertex | wgpu::BufferUsage::Storage} {
    wgpu::BindGroupLayoutEntry bindGroupLayoutEntry{wgpu::Default};
    bindGroupLayoutEntry.visibility = wgpu::ShaderStage::Compute;
    bindGroupLayoutEntry.buffer.type = wgpu::BufferBindingType::Storage;

    wgpu::BindGroupLayoutDescriptor bindGroupLayoutDesc{wgpu::Default};
    bindGroupLayoutDesc.entryCount = 1;
    bindGroupLayoutDesc.entries = &bindGroupLayoutEntry;

    m_bindGroupLayout = renderer.device.createBindGroupLayout(bindGroupLayoutDesc);

    wgpu::BindGroupEntry bindGroupEntry{wgpu::Default};
    bindGroupEntry.buffer = *m_buffer;
    bindGroupEntry.size = m_buffer.sizeBytes();

    wgpu::BindGroupDescriptor bindGroupDesc{wgpu::Default};
    bindGroupDesc.layout = m_bindGroupLayout;
    bindGroupDesc.entryCount = bindGroupLayoutDesc.entryCount;
    bindGroupDesc.entries = &bindGroupEntry;

    m_bindGroup = renderer.device.createBindGroup(bindGroupDesc);
  }

  wgpu::BindGroupLayout bindGroupLayout() const { return m_bindGroupLayout; }

  wgpu::BindGroup bindGroup() const { return m_bindGroup; }

  void draw(wgpu::RenderPassEncoder renderPass) {
    renderPass.setVertexBuffer(0, m_buffer, 0, m_buffer.sizeBytes());
    renderPass.draw(m_buffer.size(), 1, 0, 0);
  }

 private:
  Buffer<T> m_buffer;
  wgpu::BindGroupLayout m_bindGroupLayout{nullptr};
  wgpu::BindGroup m_bindGroup{nullptr};
};

template <typename T>
class UniformBuffer {
 public:
  UniformBuffer(Renderer& renderer)
      : m_buffer{renderer, 1, wgpu::BufferUsage::Uniform | wgpu::BufferUsage::CopyDst} {}

  wgpu::Buffer operator*() const { return m_buffer; }

  std::size_t sizeBytes() { return m_buffer.sizeBytes(); }

  void set(Renderer& renderer, const T& value) const {
    renderer.queue.writeBuffer(m_buffer, 0, &value, sizeof(T));
  }

 private:
  Buffer<T> m_buffer;
};
