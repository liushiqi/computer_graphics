#include "assets/shader.h"
#include <fstream>
#include <iostream>

std::ostream &liu::operator<<(std::ostream &out, const liu::attribute_type &type) {
#define print_string(name)                                                                                             \
  case name:                                                                                                           \
    out << #name;                                                                                                      \
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

void liu::shader::build_indices() {
  unsigned attribute_count, attribute_max_length;
  glGetProgramiv(program_id, GL_ACTIVE_ATTRIBUTES, reinterpret_cast<GLint *>(&attribute_count));
  glGetProgramiv(program_id, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, reinterpret_cast<GLint *>(&attribute_max_length));
  SPDLOG_TRACE("There are {} attributes in the program {}, maximum length is {}", attribute_count, name,
               attribute_max_length);
  std::vector<char> attribute_name_vector(static_cast<std::vector<char>::size_type>(attribute_max_length + 1));
  for (unsigned i = 0; i < attribute_count; i++) {
    int length, size;
    liu::attribute_type type;
    glGetActiveAttrib(program_id, (GLuint)i, static_cast<GLsizei>(attribute_max_length), &length, &size,
                      reinterpret_cast<GLenum *>(&type), attribute_name_vector.data());
    std::string attribute_name(attribute_name_vector.begin(), attribute_name_vector.begin() + length);
    attribute_indices[attribute_name] = i;
    SPDLOG_TRACE("Found attribute {}, type: {}, name: {} in program {}", i, type, attribute_name, name);
  }
  unsigned uniform_count, uniform_max_length;
  glGetProgramiv(program_id, GL_ACTIVE_UNIFORMS, reinterpret_cast<GLint *>(&uniform_count));
  glGetProgramiv(program_id, GL_ACTIVE_UNIFORM_MAX_LENGTH, reinterpret_cast<GLint *>(&uniform_max_length));
  SPDLOG_TRACE("There are {} attributes in the program {}, maximum length is {}", uniform_count, name,
               uniform_max_length);
  std::vector<char> uniform_name_vector(static_cast<std::vector<char>::size_type>(uniform_max_length + 1));
  for (unsigned i = 0; i < uniform_count; i++) {
    int length, size;
    liu::attribute_type type;
    glGetActiveUniform(program_id, (GLuint)i, static_cast<GLsizei>(uniform_max_length), &length, &size,
                       reinterpret_cast<GLenum *>(&type), uniform_name_vector.data());
    std::string uniform_name(uniform_name_vector.begin(), uniform_name_vector.begin() + length);
    uniform_indices[uniform_name] = i;
    SPDLOG_TRACE("Found uniform {}, type: {}, name: {} in program {}", i, type, uniform_name, name);
  }
  SPDLOG_INFO("Shader {} indices build succeeded.", name);
}

liu::shader::shader(const std::string &asset_root_path, const std::string &name) : name(name) {
  std::string vertex_file, fragment_file;
  GLuint vertex_shader_id, fragment_shader_id;
  GLint result;
  GLint log_length;
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
    SPDLOG_DEBUG("Shader file {} and {} read succeed.", vertex_file_path, fragment_file_path);
  } catch (const std::ifstream::failure &e) {
    SPDLOG_CRITICAL("Error: Shader read failed: {}", e.what());
    exit(1);
  }

  vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
  const GLchar *vertex_content = vertex_file.c_str();
  glShaderSource(vertex_shader_id, 1, &vertex_content, nullptr);
  glCompileShader(vertex_shader_id);
  glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &result);
  glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &log_length);
  if (result != GL_TRUE) {
    std::vector<char> log_content(static_cast<std::vector<char>::size_type>(log_length + 1));
    glGetShaderInfoLog(vertex_shader_id, log_length, nullptr, log_content.data());
    std::string log(log_content.begin(), log_content.end());
    spdlog::critical("Vertex shader compile failed with error:\n{}", log);
    exit(1);
  } else if (log_length != 0) {
    std::vector<char> log_content(static_cast<std::vector<char>::size_type>(log_length + 1));
    glGetShaderInfoLog(vertex_shader_id, log_length, nullptr, log_content.data());
    std::string log(log_content.begin(), log_content.end());
    SPDLOG_WARN("Vertex shader compile finished with warning:\n{}", log);
  }
  SPDLOG_DEBUG("Vertex shader compile succeeded.");

  fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
  const GLchar *fragment_content = fragment_file.c_str();
  glShaderSource(fragment_shader_id, 1, &fragment_content, nullptr);
  glCompileShader(fragment_shader_id);
  glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &result);
  glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, &log_length);
  if (result != GL_TRUE) {
    std::vector<char> log_content(static_cast<std::vector<char>::size_type>(log_length + 1));
    glGetShaderInfoLog(fragment_shader_id, log_length, nullptr, log_content.data());
    std::string log(log_content.begin(), log_content.end());
    SPDLOG_CRITICAL("Fragment shader compile failed with error:\n{}", log);
    exit(1);
  } else if (log_length != 0) {
    std::vector<char> log_content(static_cast<std::vector<char>::size_type>(log_length + 1));
    glGetShaderInfoLog(fragment_shader_id, log_length, nullptr, log_content.data());
    std::string log(log_content.begin(), log_content.end());
    SPDLOG_WARN("Fragment shader compile finished with warning:\n{}", log);
  }
  SPDLOG_DEBUG("Fragment shader compile succeeded.");

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
    SPDLOG_CRITICAL("Shader program link failed with error:\n{}", log);
    exit(1);
  } else if (log_length != 0) {
    std::vector<char> log_content(static_cast<std::vector<char>::size_type>(log_length + 1));
    glGetShaderInfoLog(fragment_shader_id, log_length, nullptr, log_content.data());
    std::string log(log_content.begin(), log_content.end());
    SPDLOG_WARN("Shader program link finished with warning:\n{}", log);
  }
  SPDLOG_DEBUG("Shader link succeeded.");

  glDetachShader(program_id, vertex_shader_id);
  glDetachShader(program_id, fragment_shader_id);
  glDeleteShader(vertex_shader_id);
  glDeleteShader(fragment_shader_id);
  SPDLOG_INFO("Shader {} load succeeded.", name);
  build_indices();
}

void liu::shader::apply(const std::function<void()> &callback) const {
  active();
  callback();
  inactive();
}

unsigned int liu::shader::get_attribute_index(const std::string &attribute_name) const {
  if (attribute_indices.count(attribute_name) == 1)
    return attribute_indices.at(attribute_name);
  else {
    SPDLOG_CRITICAL("Requesting non-existing attribute {} in shader {}", attribute_name, name);
    throw std::runtime_error("Requesting non-existing attribute.");
  }
}

unsigned int liu::shader::get_uniform_index(const std::string &uniform_name) const {
  if (uniform_indices.count(uniform_name) == 1)
    return uniform_indices.at(uniform_name);
  else {
    SPDLOG_CRITICAL("Requesting non-existing uniform {} in shader {}", uniform_name, name);
    throw std::runtime_error("Requesting non-existing uniform.");
  }
}

void liu::shader::active() const { glUseProgram(program_id); }

void liu::shader::inactive() { glUseProgram(0); }

void liu::shader::apply(const std::function<void()> &callback) {
  active();
  callback();
  inactive();
}
