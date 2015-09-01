#include <iostream>
#include "codel.hpp"
#include "codel_table.hpp"
#include "color_block.hpp"
#include "interpreter.hpp"
#include "png.hpp"
#include "utils.hpp"
#include "visualize.hpp"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "usage: " << argv[0] << " [PNG FILENAME]" << std::endl;
    return EXIT_FAILURE;
  }
  try {
    const auto image = Image(argv[1]);
    visualize(image);
    const auto table = make_codel_table(image);
    auto network = color_block_network(table);
    Interpreter interpreter(std::move(network));
    interpreter.run();
  } catch (png::error& e) {
    std::cerr << e.what() << std::endl;
  }
  return EXIT_SUCCESS;
}
