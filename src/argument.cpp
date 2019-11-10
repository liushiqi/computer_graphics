#include <boost/program_options.hpp>
#include <iostream>

std::string parse_arguments(int argc, char *argv[]) {
  std::string assets_path;
  boost::program_options::options_description options("A program for computer graphics class");
  try {
    options.add_options()("help,h", "Display this help message.")("assets,a", boost::program_options::value<std::string>()->required(),
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
