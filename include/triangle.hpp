#pragma once

#include <webgpu/webgpu.hpp>
#include "glm/ext/vector_float3.hpp"
#include "renderer/buffer.hpp"
#include "renderer/program.hpp"
#include "renderer/renderer.hpp"

class Triangle {
 public:
  explicit Triangle(Renderer& renderer, wgpu::BindGroupLayout cameraBindGroupLayout);

  void draw(wgpu::TextureView view, wgpu::CommandEncoder encoder, wgpu::BindGroup cameraBindGroup);

 private:
  Program m_program;
  VertexBuffer<glm::vec3> m_vertices;
};
