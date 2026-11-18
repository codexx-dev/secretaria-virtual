#include "setup.h"
#include <iostream>



int main() {
  Setup setup;

  setup.install();
  std::cout << "Pressione Enter para encerrar.\n";
  std::cin.get();

  return 0;
}