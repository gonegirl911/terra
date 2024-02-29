#pragma once

#include <webgpu/webgpu.hpp>
#include "renderer/buffer.hpp"
#include "renderer/renderer.hpp"

template <typename T>
class Uniform {
 public:
  Uniform(Renderer& renderer, wgpu::ShaderStageFlags visibility) : m_buffer{renderer} {
    wgpu::BindGroupLayoutEntry bindGroupLayoutEntry{wgpu::Default};
    bindGroupLayoutEntry.visibility = visibility;
    bindGroupLayoutEntry.buffer.type = wgpu::BufferBindingType::Uniform;

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

  ~Uniform() {
    m_bindGroup.release();
    m_bindGroupLayout.release();
  }

  wgpu::BindGroupLayout bindGroupLayout() const { return m_bindGroupLayout; }

  wgpu::BindGroup bindGroup() const { return m_bindGroup; }

  void set(Renderer& renderer, const T& data) const { m_buffer.set(renderer, data); }

 private:
  UniformBuffer<T> m_buffer;
  wgpu::BindGroupLayout m_bindGroupLayout{nullptr};
  wgpu::BindGroup m_bindGroup{nullptr};
};
