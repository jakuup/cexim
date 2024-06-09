#ifndef __PROMPT_HPP__
#define __PROMPT_HPP__

#include <string>
#include <ncurses.h>

namespace prompt {

void setCompleterCallback();

void setExecutorCallback();

/// @brief Runs prompt loop on ncurses window
/// @param win ncurses window
/// @param prompt prompt test, e.g. "command>"
/// @param intro introduction text printed once the function is called
/// @return true if the loop exits normally, otherwise false
bool runLoop(WINDOW *_window, const std::string &_prompt, const std::string &_intro);

}

#endif