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
  enum Keys : std::uint8_t {
    W = 1 << 0,
    A = 1 << 1,
    S = 1 << 2,
    D = 1 << 3,
    Space = 1 << 4,
    LShift = 1 << 5,
  };

  float m_dx = 0.0f;
  float m_dy = 0.0f;
  std::uint8_t m_relevantKeys{};
  std::uint8_t m_keyHistory{};
  float m_speed;
  float m_sensitivity;

  void applyRotation(View& view);

  void applyMovement(View& view, float dt);
};
