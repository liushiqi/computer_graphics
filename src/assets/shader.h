#pragma once

#include <glad/gl.h>
#include <string>

namespace liu {
class shader {
private:
  GLuint program_id;

public:
  shader(const std::string &asset_root_path, const std::string &name);

  void active();

  static void inactive();

  ~shader();
};
} // namespace liu
