#pragma once

#include <webgpu/webgpu.h>
#include <array>
#include <cstddef>
#include <cstdint>
#include "glm/vec2.hpp"

template <typename T>
struct FormatOf {};

template <>
struct FormatOf<glm::vec2> {
  static constexpr WGPUVertexFormat VALUE = WGPUVertexFormat_Float32x2;
};

template <typename T>
constexpr WGPUVertexFormat FORMAT_OF = FormatOf<T>::VALUE;

template <typename T>
constexpr T replace(T& dest, T src) {
  const auto value{dest};
  dest = src;
  return value;
}

template <typename... Ts>
constexpr std::array<WGPUVertexAttribute, sizeof...(Ts)> attribs() {
  std::size_t offset = 0;
  std::uint32_t location = 0;
  return {{{.format = FORMAT_OF<Ts>,
            .offset = replace(offset, offset + sizeof(Ts)),
            .shaderLocation = location++}...}};
}

template <typename... Ts>
constexpr std::array<WGPUVertexAttribute, sizeof...(Ts)> ATTRIBS{attribs<Ts...>()};

template <typename... Ts>
constexpr WGPUVertexBufferLayout layout() {
  WGPUVertexBufferLayout layout{};
  layout.arrayStride = (sizeof(Ts) + ...);
  layout.stepMode = WGPUVertexStepMode_Vertex;
  layout.attributeCount = sizeof...(Ts);
  layout.attributes = ATTRIBS<Ts...>.data();
  return layout;
}

template <typename... Ts>
constexpr WGPUVertexBufferLayout LAYOUT{layout<Ts...>()};
