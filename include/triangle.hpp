#pragma once

#include <webgpu/webgpu.hpp>
#include "glm/ext/vector_float3.hpp"
#include "renderer/buffer.hpp"
#include "renderer/layout.hpp"
#include "renderer/program.hpp"
#include "renderer/renderer.hpp"

struct TriangleVertex {
  glm::vec3 coords;
  glm::vec3 normal;

  static constexpr auto DESC = LAYOUT<glm::vec3, glm::vec3>;
};

class Triangle {
 public:
  explicit Triangle(Renderer& renderer, wgpu::BindGroupLayout cameraBindGroupLayout);

  void draw(wgpu::TextureView view, wgpu::CommandEncoder encoder, wgpu::BindGroup cameraBindGroup);

 private:
  Program m_program;
  VertexBuffer<TriangleVertex> m_vertices;
};
