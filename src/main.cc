#include <cstdint>
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "query_tree.h"

void ExecuteCommand(syntacore::QueryTree &qt, char command, int64_t value) {
  switch (command) {
    case 'k': qt.Insert(value); break;
    case 'm': std::cout << qt.NthSmallest(value) << " "; break;
    case 'n': std::cout << qt.GetNumberOfSmallerValues(value) << " "; break;
    default:
      std::ostringstream error;
      error << "Unexpected command " << command;
      throw std::invalid_argument(error.str());
  }
}

int main() {
  syntacore::QueryTree qt;
  char command;
  int64_t value;
  while (std::cin >> command >> value) {
    try {
      ExecuteCommand(qt, command, value);
    } catch (std::exception &exc) { std::cerr << exc.what() << std::endl; }
  }
  return 0;
}
