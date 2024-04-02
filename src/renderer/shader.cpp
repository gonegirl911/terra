#include "renderer/shader.hpp"
#include <filesystem>
#include <fstream>
#include <ios>
#include <iterator>
#include <string>
#include <webgpu/webgpu.hpp>
#include "renderer/renderer.hpp"

std::string readToString(const fs::path& path) {
  std::ifstream ifs{path};
  ifs.exceptions(std::ios_base::failbit);
  return {std::istreambuf_iterator{ifs}, {}};
}

Shader::Shader(Renderer& renderer, const fs::path& path) {
  const auto src = readToString(path);

  wgpu::ShaderModuleWGSLDescriptor srcDesc{wgpu::Default};
  srcDesc.chain.sType = wgpu::SType::ShaderModuleWGSLDescriptor;
  srcDesc.code = src.data();

  wgpu::ShaderModuleDescriptor moduleDesc{wgpu::Default};
  moduleDesc.nextInChain = &srcDesc.chain;

  m_module = renderer.device.createShaderModule(moduleDesc);
}

Shader::~Shader() { m_module.release(); }

Shader::operator wgpu::ShaderModule() const { return m_module; }
