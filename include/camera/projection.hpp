#pragma once

#include "glm/ext/matrix_float4x4.hpp"

class Projection {
 public:
  Projection(float fovy, float aspect, float znear, float zfar);

  glm::mat4 matrix() const;

 private:
  float m_fovy;
  float m_aspect;
  float m_znear;
  float m_zfar;

  friend class Camera;
};
