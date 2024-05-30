#include <string>
#include "prompt.hpp"

namespace prompt {

std::string hello(const std::string name)
{
  return "Hello " + name + "!";
}

}