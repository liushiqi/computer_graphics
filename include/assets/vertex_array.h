#pragma once

#include "assets/vertex_buffer.h"
#include "logger.h"
#include "shader.h"
#include <functional>
#include <glad/gl.h>
#include <map>
#include <spdlog/fmt/ostr.h>

namespace liu {
enum class draw_mode {
  POINTS = GL_POINTS,
  LINE_STRIP = GL_LINE_STRIP,
  LINE_LOOP = GL_LINE_LOOP,
  LINES = GL_LINES,
  LINE_STRIP_ADJACENCY = GL_LINE_STRIP_ADJACENCY,
  LINES_ADJACENCY = GL_LINES_ADJACENCY,
  TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
  TRIANGLE_FAN = GL_TRIANGLE_FAN,
  TRIANGLES = GL_TRIANGLES,
  TRIANGLE_STRIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY,
  TRIANGLES_ADJACENCY = GL_TRIANGLES_ADJACENCY,
  PATCHES = GL_PATCHES,
};

std::ostream &operator<<(std::ostream &out, const draw_mode &type);

enum class index_type {
  UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
  UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
  UNSIGNED_INT = GL_UNSIGNED_INT,
};

std::ostream &operator<<(std::ostream &out, const index_type &type);

enum class array_type {
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

class vertex_array {
private:
  GLuint vertex_array_id;
  std::map<buffer_target, vertex_buffer> buffers;

public:
  vertex_array();

  void add_buffer(buffer_target type);

  template <class T, class InputIterator>
  void bind_vertex_buffer(buffer_target type, InputIterator begin, InputIterator end, liu::buffer_usage usage) {
    if (buffers.count(type) == 1) {
      buffers.at(type).bind_data<T, InputIterator>(begin, end, usage);
    } else {
      SPDLOG_WARN("Buffer with type {} used before creating.", type);
    }
  }

  void active() const;

  static void inactive();

  static void activate_attribute(const liu::shader &shader, std::string attrib_name, int count, liu::array_type type,
                          bool do_normalize, int stride, int offset);

  static void draw(draw_mode mode, int from, int count);

  void draw_index(draw_mode mode, int count, index_type type);

  void apply(const std::function<void()> &callback);

  void apply(const std::function<void()> &callback) const;
};
} // namespace liu
