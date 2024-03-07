#pragma once

#include <webgpu/webgpu.hpp>
#include "camera/controller.hpp"
#include "camera/projection.hpp"
#include "camera/view.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
#include "renderer/buffer.hpp"
#include "renderer/renderer.hpp"

struct CameraUniformData {
  glm::mat4 vp;
  glm::vec3 origin;
  float padding{};
};

class Camera {
 public:
  explicit Camera(Renderer& renderer);

  void onCursorPos(double dx, double dy);

  void onKey(int key, int action);

  void update(Renderer& renderer, float dt);

  wgpu::BindGroupLayout bindGroupLayout() const;

  wgpu::BindGroup bindGroup() const;

 private:
  View m_view{{0.0, 1.62, 0.0}, {0.0, 0.0, 1.0}};
  Projection m_projection{90.0, 0.0, 0.1, 1000.0};
  Controller m_controller{25.0, 0.0025};
  Uniform<CameraUniformData> m_uniform;
};
