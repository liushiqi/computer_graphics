#include "assets/shader.hpp"
#include "logger.hpp"
#include <fstream>
#include <iostream>

std::ostream &liu::operator<<(std::ostream &out, const liu::attribute_type &type) {
#define print_string(name)                                                                                                                           \
  case name:                                                                                                                                         \
    out << #name;                                                                                                                                    \
    break
  switch (type) {
    print_string(liu::attribute_type::FLOAT);
    print_string(liu::attribute_type::FLOAT_VEC2);
    print_string(liu::attribute_type::FLOAT_VEC3);
    print_string(liu::attribute_type::FLOAT_VEC4);
    print_string(liu::attribute_type::FLOAT_MAT2);
    print_string(liu::attribute_type::FLOAT_MAT3);
    print_string(liu::attribute_type::FLOAT_MAT4);
    print_string(liu::attribute_type::FLOAT_MAT2x3);
    print_string(liu::attribute_type::FLOAT_MAT2x4);
    print_string(liu::attribute_type::FLOAT_MAT3x2);
    print_string(liu::attribute_type::FLOAT_MAT3x4);
    print_string(liu::attribute_type::FLOAT_MAT4x2);
    print_string(liu::attribute_type::FLOAT_MAT4x3);
    print_string(liu::attribute_type::INT);
    print_string(liu::attribute_type::INT_VEC2);
    print_string(liu::attribute_type::INT_VEC3);
    print_string(liu::attribute_type::INT_VEC4);
    print_string(liu::attribute_type::UNSIGNED_INT);
    print_string(liu::attribute_type::UNSIGNED_INT_VEC2);
    print_string(liu::attribute_type::UNSIGNED_INT_VEC3);
    print_string(liu::attribute_type::UNSIGNED_INT_VEC4);
    print_string(liu::attribute_type::DOUBLE);
    print_string(liu::attribute_type::DOUBLE_VEC2);
    print_string(liu::attribute_type::DOUBLE_VEC3);
    print_string(liu::attribute_type::DOUBLE_VEC4);
    print_string(liu::attribute_type::DOUBLE_MAT2);
    print_string(liu::attribute_type::DOUBLE_MAT3);
    print_string(liu::attribute_type::DOUBLE_MAT4);
    print_string(liu::attribute_type::DOUBLE_MAT2x3);
    print_string(liu::attribute_type::DOUBLE_MAT2x4);
    print_string(liu::attribute_type::DOUBLE_MAT3x2);
    print_string(liu::attribute_type::DOUBLE_MAT3x4);
    print_string(liu::attribute_type::DOUBLE_MAT4x2);
    print_string(liu::attribute_type::DOUBLE_MAT4x3);
  }
  return out;
#undef print_string
}

std::ostream &liu::operator<<(std::ostream &out, const liu::array_type &type) {
#define print_string(name)                                                                                                                           \
  case name:                                                                                                                                         \
    out << #name;                                                                                                                                    \
    break
  switch (type) {
    print_string(liu::array_type::BYTE);
    print_string(liu::array_type::UNSIGNED_BYTE);
    print_string(liu::array_type::SHORT);
    print_string(liu::array_type::UNSIGNED_SHORT);
    print_string(liu::array_type::INT);
    print_string(liu::array_type::UNSIGNED_INT);
    print_string(liu::array_type::HALF_FLOAT);
    print_string(liu::array_type::FLOAT);
    print_string(liu::array_type::DOUBLE);
    print_string(liu::array_type::FIXED);
    print_string(liu::array_type::INT_2_10_10_10_REV);
    print_string(liu::array_type::UNSIGNED_INT_2_10_10_10_REV);
    print_string(liu::array_type::UNSIGNED_INT_10F_11F_11F_REV);
  }
  return out;
#undef print_string
}

void liu::shader::build_indices() {
  unsigned attribute_count, attribute_max_length;
  glGetProgramiv(program_id, GL_ACTIVE_ATTRIBUTES, reinterpret_cast<std::int32_t *>(&attribute_count));
  glGetProgramiv(program_id, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, reinterpret_cast<std::int32_t *>(&attribute_max_length));
  trace("There are {} attributes in the program {}, maximum length is {}", attribute_count, name, attribute_max_length);
  std::vector<char> attribute_name_vector(static_cast<std::vector<char>::size_type>(attribute_max_length + 1));
  for (unsigned i = 0; i < attribute_count; i++) {
    std::int32_t length, size;
    liu::attribute_type type;
    glGetActiveAttrib(program_id, (std::uint32_t)i, static_cast<GLsizei>(attribute_max_length), &length, &size,
                      reinterpret_cast<std::uint32_t *>(&type), attribute_name_vector.data());
    std::int32_t position = glGetAttribLocation(program_id, attribute_name_vector.data());
    std::string attribute_name(attribute_name_vector.begin(), attribute_name_vector.begin() + length);
    attribute_indices[attribute_name] = position;
    trace("Found attribute {}, type: {}, name: {} in program {}", size, type, attribute_name, name);
  }
  std::uint32_t uniform_count, uniform_max_length;
  glGetProgramiv(program_id, GL_ACTIVE_UNIFORMS, reinterpret_cast<std::int32_t *>(&uniform_count));
  glGetProgramiv(program_id, GL_ACTIVE_UNIFORM_MAX_LENGTH, reinterpret_cast<std::int32_t *>(&uniform_max_length));
  SPDLOG_TRACE("There are {} attributes in the program {}, maximum length is {}", uniform_count, name, uniform_max_length);
  std::vector<char> uniform_name_vector(static_cast<std::vector<char>::size_type>(uniform_max_length + 1));
  for (unsigned i = 0; i < uniform_count; i++) {
    std::int32_t length, size;
    liu::attribute_type type;
    glGetActiveUniform(program_id, (std::uint32_t)i, static_cast<GLsizei>(uniform_max_length), &length, &size,
                       reinterpret_cast<std::uint32_t *>(&type), uniform_name_vector.data());
    std::int32_t position = glGetUniformLocation(program_id, uniform_name_vector.data());
    std::string uniform_name(uniform_name_vector.begin(), uniform_name_vector.begin() + length);
    uniform_indices[uniform_name] = position;
    trace("Found uniform {}, type: {}, name: {} in program {}", i, type, uniform_name, name);
  }
  info("Shader {} indices build succeeded.", name);
}

liu::shader::shader(const std::string &asset_root_path, const std::string &name) : name(name) {
  std::string vertex_file, fragment_file;
  std::uint32_t vertex_shader_id, fragment_shader_id;
  std::int32_t result;
  std::int32_t log_length;
  try {
    std::string vertex_file_path, fragment_file_path;
    if (*asset_root_path.rbegin() == '/') {
      vertex_file_path = asset_root_path + "shader/" + name + ".vsh";
      fragment_file_path = asset_root_path + "shader/" + name + ".fsh";
    } else {
      vertex_file_path = asset_root_path + "/shader/" + name + ".vsh";
      fragment_file_path = asset_root_path + "/shader/" + name + ".fsh";
    }
    std::ifstream vin(vertex_file_path), fin(fragment_file_path);
    std::copy(std::istreambuf_iterator<char>(vin), std::istreambuf_iterator<char>(), std::back_inserter(vertex_file));
    std::copy(std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>(), std::back_inserter(fragment_file));
    debug("Shader file {} and {} read succeed.", vertex_file_path, fragment_file_path);
  } catch (const std::ifstream::failure &e) {
    critical("Error: Shader read failed: {}", e.what());
    exit(1);
  }

  vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
  const char *vertex_content = vertex_file.c_str();
  glShaderSource(vertex_shader_id, 1, &vertex_content, nullptr);
  glCompileShader(vertex_shader_id);
  glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &result);
  glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &log_length);
  if (result != GL_TRUE) {
    std::vector<char> log_content(static_cast<std::vector<char>::size_type>(log_length + 1));
    glGetShaderInfoLog(vertex_shader_id, log_length, nullptr, log_content.data());
    std::string log(log_content.begin(), log_content.end());
    critical("Vertex shader compile failed with error:\n{}", log);
    exit(1);
  } else if (log_length != 0) {
    std::vector<char> log_content(static_cast<std::vector<char>::size_type>(log_length + 1));
    glGetShaderInfoLog(vertex_shader_id, log_length, nullptr, log_content.data());
    std::string log(log_content.begin(), log_content.end());
    warn("Vertex shader compile finished with warning:\n{}", log);
  }
  debug("Vertex shader compile succeeded.");

  fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
  const char *fragment_content = fragment_file.c_str();
  glShaderSource(fragment_shader_id, 1, &fragment_content, nullptr);
  glCompileShader(fragment_shader_id);
  glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &result);
  glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, &log_length);
  if (result != GL_TRUE) {
    std::vector<char> log_content(static_cast<std::vector<char>::size_type>(log_length + 1));
    glGetShaderInfoLog(fragment_shader_id, log_length, nullptr, log_content.data());
    std::string log(log_content.begin(), log_content.end());
    critical("Fragment shader compile failed with error:\n{}", log);
    exit(1);
  } else if (log_length != 0) {
    std::vector<char> log_content(static_cast<std::vector<char>::size_type>(log_length + 1));
    glGetShaderInfoLog(fragment_shader_id, log_length, nullptr, log_content.data());
    std::string log(log_content.begin(), log_content.end());
    warn("Fragment shader compile finished with warning:\n{}", log);
  }
  debug("Fragment shader compile succeeded.");

  program_id = glCreateProgram();
  glAttachShader(program_id, vertex_shader_id);
  glAttachShader(program_id, fragment_shader_id);
  glLinkProgram(program_id);
  glGetProgramiv(program_id, GL_LINK_STATUS, &result);
  glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);
  if (result != GL_TRUE) {
    std::vector<char> log_content(static_cast<std::vector<char>::size_type>(log_length + 1));
    glGetProgramInfoLog(program_id, 1024, &log_length, log_content.data());
    std::string log(log_content.begin(), log_content.end());
    critical("Shader program link failed with error:\n{}", log);
    exit(1);
  } else if (log_length != 0) {
    std::vector<char> log_content(static_cast<std::vector<char>::size_type>(log_length + 1));
    glGetShaderInfoLog(fragment_shader_id, log_length, nullptr, log_content.data());
    std::string log(log_content.begin(), log_content.end());
    warn("Shader program link finished with warning:\n{}", log);
  }
  debug("Shader link succeeded.");

  glDetachShader(program_id, vertex_shader_id);
  glDetachShader(program_id, fragment_shader_id);
  glDeleteShader(vertex_shader_id);
  glDeleteShader(fragment_shader_id);
  info("Shader {} load succeeded.", name);
  build_indices();
}

void liu::shader::apply(const std::function<void()> &callback) const {
  active();
  callback();
  inactive();
}

std::int32_t liu::shader::get_attribute_index(const std::string &attribute_name) const {
  try {
    return attribute_indices.at(attribute_name);
  } catch (const std::out_of_range &e) {
    critical("Requesting non-existing attribute {} in shader {}", attribute_name, name);
    throw e;
  }
}

std::int32_t liu::shader::get_uniform_index(const std::string &uniform_name) const {
  try {
    return uniform_indices.at(uniform_name);
  } catch (const std::out_of_range &e) {
    critical("Requesting non-existing uniform {} in shader {}", uniform_name, name);
    throw e;
  }
}

void liu::shader::activate_attribute(const std::string &attrib_name, int count, liu::array_type type, bool do_normalize, int stride, int offset) {
  glVertexAttribPointer(static_cast<std::uint32_t>(get_attribute_index(attrib_name)), count, static_cast<std::uint32_t>(type),
                        static_cast<std::uint8_t>(do_normalize), stride, reinterpret_cast<void *>(offset));
  glEnableVertexAttribArray(static_cast<std::uint32_t>(get_attribute_index(attrib_name)));
}

void liu::shader::active() const { glUseProgram(program_id); }

void liu::shader::inactive() { glUseProgram(0); }

void liu::shader::apply(const std::function<void()> &callback) {
  active();
  callback();
  inactive();
}
