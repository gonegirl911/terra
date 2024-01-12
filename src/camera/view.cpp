#include "camera/view.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"

View::View(glm::vec3 origin, glm::vec3 dir)
    : m_origin{origin},
      m_forward{glm::normalize(dir)},
      m_right{right(m_forward)},
      m_up{up(m_forward, m_right)},
      m_yaw{glm::atan(m_forward.z, m_forward.x)},
      m_pitch{glm::asin(m_forward.y)} {}

glm::mat4 View::matrix() const {
  // clang-format off
  return {
    m_right.x, m_up.x, m_forward.x, 0.0f,
    m_right.y, m_up.y, m_forward.y, 0.0f,
    m_right.z, m_up.z, m_forward.z, 0.0f,
    -glm::dot(m_origin, m_right), -glm::dot(m_origin, m_up), -glm::dot(m_origin, m_forward), 1.0f,
  };
  // clang-format on
}

glm::vec3 View::forward(float yaw, float pitch) {
  return {
    glm::cos(yaw) * glm::cos(pitch),
    glm::sin(pitch),
    glm::sin(yaw) * glm::cos(pitch),
  };
}

glm::vec3 View::right(glm::vec3 forward) {
  return glm::normalize(glm::cross({0.0f, 1.0f, 0.0f}, forward));
}

glm::vec3 View::up(glm::vec3 forward, glm::vec3 right) { return glm::cross(forward, right); }
