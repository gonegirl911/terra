#include "camera/controller.hpp"
#include "GLFW/glfw3.h"
#include "camera/view.hpp"
#include "glm/common.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include "glm/gtc/constants.hpp"

Controller::Controller(float speed, float sensitivity)
    : m_speed{speed}, m_sensitivity{sensitivity} {}

void Controller::onCursorPos(double dx, double dy) {
  m_dx += dx;
  m_dy += dy;
}

void Controller::onKey(int key, int action) {
  Keys value;
  Keys opp;

  switch (key) {
    case GLFW_KEY_W:
      value = Keys::W;
      opp = Keys::S;
      break;
    case GLFW_KEY_A:
      value = Keys::A;
      opp = Keys::D;
      break;
    case GLFW_KEY_S:
      value = Keys::S;
      opp = Keys::W;
      break;
    case GLFW_KEY_D:
      value = Keys::D;
      opp = Keys::A;
      break;
    case GLFW_KEY_SPACE:
      value = Keys::Space;
      opp = Keys::LShift;
      break;
    case GLFW_KEY_LEFT_SHIFT:
      value = Keys::LShift;
      opp = Keys::Space;
      break;
    default:
      return;
  };

  if (action == GLFW_PRESS) {
    m_relevantKeys |= value;
    m_relevantKeys &= ~opp;
    m_keyHistory |= value;
  } else if (action == GLFW_RELEASE) {
    m_relevantKeys &= ~value;
    if (m_keyHistory & opp) {
      m_relevantKeys |= opp;
    }
    m_keyHistory &= ~value;
  }
}

bool Controller::applyUpdates(View& view, float dt) {
  auto isChanged = false;

  if (m_dx || m_dy) {
    applyRotation(view);
    m_dx = 0.0f;
    m_dy = 0.0f;
    isChanged = true;
  }

  if (m_relevantKeys) {
    applyMovement(view, dt);
    isChanged = true;
  }

  return isChanged;
}

void Controller::applyRotation(View& view) {
  static constexpr auto BOUND_Y = glm::half_pi<float>() - glm::epsilon<float>();

  view.m_yaw = glm::mod(view.m_yaw - m_dx * m_sensitivity, glm::tau<float>());
  view.m_pitch = glm::clamp(view.m_pitch - m_dy * m_sensitivity, -BOUND_Y, BOUND_Y);
  view.m_forward = View::forward(view.m_yaw, view.m_pitch);
  view.m_right = View::right(view.m_forward);
  view.m_up = View::up(view.m_forward, view.m_right);
}

void Controller::applyMovement(View& view, float dt) {
  glm::vec3 dir{};
  const auto forward = glm::cross(view.m_right, {0.0f, 1.0f, 0.0f});

  if (m_relevantKeys & Keys::W) {
    dir += forward;
  } else if (m_relevantKeys & Keys::S) {
    dir -= forward;
  }

  if (m_relevantKeys & Keys::A) {
    dir -= view.m_right;
  } else if (m_relevantKeys & Keys::D) {
    dir += view.m_right;
  }

  if (m_relevantKeys & Keys::Space) {
    ++dir.y;
  } else if (m_relevantKeys & Keys::LShift) {
    --dir.y;
  }

  view.m_origin += glm::normalize(dir) * m_speed * dt;
}
