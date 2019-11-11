#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4267)
#pragma warning(disable : 4819)
#pragma warning(disable : 26444)
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#endif
#include <cxxopts.hpp>
#include <iostream>
#ifdef _MSC_VER
#pragma warning(pop)
#else
#pragma GCC diagnostic pop
#endif

std::string parse_arguments(int argc, char *argv[]) {
  std::string assets_path;
  cxxopts::Options options("computer-graphics", "A program for computer graphics class");
  try {
    options.add_options()("h,help", "Display this help message.")("a,assets", "the path to assets directory.", cxxopts::value<std::string>());
    auto result = options.parse(argc, argv);
    if (result.count("help")) {
      std::cout << options.help() << std::endl;
      exit(0);
    }
    if (result.count("assets")) {
      assets_path = result["assets"].as<std::string>();
    }
  } catch (const cxxopts::OptionSpecException &error) {
    std::cerr << "arguments parse failed: " << error.what() << std::endl << options.help();
    exit(1);
  }
  return assets_path;
}
