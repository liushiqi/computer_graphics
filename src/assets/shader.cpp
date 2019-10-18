#include "shader.h"
#include <fstream>
#include <iostream>

liu::shader::shader(const std::string &asset_root_path, const std::string &name) {
  std::string vertex_file, fragment_file;
  GLuint vertex_shader_id, fragment_shader_id;
  GLint result;
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
  } catch (const std::ifstream::failure &e) {
    std::cerr << "Error: Shader read failed." << std::endl;
    exit(1);
  }
  vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
  const GLchar *vertex_content = vertex_file.c_str();
  glShaderSource(vertex_shader_id, 1, &vertex_content, nullptr);
  glCompileShader(vertex_shader_id);
  glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &result);
  if (result != GL_TRUE) {
    int log_length;
    char log_content[1024];
    glGetShaderInfoLog(vertex_shader_id, 1024, &log_length, log_content);
    std::cerr << "Vertex shader compile failed with error:" << std::endl << log_content;
    exit(1);
  }
  fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
  const GLchar *fragment_content = fragment_file.c_str();
  glShaderSource(fragment_shader_id, 1, &fragment_content, nullptr);
  glCompileShader(fragment_shader_id);
  glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &result);
  if (result != GL_TRUE) {
    int log_length;
    char log_content[1024];
    glGetShaderInfoLog(fragment_shader_id, 1024, &log_length, log_content);
    std::cerr << "Fragment shader compile failed with error:" << std::endl << log_content;
    exit(1);
  }
  program_id = glCreateProgram();
  glAttachShader(program_id, vertex_shader_id);
  glAttachShader(program_id, fragment_shader_id);
  glLinkProgram(program_id);
  glGetProgramiv(program_id, GL_LINK_STATUS, &result);
  if (result != GL_TRUE) {
    int log_length;
    char log_content[1024];
    glGetProgramInfoLog(program_id, 1024, &log_length, log_content);
    std::cerr << "Shader program link failed with error:" << std::endl << log_content;
    exit(1);
  }
  glDeleteShader(vertex_shader_id);
  glDeleteShader(fragment_shader_id);
}

void liu::shader::active() { glUseProgram(program_id); }

void liu::shader::inactive() { glUseProgram(0); }

liu::shader::~shader() { glDeleteProgram(program_id); }
