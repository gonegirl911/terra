#pragma once

#include "camera/controller.hpp"
#include "camera/projection.hpp"
#include "camera/view.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "renderer/renderer.hpp"
#include "renderer/uniform.hpp"

class Camera {
 public:
  Camera(Renderer& renderer);

  void onCursorPos(double dx, double dy);

  void onKey(int key, int action);

  void update(Renderer& renderer, float dt);

  wgpu::BindGroupLayout bindGroupLayout() const;

  wgpu::BindGroup bindGroup() const;

 private:
  View m_view{{0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 1.0f}};
  Projection m_projection{90.0f, 0.0f, 0.1f, 100.0f};
  Controller m_controller{25.0f, 0.0025f};
  Uniform<glm::mat4> m_uniform;
};
