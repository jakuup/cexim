#ifndef __UI_HPP__
#define __UI_HPP__

#include <string>
#include <ncurses.h>

namespace ui 
{

class UserInterface
{
public:

  std::string intro;  // the introduction message
  std::string prompt; // the prompt string

  UserInterface(int t_startx, int t_starty, int t_width, int t_height);

  bool execute();

  ~UserInterface();

private:

  WINDOW *m_window;     // the window that the prompt is operating on
  std::string m_prompt; // the prompt string
  std::string m_line;   // the line input by the user
  int m_pos;            // position in m_line
  int m_row[2];         // m_line row: [0] - begin, [1] - end
  int m_col[2];         // m_line col: [0] - begin, [1] - end

  void printPrompt();
  bool loop();

};

}

#endif