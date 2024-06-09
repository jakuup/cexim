#include <iostream>
#include <ncurses.h>
#include <string>
#include <utility>
#include <vector>
#include "prompt.hpp"


int main(int argc, const char *argv[])
{
  std::vector<std::string> args;
  for (int i = 1; i < argc; ++i) {
    args.push_back(argv[i]);
  }

  WINDOW *term = nullptr;
  std::pair<int, std::string> width;
  std::pair<int, std::string> height;

  if (args.size() == 0) {
    initscr();
    width  = { getmaxx(stdscr), std::to_string(getmaxx(stdscr)) };
    height = { getmaxy(stdscr), std::to_string(getmaxy(stdscr)) };
    term = stdscr;
  }
  else {
    if (args[0] == "term") {
      try {
        width  = { std::stoi(args.at(1)), args.at(1) };
        height = { std::stoi(args.at(2)), args.at(2) };
      }
      catch (std::out_of_range) {
        return 1;
      }
      initscr();
      term = newwin(std::get<int>(height), std::get<int>(width), 0, 0);
    }
    else
    if (args[0] == "auto") {
      if (args[1] == "null") {
        return prompt::runLoop(nullptr, "", "") != false;
      }
    }
  }

  std::string pmt = "test>";
  std::string itr = "This is test on terminal geometry:\n";
  itr += "     (";
  itr += std::get<std::string>(width);
  itr += ",";
  itr += std::get<std::string>(height);
  itr += ")\n";
  for (int i = 0; i < std::get<int>(width); ++i) {
    itr += std::to_string(i % 10);
  }

  raw();
  noecho();
  keypad(term, true);
  scrollok(term, TRUE);
  bool result = prompt::runLoop(term, pmt, itr);
  if (term != stdscr) {
    delwin(term);
  }
  endwin();
  std::cout << result << std::endl;
  return 1;
}