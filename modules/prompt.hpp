#ifndef __PROMPT_HPP__
#define __PROMPT_HPP__

#include <string>
#include <ncurses.h>

namespace prompt {

bool testMode = false;

void setCompleterCallback();

void setExecutorCallback();

/// @brief Runs prompt loop on ncurses window
/// @param win ncurses window
/// @param prompt prompt test, e.g. "command>"
/// @param intro introduction text printed once the function is called
/// @return true if the loop exits normally, otherwise false
bool runLoop(WINDOW *win, const std::string &prompt, const std::string &intro);

}

#endif