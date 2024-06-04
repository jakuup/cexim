#include <iostream>
#include <string>
#include <ncurses.h>
#include "prompt.hpp"

int main(int argc, char **argv)
{
  std::string pmt = "test>";
  std::string itr = "This\n is\n  test.";

  if (argc < 2) {
    // this must not be running from ctest
    WINDOW *win = initscr();
    raw();
    noecho();
    keypad(stdscr, true);
    scrollok(stdscr, TRUE);
    prompt::testMode = true;
    bool result = prompt::runLoop(win, pmt, itr);
    endwin();
    std::cout << result << std::endl;
    return 1;
  }

  std::string arg1 = argv[1];
  if (arg1 == "null") {
    return prompt::runLoop(nullptr, pmt, itr) != false;
  }
}