#include "renderer/shader.hpp"
#include <filesystem>
#include <fstream>
#include <ios>
#include <iterator>
#include <string>
#include <webgpu/webgpu.hpp>
#include "renderer/renderer.hpp"

std::string readToString(const std::filesystem::path& path) {
  std::ifstream ifs{path};
  ifs.exceptions(std::ios_base::badbit);
  return std::string{std::istreambuf_iterator<char>{ifs}, {}};
}

Shader::Shader(Renderer& renderer) {
  const auto src = readToString("../assets/shaders/triangle.wgsl");

  wgpu::ShaderModuleWGSLDescriptor srcDesc{wgpu::Default};
  srcDesc.chain.sType = wgpu::SType::ShaderModuleWGSLDescriptor;
  srcDesc.code = src.data();

  wgpu::ShaderModuleDescriptor moduleDesc{wgpu::Default};
  moduleDesc.nextInChain = &srcDesc.chain;

  m_module = renderer.device.createShaderModule(moduleDesc);
}

Shader::~Shader() { m_module.release(); }

Shader::operator wgpu::ShaderModule() const { return m_module; }
