#pragma once

#include <webgpu/webgpu.h>
#include <array>
#include <cstddef>
#include <cstdint>
#include <utility>
#include "glm/ext/vector_float3.hpp"

template <typename T>
struct FormatOf {};

template <>
struct FormatOf<glm::vec3> {
  static constexpr auto VALUE = WGPUVertexFormat_Float32x3;
};

template <>
struct FormatOf<float> {
  static constexpr auto VALUE = WGPUVertexFormat_Float32;
};

template <typename T>
inline constexpr auto FORMAT_OF = FormatOf<T>::VALUE;

template <typename T, typename U>
constexpr T replace(T& dst, U&& src) {
  const auto value = std::move(dst);
  dst = std::forward<U>(src);
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
inline constexpr auto ATTRIBS = attribs<Ts...>();

template <typename... Ts>
inline constexpr WGPUVertexBufferLayout LAYOUT{
    .arrayStride = (sizeof(Ts) + ...),
    .stepMode = WGPUVertexStepMode_Vertex,
    .attributeCount = sizeof...(Ts),
    .attributes = ATTRIBS<Ts...>.data(),
};
