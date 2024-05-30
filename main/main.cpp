#include <iostream>
#include "prompt.hpp"

int main()
{
  std::cout << prompt::hello("main") << std::endl;
  return 0;
}