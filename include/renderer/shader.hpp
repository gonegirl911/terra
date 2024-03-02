#pragma once

#include <filesystem>
#include <webgpu/webgpu.hpp>
#include "renderer.hpp"

namespace fs = std::filesystem;

class Shader {
 public:
  Shader(Renderer& renderer, const fs::path& path);
  Shader(const Shader&) = delete;
  Shader& operator=(const Shader&) = delete;
  ~Shader();

  operator wgpu::ShaderModule() const;

 private:
  wgpu::ShaderModule m_module{nullptr};
};
