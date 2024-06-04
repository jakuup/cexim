#include <future>
#include "prompt.hpp"

namespace prompt {
namespace {

  WINDOW *window;
  std::string prompt;
  std::string intro;
  
  std::string line = "";
  //int row, col;
  int lpos;              // position in line
  int lrow[2], lcol[2];  // line row/col: [0] - begin, [1] - end

  // --------------------------------------------------------
  void printIntro()
  {
    wmove(window, 0, 0);
    wprintw(window, "%s\n", intro.c_str());
  }

  // --------------------------------------------------------
  void printPrompt()
  {
    if (getcurx(window) != 0) {
      wprintw(window, "\n");
    }
    wprintw(window, "%s ", prompt.c_str());
    lcol[0] = lcol[1] = getcurx(window);
    lrow[0] = lrow[1] = getcury(window);
  }

  // --------------------------------------------------------
  void printStatus(int _key = 0)
  {
    if (not testMode) {
      return;
    }

    char *buf = new char[getmaxx(window)];

    std::snprintf(buf, getmaxx(window),
                  "(%d,%d)[%d,%d] %d [%d]",
                  getcury(window), getcurx(window), getmaxy(window), getmaxx(window),
                  lpos, _key);

    int row = getcury(window);
    int col = getcurx(window);
    
    move(window->_maxy, 0);
    deleteln();
    wprintw(window, "%s", buf);
    move(row, col);

    delete buf;
  }

  // --------------------------------------------------------
  bool loop()
  {
    int key;

    while (true) {
      key = wgetch(window);
      switch (key) {
        case '~':
          return true;

        default:
          if ((key >= ' ') && (key <= '~')) {
            int row = getcury(window);
            int col = getcurx(window);
            line.insert(lpos, 1, key);
            std::string update = line.substr(lpos);
            ++lpos;
            wprintw(window, "%s", update.c_str());
            lcol[1] = getcurx(window);
            lrow[1] = getcury(window);
            move(row, col + 1);
            refresh();
          }
          break;

        case '\n':
          move(lrow[1], lcol[1]);
          refresh();
          addch('\n');
          if (line.size() > 0) {
            //executeCommand(line);
          }
          line.erase();
          lpos = 0;
          printPrompt();
          break;

        case KEY_RIGHT:
          if (lpos < line.size()) {
            ++lpos;
            int row = getcury(window);
            int col = getcurx(window) + 1;
            if (col == getmaxx(window)) {
              col = 0;
              ++row;
            }
            move(row, col);
            refresh();
          }
          break;

        case KEY_LEFT:
          if (lpos > 0) {
            --lpos;
            int row = getcury(window);
            int col = getcurx(window);
            if (col == 0) {
              col = getmaxx(window) - 1;
              --row;
            }
            else {
              --col;
            }
            move(row, col);
            refresh();
          }
          break;

        case KEY_HOME:
          lpos = 0;
          move(lrow[0], lcol[0]);
          refresh();
          break;

        case KEY_END:
          lpos = line.size();
          move(lrow[1], lcol[1]);
          refresh();
          break;

        case KEY_BACKSPACE:
          if (lpos > 0) {
            std::string rest = line.substr(lpos); // remember the following part of the line
            --lpos;                               // erase the preceding character
            line.erase(lpos, 1);
            int row = getcury(window);            // remember the new (row,col)
            int col = getcurx(window) - 1;
            move(row, col);                       // print the following part of the line
            printw("%s", rest.c_str());
            delch();                              // currently at the end, wipe the last character
            lrow[1] = getcury(window);            // update new end (row,col)
            lcol[1] = getcurx(window);
            move(row, col);                       // move back to the new (row,col)
          }
          break;
      }
      printStatus(key);
    }

    return false;
  }

}

// ========================================================
bool runLoop(WINDOW *_window, const std::string &_prompt, const std::string &_intro)
{
  if (_window == nullptr) {
    return false;
  }

  window = _window;
  prompt = _prompt;
  intro = _intro;

  printIntro();
  printPrompt();
  printStatus();

  auto f = std::async(std::launch::async, loop);
  while (f.wait_for(std::chrono::milliseconds(20)) != std::future_status::ready);
  return f.get();
}

}