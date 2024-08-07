#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <array>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <ranges>
#include <span>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <webgpu/webgpu.hpp>
#include "glm/ext/vector_float4.hpp"
#include "glm/gtx/hash.hpp"
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

  Buffer(Buffer&& other) noexcept { std::swap(m_buffer, other.m_buffer); }

  Buffer& operator=(const Buffer&) = delete;

  ~Buffer() {
    if (m_buffer) {
      m_buffer.destroy();
      m_buffer.release();
    }
  }

  std::size_t size() { return sizeBytes() / sizeof(T); }

  std::size_t sizeBytes() { return m_buffer.getSize(); }

  operator wgpu::Buffer() const { return m_buffer; }

  wgpu::Buffer operator*() const { return m_buffer; }

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
  BufferGroup(Renderer& renderer, Buffer<Ts>&&... buffers, wgpu::BindGroupLayout bindGroupLayout)
      : m_buffers{std::forward<Buffer<Ts>>(buffers)...} {
    const auto bindGroupEntries = [&]<std::size_t... Is>(std::index_sequence<Is...>) {
      return std::array{[&] {
        wgpu::BindGroupEntry entry{wgpu::Default};
        entry.binding = Is;
        entry.buffer = *std::get<Is>(m_buffers);
        entry.size = std::get<Is>(m_buffers).sizeBytes();
        return entry;
      }()...};
    }(std::index_sequence_for<Ts...>{});

    wgpu::BindGroupDescriptor bindGroupDesc{wgpu::Default};
    bindGroupDesc.layout = bindGroupLayout;
    bindGroupDesc.entryCount = bindGroupEntries.size();
    bindGroupDesc.entries = bindGroupEntries.data();

    m_bindGroup = renderer.device.createBindGroup(bindGroupDesc);
  }

  ~BufferGroup() { m_bindGroup.release(); }

  wgpu::BindGroup bindGroup() const { return m_bindGroup; }

  operator wgpu::Buffer() const
    requires (sizeof...(Ts) == 1)
  {
    return *std::get<0>(m_buffers);
  }

  template <std::size_t Index>
    requires (sizeof...(Ts) > 1)
  auto& get() {
    return std::get<Index>(m_buffers);
  }

  static wgpu::BindGroupLayout bindGroupLayout(
      Renderer& renderer, wgpu::ShaderStageFlags visibility,
      std::array<wgpu::BufferBindingType, sizeof...(Ts)> types) {
    const auto bindGroupLayoutEntries = [&]<std::size_t... Is>(std::index_sequence<Is...>) {
      return std::array{[&] {
        wgpu::BindGroupLayoutEntry entry{wgpu::Default};
        entry.binding = Is;
        entry.visibility = visibility;
        entry.buffer.type = types[Is];
        return entry;
      }()...};
    }(std::index_sequence_for<Ts...>{});

    wgpu::BindGroupLayoutDescriptor bindGroupLayoutDesc{wgpu::Default};
    bindGroupLayoutDesc.entryCount = bindGroupLayoutEntries.size();
    bindGroupLayoutDesc.entries = bindGroupLayoutEntries.data();

    return renderer.device.createBindGroupLayout(bindGroupLayoutDesc);
  }

 private:
  std::tuple<Buffer<Ts>...> m_buffers;
  wgpu::BindGroup m_bindGroup{nullptr};
};

template <typename... Ts>
class ConstantGroup {
 public:
  ConstantGroup(Renderer& renderer, std::span<const Ts>... data)
      : m_bindGroupLayout{decltype(m_buffers)::bindGroupLayout(
            renderer, wgpu::ShaderStage::Compute, {[]<typename T> {
              return wgpu::BufferBindingType::ReadOnlyStorage;
            }.template operator()<Ts>()...})},
        m_buffers{
            renderer,
            {renderer, data, wgpu::BufferUsage::Storage}...,
            m_bindGroupLayout,
        } {}

  ~ConstantGroup() { m_bindGroupLayout.release(); }

  wgpu::BindGroupLayout bindGroupLayout() const { return m_bindGroupLayout; }

  wgpu::BindGroup bindGroup() const { return m_buffers.bindGroup(); }

 private:
  wgpu::BindGroupLayout m_bindGroupLayout;
  BufferGroup<Ts...> m_buffers;
};

template <typename V>
class ChunkMap {
 public:
  ChunkMap(Renderer& renderer)
      : m_bindGroupLayout{decltype(m_chunks)::mapped_type::bindGroupLayout(
            renderer, wgpu::ShaderStage::Compute,
            {wgpu::BufferBindingType::Uniform, wgpu::BufferBindingType::Storage,
             wgpu::BufferBindingType::Storage})} {}

  ~ChunkMap() { m_bindGroupLayout.release(); }

  void insert(Renderer& renderer, glm::vec4 coords, std::size_t size) {
    m_chunks.try_emplace(
        coords, renderer, Buffer<glm::vec4>{renderer, {&coords, 1}, wgpu::BufferUsage::Uniform},
        Buffer<V>{renderer, size, wgpu::BufferUsage::Vertex | wgpu::BufferUsage::Storage},
        Buffer<std::uint32_t>{renderer, 1, wgpu::BufferUsage::Storage}, m_bindGroupLayout);
  }

  wgpu::BindGroupLayout bindGroupLayout() const { return m_bindGroupLayout; }

  void apply(std::invocable<wgpu::BindGroup> auto f) const {
    for (const auto& buffers : std::views::values(m_chunks)) {
      f(buffers.bindGroup());
    }
  }

  void draw(wgpu::RenderPassEncoder renderPass) {
    for (auto& buffers : std::views::values(m_chunks)) {
      auto& vertexBuffer = buffers.template get<1>();
      renderPass.setVertexBuffer(0, vertexBuffer, 0, vertexBuffer.sizeBytes());
      renderPass.draw(vertexBuffer.size(), 1, 0, 0);
    }
  }

 private:
  wgpu::BindGroupLayout m_bindGroupLayout;
  std::unordered_map<glm::vec4, BufferGroup<glm::vec4, V, std::uint32_t>> m_chunks{};
};

template <typename T>
class Uniform {
 public:
  Uniform(Renderer& renderer, wgpu::ShaderStageFlags visibility)
      : m_bindGroupLayout{decltype(m_buffer)::bindGroupLayout(renderer, visibility,
                                                              {wgpu::BufferBindingType::Uniform})},
        m_buffer{renderer,
                 {renderer, 1, wgpu::BufferUsage::Uniform | wgpu::BufferUsage::CopyDst},
                 m_bindGroupLayout} {}

  ~Uniform() { m_bindGroupLayout.release(); }

  wgpu::BindGroupLayout bindGroupLayout() const { return m_bindGroupLayout; }

  wgpu::BindGroup bindGroup() const { return m_buffer.bindGroup(); }

  void set(Renderer& renderer, const T& value) const {
    renderer.queue.writeBuffer(m_buffer, 0, &value, sizeof(T));
  }

 private:
  wgpu::BindGroupLayout m_bindGroupLayout;
  BufferGroup<T> m_buffer;
};
