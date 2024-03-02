#pragma once

#include <array>
#include <cstddef>
#include <cstring>
#include <span>
#include <tuple>
#include <utility>
#include <webgpu/webgpu.hpp>
#include "renderer.hpp"

template <typename T>
class Buffer {
 public:
  Buffer(Renderer& renderer, std::span<const T> data, wgpu::BufferUsageFlags usage)
      : Buffer{renderer, data.size(), usage, true} {
    const auto sizeBytes = data.size_bytes();
    std::memcpy(m_buffer.getMappedRange(0, sizeBytes), data.data(), sizeBytes);
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

template <typename F>
class make {
 public:
  make(F&& f) : f{std::forward<F>(f)} {}

  operator auto() { return f(); }

 private:
  F f;
};

template <typename... Ts>
class BufferGroup {
 public:
  BufferGroup(Renderer& renderer, std::span<const Ts>... data)
      : m_buffers{make{[&] {
          return Buffer<Ts>{renderer, data, wgpu::BufferUsage::Storage};
        }}...} {
    init(renderer, wgpu::BufferBindingType::ReadOnlyStorage);
  }

  BufferGroup(Renderer& renderer, std::size_t size, wgpu::BufferUsageFlags usage)
    requires (sizeof...(Ts) == 1)
      : m_buffers{make{[&] {
          return Buffer<Ts>{renderer, size, usage | wgpu::BufferUsage::Storage};
        }}...} {
    init(renderer, wgpu::BufferBindingType::Storage);
  }

  ~BufferGroup() {
    m_bindGroup.release();
    m_bindGroupLayout.release();
  }

  operator wgpu::Buffer() const
    requires (sizeof...(Ts) == 1)
  {
    return std::get<0>(m_buffers);
  }

  wgpu::BindGroupLayout bindGroupLayout() const { return m_bindGroupLayout; }

  wgpu::BindGroup bindGroup() const { return m_bindGroup; }

  std::size_t size()
    requires (sizeof...(Ts) == 1)
  {
    return std::get<0>(m_buffers).size();
  }

  std::size_t sizeBytes()
    requires (sizeof...(Ts) == 1)
  {
    return std::get<0>(m_buffers).sizeBytes();
  }

 private:
  std::tuple<Buffer<Ts>...> m_buffers;
  wgpu::BindGroupLayout m_bindGroupLayout{nullptr};
  wgpu::BindGroup m_bindGroup{nullptr};

  void init(Renderer& renderer, wgpu::BufferBindingType type) {
    const auto bindGroupLayoutEntries = [&]<std::size_t... Is>(std::index_sequence<Is...>) {
      return std::array{[&] {
        wgpu::BindGroupLayoutEntry entry{wgpu::Default};
        entry.binding = Is;
        entry.visibility = wgpu::ShaderStage::Compute;
        entry.buffer.type = type;
        return entry;
      }()...};
    }(std::make_index_sequence<sizeof...(Ts)>{});

    wgpu::BindGroupLayoutDescriptor bindGroupLayoutDesc{wgpu::Default};
    bindGroupLayoutDesc.entryCount = bindGroupLayoutEntries.size();
    bindGroupLayoutDesc.entries = bindGroupLayoutEntries.data();

    m_bindGroupLayout = renderer.device.createBindGroupLayout(bindGroupLayoutDesc);

    const auto bindGroupEntries = [&]<std::size_t... Is>(std::index_sequence<Is...>) {
      return std::array{[&] {
        wgpu::BindGroupEntry entry{wgpu::Default};
        entry.binding = Is;
        entry.buffer = *std::get<Is>(m_buffers);
        entry.size = std::get<Is>(m_buffers).sizeBytes();
        return entry;
      }()...};
    }(std::make_index_sequence<sizeof...(Ts)>{});

    wgpu::BindGroupDescriptor bindGroupDesc{wgpu::Default};
    bindGroupDesc.layout = m_bindGroupLayout;
    bindGroupDesc.entryCount = bindGroupLayoutDesc.entryCount;
    bindGroupDesc.entries = bindGroupEntries.data();

    m_bindGroup = renderer.device.createBindGroup(bindGroupDesc);
  }
};

template <typename T>
class VertexBuffer {
 public:
  VertexBuffer(Renderer& renderer, std::size_t size)
      : m_buffer{renderer, size, wgpu::BufferUsage::Vertex} {}

  wgpu::BindGroupLayout bindGroupLayout() const { return m_buffer.bindGroupLayout(); }

  wgpu::BindGroup bindGroup() const { return m_buffer.bindGroup(); }

  void draw(wgpu::RenderPassEncoder renderPass) {
    renderPass.setVertexBuffer(0, m_buffer, 0, m_buffer.sizeBytes());
    renderPass.draw(m_buffer.size(), 1, 0, 0);
  }

 private:
  BufferGroup<T> m_buffer;
};

template <typename T>
class UniformBuffer {
 public:
  explicit UniformBuffer(Renderer& renderer)
      : m_buffer{renderer, 1, wgpu::BufferUsage::Uniform | wgpu::BufferUsage::CopyDst} {}

  wgpu::Buffer operator*() const { return m_buffer; }

  std::size_t sizeBytes() { return m_buffer.sizeBytes(); }

  void set(Renderer& renderer, const T& value) const {
    renderer.queue.writeBuffer(m_buffer, 0, &value, sizeof(T));
  }

 private:
  Buffer<T> m_buffer;
};
