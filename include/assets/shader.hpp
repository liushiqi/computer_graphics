#pragma once

#include "vertex_array.hpp"
#include <cstdint>
#include "logger.h"
#include <functional>
#include <glad/gl.h>
#include <iostream>
#include <map>
#include <spdlog/fmt/ostr.h>
#include <string>

namespace liu {
enum class attribute_type : std::uint32_t {
  FLOAT = GL_FLOAT,
  FLOAT_VEC2 = GL_FLOAT_VEC2,
  FLOAT_VEC3 = GL_FLOAT_VEC3,
  FLOAT_VEC4 = GL_FLOAT_VEC4,
  FLOAT_MAT2 = GL_FLOAT_MAT2,
  FLOAT_MAT3 = GL_FLOAT_MAT3,
  FLOAT_MAT4 = GL_FLOAT_MAT4,
  FLOAT_MAT2x3 = GL_FLOAT_MAT2x3,
  FLOAT_MAT2x4 = GL_FLOAT_MAT2x4,
  FLOAT_MAT3x2 = GL_FLOAT_MAT3x2,
  FLOAT_MAT3x4 = GL_FLOAT_MAT3x4,
  FLOAT_MAT4x2 = GL_FLOAT_MAT4x2,
  FLOAT_MAT4x3 = GL_FLOAT_MAT4x3,
  INT = GL_INT,
  INT_VEC2 = GL_INT_VEC2,
  INT_VEC3 = GL_INT_VEC3,
  INT_VEC4 = GL_INT_VEC4,
  UNSIGNED_INT = GL_UNSIGNED_INT,
  UNSIGNED_INT_VEC2 = GL_UNSIGNED_INT_VEC2,
  UNSIGNED_INT_VEC3 = GL_UNSIGNED_INT_VEC3,
  UNSIGNED_INT_VEC4 = GL_UNSIGNED_INT_VEC4,
  DOUBLE = GL_DOUBLE,
  DOUBLE_VEC2 = GL_DOUBLE_VEC2,
  DOUBLE_VEC3 = GL_DOUBLE_VEC3,
  DOUBLE_VEC4 = GL_DOUBLE_VEC4,
  DOUBLE_MAT2 = GL_DOUBLE_MAT2,
  DOUBLE_MAT3 = GL_DOUBLE_MAT3,
  DOUBLE_MAT4 = GL_DOUBLE_MAT4,
  DOUBLE_MAT2x3 = GL_DOUBLE_MAT2x3,
  DOUBLE_MAT2x4 = GL_DOUBLE_MAT2x4,
  DOUBLE_MAT3x2 = GL_DOUBLE_MAT3x2,
  DOUBLE_MAT3x4 = GL_DOUBLE_MAT3x4,
  DOUBLE_MAT4x2 = GL_DOUBLE_MAT4x2,
  DOUBLE_MAT4x3 = GL_DOUBLE_MAT4x3,
};

std::ostream &operator<<(std::ostream &out, const attribute_type &type);

enum class array_type : std::uint32_t {
  BYTE = GL_BYTE,
  UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
  SHORT = GL_SHORT,
  UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
  INT = GL_INT,
  UNSIGNED_INT = GL_UNSIGNED_INT,
  HALF_FLOAT = GL_HALF_FLOAT,
  FLOAT = GL_FLOAT,
  DOUBLE = GL_DOUBLE,
  FIXED = GL_FIXED,
  INT_2_10_10_10_REV = GL_INT_2_10_10_10_REV,
  UNSIGNED_INT_2_10_10_10_REV = GL_UNSIGNED_INT_2_10_10_10_REV,
  UNSIGNED_INT_10F_11F_11F_REV = GL_UNSIGNED_INT_10F_11F_11F_REV,
};

std::ostream &operator<<(std::ostream &out, const array_type &type);

class shader {
private:
  std::uint32_t program_id;
  std::string name;
  std::map<std::string, std::int32_t> attribute_indices;
  std::map<std::string, std::int32_t> uniform_indices;

  void build_indices();

public:
  shader(const std::string &asset_root_path, const std::string &name);

  [[nodiscard]] std::int32_t get_attribute_index(const std::string &attribute_name) const;

  [[nodiscard]] std::int32_t get_uniform_index(const std::string &uniform_name) const;

  void activate_attribute(const std::string &attrib_name, int count, liu::array_type type, bool do_normalize, int stride, int offset);

  void active() const;

  static void inactive();

  void apply(const std::function<void()> &callback);

  void apply(const std::function<void()> &callback) const;
};
} // namespace liu
