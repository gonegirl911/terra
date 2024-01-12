#pragma once

#include "renderer.hpp"

class Shader {
 public:
  explicit Shader(Renderer& renderer);
  Shader(const Shader&) = delete;
  Shader& operator=(const Shader&) = delete;
  ~Shader();

  operator wgpu::ShaderModule() const;

 private:
  wgpu::ShaderModule m_module{nullptr};
};
