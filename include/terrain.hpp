#pragma once

#include <array>
#include <cstdint>
#include <webgpu/webgpu.hpp>
#include "glm/ext/vector_float3.hpp"
#include "renderer/buffer.hpp"
#include "renderer/layout.hpp"
#include "renderer/program.hpp"
#include "renderer/renderer.hpp"
#include "renderer/texture.hpp"

struct TerrainVertex {
  glm::vec3 coords;
  glm::vec3 normal;

  static constexpr auto DESC = LAYOUT<glm::vec3, glm::vec3>;
};

class Terrain {
 public:
  Terrain(Renderer& renderer, wgpu::BindGroupLayout cameraBindGroupLayout);

  void update(Renderer& renderer);

  void draw(wgpu::TextureView view, wgpu::CommandEncoder encoder, wgpu::BindGroup cameraBindGroup);

 private:
  BufferGroup<glm::vec3, std::array<std::int32_t, 16>, std::array<std::uint32_t, 2>> m_tables;
  VertexBuffer<TerrainVertex> m_vertices;
  ComputeProgram m_generator;
  RenderProgram m_program;
  DepthBuffer m_depthBuffer;
  bool m_shouldGenerate{true};

  void generate(wgpu::CommandEncoder encoder) const;
};
