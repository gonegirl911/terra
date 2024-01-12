#pragma once

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"

class View {
 public:
  View(glm::vec3 origin, glm::vec3 dir);

  glm::mat4 matrix() const;

 private:
  glm::vec3 m_origin;
  glm::vec3 m_forward;
  glm::vec3 m_right;
  glm::vec3 m_up;
  float m_yaw;
  float m_pitch;

  static glm::vec3 forward(float yaw, float pitch);

  static glm::vec3 right(glm::vec3 forward);

  static glm::vec3 up(glm::vec3 forward, glm::vec3 right);

  friend class Controller;
};
