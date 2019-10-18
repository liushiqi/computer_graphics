#include "assets/shader.h"
#include <GLFW/glfw3.h>
#include <boost/program_options.hpp>
#include <glad/gl.h>
#include <iostream>

int main(int argc, char *argv[]) {
  std::string assets_path;
  boost::program_options::options_description options("A program for computer graphics class");
  try {
    options.add_options()("help,h", "Display this help message.")(
        "assets,a", boost::program_options::value<std::string>()->default_value("./assets"),
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

  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "Main display window.", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scan_code, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
  });

  gladLoadGL(glfwGetProcAddress);

  liu::shader shader(assets_path, "default");
  shader.active();

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    glfwSwapBuffers(window);
  }
  return 0;
}
