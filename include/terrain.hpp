#pragma once

#include <webgpu/webgpu.hpp>
#include "glm/ext/vector_float3.hpp"
#include "renderer/buffer.hpp"
#include "renderer/layout.hpp"
#include "renderer/program.hpp"
#include "renderer/renderer.hpp"

struct TerrainVertex {
  glm::vec3 coords;
  glm::vec3 normal;

  static constexpr auto DESC = LAYOUT<glm::vec3, glm::vec3>;
};

class Terrain {
 public:
  explicit Terrain(Renderer& renderer, wgpu::BindGroupLayout cameraBindGroupLayout);

  void draw(wgpu::TextureView view, wgpu::CommandEncoder encoder, wgpu::BindGroup cameraBindGroup);

 private:
  VertexBuffer<TerrainVertex> m_vertices;
  ComputeProgram m_generator;
  RenderProgram m_program;
  bool m_shouldGenerate{true};

  void generate(wgpu::CommandEncoder encoder) const;
};
