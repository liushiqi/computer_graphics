#pragma once

#include <cstdint>
#include <glad/gl.h>
#include <iostream>
#include <vector>

namespace liu {
enum class buffer_target {
  ARRAY_BUFFER = GL_ARRAY_BUFFER,
  ATOMIC_COUNTER_BUFFER = GL_ATOMIC_COUNTER_BUFFER,
  COPY_READ_BUFFER = GL_COPY_READ_BUFFER,
  COPY_WRITE_BUFFER = GL_COPY_WRITE_BUFFER,
  DISPATCH_INDIRECT_BUFFER = GL_DISPATCH_INDIRECT_BUFFER,
  DRAW_INDIRECT_BUFFER = GL_DRAW_INDIRECT_BUFFER,
  ELEMENT_ARRAY_BUFFER = GL_ELEMENT_ARRAY_BUFFER,
  PIXEL_PACK_BUFFER = GL_PIXEL_PACK_BUFFER,
  PIXEL_UNPACK_BUFFER = GL_PIXEL_UNPACK_BUFFER,
  QUERY_BUFFER = GL_QUERY_BUFFER,
  SHADER_STORAGE_BUFFER = GL_SHADER_STORAGE_BUFFER,
  TEXTURE_BUFFER = GL_TEXTURE_BUFFER,
  TRANSFORM_FEEDBACK_BUFFER = GL_TRANSFORM_FEEDBACK_BUFFER,
  UNIFORM_BUFFER = GL_UNIFORM_BUFFER,
};

std::ostream &operator<<(std::ostream &out, const buffer_target &type);

enum class buffer_usage {
  STREAM_DRAW = GL_STREAM_DRAW,
  STREAM_READ = GL_STREAM_READ,
  STREAM_COPY = GL_STREAM_COPY,
  STATIC_DRAW = GL_STATIC_DRAW,
  STATIC_READ = GL_STATIC_READ,
  STATIC_COPY = GL_STATIC_COPY,
  DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
  DYNAMIC_READ = GL_DYNAMIC_READ,
  DYNAMIC_COPY = GL_DYNAMIC_COPY,
};

std::ostream &operator<<(std::ostream &out, const buffer_usage &type);

class vertex_buffer {
private:
  std::uint32_t vertex_buffer_id;
  buffer_target type;

public:
  explicit vertex_buffer(buffer_target type);

  template <class T, class InputIterator> void bind_data(InputIterator begin, InputIterator end, liu::buffer_usage usage) {
    std::vector<T> data;
    std::copy(begin, end, std::back_inserter(data));
    glBufferData(static_cast<std::uint32_t>(type), static_cast<std::intptr_t>(data.size() * sizeof(T)), data.data(),
                 static_cast<std::uint32_t>(usage));
  }
};
} // namespace liu
