#include "assets/vertex_buffer.hpp"

std::ostream &liu::operator<<(std::ostream &out, const buffer_target &type) {
#define print_string(name)                                                                                                                           \
  case name:                                                                                                                                         \
    out << #name;                                                                                                                                    \
    break
  switch (type) {
    print_string(liu::buffer_target::ARRAY_BUFFER);
    print_string(liu::buffer_target::ATOMIC_COUNTER_BUFFER);
    print_string(liu::buffer_target::COPY_READ_BUFFER);
    print_string(liu::buffer_target::COPY_WRITE_BUFFER);
    print_string(liu::buffer_target::DISPATCH_INDIRECT_BUFFER);
    print_string(liu::buffer_target::DRAW_INDIRECT_BUFFER);
    print_string(liu::buffer_target::ELEMENT_ARRAY_BUFFER);
    print_string(liu::buffer_target::PIXEL_PACK_BUFFER);
    print_string(liu::buffer_target::PIXEL_UNPACK_BUFFER);
    print_string(liu::buffer_target::QUERY_BUFFER);
    print_string(liu::buffer_target::SHADER_STORAGE_BUFFER);
    print_string(liu::buffer_target::TEXTURE_BUFFER);
    print_string(liu::buffer_target::TRANSFORM_FEEDBACK_BUFFER);
    print_string(liu::buffer_target::UNIFORM_BUFFER);
  }
  return out;
#undef print_string
}

std::ostream &liu::operator<<(std::ostream &out, const buffer_usage &type) {
#define print_string(name)                                                                                                                           \
  case name:                                                                                                                                         \
    out << #name;                                                                                                                                    \
    break
  switch (type) {
    print_string(liu::buffer_usage::STREAM_DRAW);
    print_string(liu::buffer_usage::STREAM_READ);
    print_string(liu::buffer_usage::STREAM_COPY);
    print_string(liu::buffer_usage::STATIC_DRAW);
    print_string(liu::buffer_usage::STATIC_READ);
    print_string(liu::buffer_usage::STATIC_COPY);
    print_string(liu::buffer_usage::DYNAMIC_DRAW);
    print_string(liu::buffer_usage::DYNAMIC_READ);
    print_string(liu::buffer_usage::DYNAMIC_COPY);
  }
  return out;
#undef print_string
}

liu::vertex_buffer::vertex_buffer(liu::buffer_target type) : vertex_buffer_id(0), type(type) {
  glGenBuffers(1, &vertex_buffer_id);
  glBindBuffer(static_cast<std::uint32_t>(type), vertex_buffer_id);
}
