#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
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

  Buffer(Buffer&& other) { std::swap(m_buffer, other.m_buffer); }

  Buffer& operator=(const Buffer&) = delete;

  ~Buffer() {
    if (m_buffer) {
      m_buffer.destroy();
      m_buffer.release();
    }
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

template <typename... Ts>
class BufferGroup {
 public:
  BufferGroup(Renderer& renderer, Buffer<Ts>&&... buffers, wgpu::ShaderStageFlags visibility,
              wgpu::BufferBindingType type)
      : m_buffers{std::forward<Buffer<Ts>>(buffers)...} {
    const auto bindGroupLayoutEntries = [&]<std::size_t... Is>(std::index_sequence<Is...>) {
      return std::array{[&] {
        wgpu::BindGroupLayoutEntry entry{wgpu::Default};
        entry.binding = Is;
        entry.visibility = visibility;
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

  ~BufferGroup() {
    m_bindGroup.release();
    m_bindGroupLayout.release();
  }

  template <std::size_t Index>
  wgpu::Buffer get() const {
    return std::get<Index>(m_buffers);
  }

  template <std::size_t Index>
  std::size_t size() {
    return std::get<Index>(m_buffers).size();
  }

  template <std::size_t Index>
  std::size_t sizeBytes() {
    return std::get<Index>(m_buffers).sizeBytes();
  }

  wgpu::BindGroupLayout bindGroupLayout() const { return m_bindGroupLayout; }

  wgpu::BindGroup bindGroup() const { return m_bindGroup; }

 private:
  std::tuple<Buffer<Ts>...> m_buffers;
  wgpu::BindGroupLayout m_bindGroupLayout{nullptr};
  wgpu::BindGroup m_bindGroup{nullptr};
};

template <typename... Ts>
class ConstantGroup {
 public:
  ConstantGroup(Renderer& renderer, std::span<const Ts>... data)
      : m_buffers{
          renderer,
          {renderer, data, wgpu::BufferUsage::Storage}...,
          wgpu::ShaderStage::Compute,
          wgpu::BufferBindingType::ReadOnlyStorage,
        } {}

  wgpu::BindGroupLayout bindGroupLayout() const { return m_buffers.bindGroupLayout(); }

  wgpu::BindGroup bindGroup() const { return m_buffers.bindGroup(); }

 private:
  BufferGroup<Ts...> m_buffers;
};

template <typename T>
class VertexBuffer {
 public:
  VertexBuffer(Renderer& renderer, std::size_t size)
      : m_buffers{renderer,
                  {renderer, size, wgpu::BufferUsage::Vertex | wgpu::BufferUsage::Storage},
                  {renderer, 1, wgpu::BufferUsage::Storage},
                  wgpu::ShaderStage::Compute,
                  wgpu::BufferBindingType::Storage} {}

  std::size_t sizeBytes() { return m_buffers.template sizeBytes<0>(); }

  wgpu::BindGroupLayout bindGroupLayout() const { return m_buffers.bindGroupLayout(); }

  wgpu::BindGroup bindGroup() const { return m_buffers.bindGroup(); }

  void draw(wgpu::RenderPassEncoder renderPass) {
    renderPass.setVertexBuffer(0, m_buffers.template get<0>(), 0, sizeBytes());
    renderPass.draw(m_buffers.template size<0>(), 1, 0, 0);
  }

 private:
  BufferGroup<T, std::uint32_t> m_buffers;
};

template <typename T>
class Uniform {
 public:
  Uniform(Renderer& renderer, wgpu::ShaderStageFlags visibility)
      : m_buffers{renderer,
                  {renderer, 1, wgpu::BufferUsage::Uniform | wgpu::BufferUsage::CopyDst},
                  visibility,
                  wgpu::BufferBindingType::Uniform} {}

  wgpu::BindGroupLayout bindGroupLayout() const { return m_buffers.bindGroupLayout(); }

  wgpu::BindGroup bindGroup() const { return m_buffers.bindGroup(); }

  void set(Renderer& renderer, const T& value) const {
    renderer.queue.writeBuffer(m_buffers.template get<0>(), 0, &value, sizeof(T));
  }

 private:
  BufferGroup<T> m_buffers;
};
