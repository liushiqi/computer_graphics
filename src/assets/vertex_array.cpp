#include "assets/vertex_array.hpp"

std::ostream &liu::operator<<(std::ostream &out, const draw_mode &type) {
#define print_string(name)                                                                                             \
  case name:                                                                                                           \
    out << #name;                                                                                                      \
    break
  switch (type) {
    print_string(liu::draw_mode::POINTS);
    print_string(liu::draw_mode::LINE_STRIP);
    print_string(liu::draw_mode::LINE_LOOP);
    print_string(liu::draw_mode::LINES);
    print_string(liu::draw_mode::LINE_STRIP_ADJACENCY);
    print_string(liu::draw_mode::LINES_ADJACENCY);
    print_string(liu::draw_mode::TRIANGLE_STRIP);
    print_string(liu::draw_mode::TRIANGLE_FAN);
    print_string(liu::draw_mode::TRIANGLES);
    print_string(liu::draw_mode::TRIANGLE_STRIP_ADJACENCY);
    print_string(liu::draw_mode::TRIANGLES_ADJACENCY);
    print_string(liu::draw_mode::PATCHES);
  }
  return out;
#undef print_string
}

std::ostream &liu::operator<<(std::ostream &out, const index_type &type) {
#define print_string(name)                                                                                             \
  case name:                                                                                                           \
    out << #name;                                                                                                      \
    break
  switch (type) {
    print_string(liu::index_type::UNSIGNED_INT);
    print_string(liu::index_type::UNSIGNED_BYTE);
    print_string(liu::index_type::UNSIGNED_SHORT);
  }
  return out;
#undef print_string
}

liu::vertex_array::vertex_array() : vertex_array_id(0) { glGenVertexArrays(1, &vertex_array_id); }

void liu::vertex_array::add_buffer(liu::buffer_target target) {
  if (buffers.count(target) == 0) {
    buffers.emplace(target, liu::vertex_buffer(target));
    SPDLOG_DEBUG("Added a buffer {}.", target);
  } else {
    SPDLOG_WARN("Trying to add a existing buffer {}", target);
  }
}

void liu::vertex_array::active() const { glBindVertexArray(vertex_array_id); }

void liu::vertex_array::inactive() { glBindVertexArray(0); }

void liu::vertex_array::draw(liu::draw_mode mode, int from, int count) {
  glDrawArrays(static_cast<GLenum>(mode), from, count);
}

void liu::vertex_array::draw_index(liu::draw_mode mode, int count, liu::index_type type) {
  if (buffers.count(liu::buffer_target::ELEMENT_ARRAY_BUFFER) != 0) {
    glDrawElements(static_cast<GLenum>(mode), count, static_cast<GLenum>(type), nullptr);
  } else {
    SPDLOG_WARN("Invalid draw call to draw_index with ELEMENT_ARRAY_BUFFER not enabled.");
  }
}

void liu::vertex_array::apply(const std::function<void()> &callback) {
  active();
  callback();
  inactive();
}

void liu::vertex_array::apply(const std::function<void()> &callback) const {
  active();
  callback();
  inactive();
}
