#define GLM_FORCE_LEFT_HANDED
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "camera/projection.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/trigonometric.hpp"

Projection::Projection(float fovy, float aspect, float znear, float zfar)
    : m_fovy{glm::radians(fovy)}, m_aspect{aspect}, m_znear{znear}, m_zfar{zfar} {}

glm::mat4 Projection::matrix() const { return glm::perspective(m_fovy, m_aspect, m_znear, m_zfar); }
