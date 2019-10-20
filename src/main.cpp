#include "assets/shader.h"
#include "assets/vertex_array.h"
#include "logger.h"
#include <GLFW/glfw3.h>
#include <boost/program_options.hpp>
#include <glad/gl.h>
#include <iostream>

std::string parse_arguments(int argc, char *argv[]) {
  std::string assets_path;
  boost::program_options::options_description options("A program for computer graphics class");
  try {
    options.add_options()("help,h", "Display this help message.")("assets,a", boost::program_options::value<std::string>()->default_value("./assets"),
                                                                  "the path to assets directory.");
    boost::program_options::variables_map map;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, options), map);
    boost::program_options::notify(map);
    if (map.count("help")) {
      std::cout << options << std::endl;
      exit(0);
    }
    if (map.count("assets")) {
      assets_path = map["assets"].as<std::string>();
    }
  } catch (const boost::program_options::error &error) {
    std::cerr << "arguments parse failed: " << error.what() << std::endl << options;
    exit(1);
  }
  return assets_path;
}

int main(int argc, char *argv[]) {
  std::string assets_path = parse_arguments(argc, argv);
  init_logger();
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "Main display window.", nullptr, nullptr);
  if (window == nullptr) {
    SPDLOG_CRITICAL("Failed to create a window.");
    exit(1);
  }
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, [](GLFWwindow *window, int key, [[maybe_unused]] int scan_code, int action, [[maybe_unused]] int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
  });

  if (!gladLoadGL(glfwGetProcAddress)) {
    SPDLOG_CRITICAL("Failed to load OpenGL.");
    exit(1);
  }

  std::vector<GLfloat> points{0.5f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f};
  std::vector<GLint> index{0, 1, 3, 1, 2, 3};

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  liu::shader shader(assets_path, "default");
  liu::vertex_array array;
  shader.apply([&]() {
    array.apply([&]() {
      array.add_buffer(liu::buffer_target::ARRAY_BUFFER);
      array.bind_vertex_buffer<GLfloat>(liu::buffer_target::ARRAY_BUFFER, points.begin(), points.end(), liu::buffer_usage::STATIC_DRAW);
      array.add_buffer(liu::buffer_target::ELEMENT_ARRAY_BUFFER);
      array.bind_vertex_buffer<GLint>(liu::buffer_target::ELEMENT_ARRAY_BUFFER, index.begin(), index.end(), liu::buffer_usage::STATIC_DRAW);
      shader.activate_attribute("position", 2, liu::array_type::FLOAT, false, 3 * sizeof(GLfloat), 0);
    });
  });

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    shader.apply([&]() { array.apply([&]() { array.draw_index(liu::draw_mode::TRIANGLES, 6, liu::index_type::UNSIGNED_INT); }); });
    glfwSwapBuffers(window);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
      SPDLOG_WARN("OpenGL catches an error: {}", error);
    }
  }
  return 0;
}
