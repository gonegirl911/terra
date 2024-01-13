#pragma once

#include <cstdint>
#include "camera/view.hpp"

class Controller {
 public:
  Controller(float speed, float sensitivity);

  void onCursorPos(double dx, double dy);

  void onKey(int key, int action);

  bool applyUpdates(View& view, float dt);

 private:
  float m_dx = 0.0f;
  float m_dy = 0.0f;
  std::uint8_t m_relevantKeys{};
  std::uint8_t m_keyHistory{};
  float m_speed;
  float m_sensitivity;

  void applyRotation(View& view);

  void applyMovement(View& view, float dt);
};
