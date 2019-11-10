#pragma once

#include "assets/vertex_buffer.hpp"
#include "logger.hpp"
#include "shader.hpp"
#include <functional>
#include <glad/gl.h>
#include <iostream>
#include <map>

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

class vertex_array {
private:
  std::uint32_t vertex_array_id;
  std::map<buffer_target, vertex_buffer> buffers;

public:
  vertex_array();

  void add_buffer(buffer_target type);

  template <class T, class InputIterator>
  void bind_vertex_buffer(buffer_target type, InputIterator begin, InputIterator end, liu::buffer_usage usage) {
    try {
      buffers.at(type).bind_data<T, InputIterator>(begin, end, usage);
    } catch (const std::out_of_range &e) {
      warn("Buffer with type {} used before creating.", type);
    }
  }

  void active() const;

  static void inactive();

  static void draw(draw_mode mode, int from, int count);

  void draw_index(draw_mode mode, int count, index_type type);

  void apply(const std::function<void()> &callback);

  void apply(const std::function<void()> &callback) const;
};
} // namespace liu
