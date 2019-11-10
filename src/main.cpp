#include <assets/shader.hpp>
#include <assets/vertex_array.hpp>
#include <iostream>
#include <logger.hpp>
#include <opengl.hpp>
#include <random>

void run(const std::string &assets_path) {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "Main display window.", nullptr, nullptr);
  if (window == nullptr) {
    critical("Failed to create a window.");
    exit(1);
  }
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, [](GLFWwindow *window, int key, [[maybe_unused]] int scan_code, int action, [[maybe_unused]] int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
  });

  if (!gladLoadGL(glfwGetProcAddress)) {
    critical("Failed to load OpenGL.");
    exit(1);
  }

  std::vector<float> points{1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 1.0f};
  std::vector<std::int32_t> index{0, 1, 2, 0, 2, 3};

  liu::shader shader(assets_path, "default");
  liu::vertex_array array;
  shader.apply([&]() {
    array.apply([&]() {
      array.add_buffer(liu::buffer_target::ARRAY_BUFFER);
      array.bind_vertex_buffer<float>(liu::buffer_target::ARRAY_BUFFER, points.begin(), points.end(), liu::buffer_usage::STATIC_DRAW);
      array.add_buffer(liu::buffer_target::ELEMENT_ARRAY_BUFFER);
      array.bind_vertex_buffer<std::int32_t>(liu::buffer_target::ELEMENT_ARRAY_BUFFER, index.begin(), index.end(), liu::buffer_usage::STATIC_DRAW);
      shader.activate_attribute("position", 2, liu::array_type::FLOAT, false, 4 * sizeof(float), 0);
      shader.activate_attribute("coordinate", 2, liu::array_type::FLOAT, false, 4 * sizeof(float), 2 * sizeof(float));
    });
  });

  uint8_t texture[600 * 800 * 3];
  std::random_device device;
  std::uniform_int_distribution<uint8_t> uniform(0, 255);
  std::uint32_t texture_id;
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  std::generate(std::begin(texture), std::end(texture), [&]() -> uint8_t { return uniform(device); });
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 600, 800, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    shader.apply([&]() { array.apply([&]() { array.draw_index(liu::draw_mode::TRIANGLES, 6, liu::index_type::UNSIGNED_INT); }); });
    glfwSwapBuffers(window);
    std::uint32_t error_code = glGetError();
    if (error_code != GL_NO_ERROR) {
      warn("OpenGL catches an error: {}", error_code);
    }
  }
}

std::string parse_arguments(int argc, char *argv[]);

int main(int argc, char *argv[]) {
  std::string assets_path = parse_arguments(argc, argv);
  init_logger();
  try {
    run(assets_path);
  } catch (const std::exception &e) {
    critical("Exception caused: {}", e.what());
  }
  info("Program exited.");
}
