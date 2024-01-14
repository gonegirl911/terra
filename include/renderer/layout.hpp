#pragma once

#include <webgpu/webgpu.h>
#include <array>
#include <cstddef>
#include <cstdint>
#include "glm/ext/vector_float2.hpp"

template <typename T>
struct FormatOf {};

template <>
struct FormatOf<glm::vec2> {
  static constexpr auto VALUE = WGPUVertexFormat_Float32x2;
};

template <typename T>
constexpr auto FORMAT_OF = FormatOf<T>::VALUE;

template <typename T>
constexpr T replace(T& dst, T src) {
  const auto value = dst;
  dst = src;
  return value;
}

template <typename... Ts>
constexpr std::array<WGPUVertexAttribute, sizeof...(Ts)> attribs() {
  std::size_t offset{};
  std::uint32_t location{};
  return {{{.format = FORMAT_OF<Ts>,
            .offset = replace(offset, offset + sizeof(Ts)),
            .shaderLocation = location++}...}};
}

template <typename... Ts>
constexpr auto ATTRIBS = attribs<Ts...>();

template <typename... Ts>
constexpr WGPUVertexBufferLayout LAYOUT{
  .arrayStride = (sizeof(Ts) + ...),
  .stepMode = WGPUVertexStepMode_Vertex,
  .attributeCount = sizeof...(Ts),
  .attributes = ATTRIBS<Ts...>.data(),
};
