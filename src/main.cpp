#define GLFW_INCLUDE_NONE
#define WEBGPU_CPP_IMPLEMENTATION
#include "renderer.hpp"
#include "window.hpp"

int main() {
  const Window window{};
  const Renderer renderer{};
  window.run();
}
