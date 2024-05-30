#ifndef __PROMPT_HPP__
#define __PROMPT_HPP__

#include <string>

namespace prompt {

/// @brief Generates a hello message
/// @param name name of the caller
/// @return hello message string
std::string hello(const std::string name);

}

#endif