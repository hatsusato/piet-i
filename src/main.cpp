#include <iostream>
#include "codel_table.hpp"
#include "interpreter.hpp"
#include "png.hpp"
#include "visualize.hpp"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "usage: " << argv[0] << " [PNG FILENAME]" << std::endl;
    return EXIT_FAILURE;
  }
  try {
    const auto image = Image(argv[1]);
    visualize(image);
    const auto table = make_codel_table(image, RegardUnknownAs::WHITE);
    if (!table[0][0].is_colour()) {
      std::cerr << "upper left corner must be coloured!" << std::endl;
      return EXIT_FAILURE;
    }
    Interpreter interpreter(table);
    interpreter.run();
  } catch (png::error& e) {
    std::cerr << e.what() << std::endl;
  }
  return EXIT_SUCCESS;
}
