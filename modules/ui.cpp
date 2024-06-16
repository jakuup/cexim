#include <future>
#include "ui.hpp"

using namespace ui;

// --------------------------------------------------------
UserInterface::UserInterface(int t_startx, int t_starty, int t_width, int t_height)
{
  initscr();
  raw();
  noecho();
  m_window = newwin(t_height, t_width, t_starty, t_startx);
  keypad(m_window, TRUE);
  scrollok(m_window, TRUE);
  prompt = ">";
  m_line = "";
}

// --------------------------------------------------------
bool UserInterface::execute()
{
  if (intro != "") {
    wprintw(m_window, "%s", intro.c_str());
  }
  printPrompt();

  auto f = std::async(std::launch::async, &UserInterface::loop, this);
  while (f.wait_for(std::chrono::milliseconds(20)) != std::future_status::ready);
  return f.get();
}

// --------------------------------------------------------
UserInterface::~UserInterface()
{
  delwin(m_window);
  endwin();
}

// --------------------------------------------------------
void UserInterface::printPrompt()
{
  if (getcurx(m_window) != 0) {
    wprintw(m_window, "\n");
  }
  wprintw(m_window, "%s ", prompt.c_str());
  m_col[0] = m_col[1] = getcurx(m_window);
  m_row[0] = m_row[1] = getcury(m_window);
  m_pos = 0;
}

// --------------------------------------------------------
bool UserInterface::loop()
{
  int key;

  while (true) {
    key = wgetch(m_window);
    switch (key) {
      case '~':
        return true;

      default:
        if ((key >= ' ') && (key <= '~')) {
          int row = getcury(m_window);
          int col = getcurx(m_window);
          m_line.insert(m_pos, 1, key);
          std::string update = m_line.substr(m_pos);
          ++m_pos;
          wprintw(m_window, "%s", update.c_str());
          m_col[1] = getcurx(m_window);
          m_row[1] = getcury(m_window);
          wmove(m_window, row, col + 1);
          wrefresh(m_window);
        }
        break;

      case '\n':
        wmove(m_window, m_row[1], m_col[1]);
        wrefresh(m_window);
        waddch(m_window, '\n');
        if (m_line.size() > 0) {
          //executeCommand(m_line);
        }
        m_line.erase();
        m_pos = 0;
        printPrompt();
        break;

      case KEY_RIGHT:
        if (m_pos < m_line.size()) {
          ++m_pos;
          int row = getcury(m_window);
          int col = getcurx(m_window) + 1;
          if (col == getmaxx(m_window)) {
            col = 0;
            ++row;
          }
          wmove(m_window, row, col);
          wrefresh(m_window);
        }
        break;

      case KEY_LEFT:
        if (m_pos > 0) {
          --m_pos;
          int row = getcury(m_window);
          int col = getcurx(m_window);
          if (col == 0) {
            col = getmaxx(m_window) - 1;
            --row;
          }
          else {
            --col;
          }
          wmove(m_window, row, col);
          wrefresh(m_window);
        }
        break;

      case KEY_HOME:
        m_pos = 0;
        wmove(m_window, m_row[0], m_col[0]);
        wrefresh(m_window);
        break;

      case KEY_END:
        m_pos = m_line.size();
        wmove(m_window, m_row[1], m_col[1]);
        wrefresh(m_window);
        break;

      case KEY_BACKSPACE:
        if (m_pos > 0) {
          std::string rest = m_line.substr(m_pos); // remember the following part of the line
          --m_pos;                                 // erase the preceding character
          m_line.erase(m_pos, 1);
          int row = getcury(m_window);             // remember the new (row,col)
          int col = getcurx(m_window) - 1;
          wmove(m_window, row, col);               // print the following part of the line
          printw("%s", rest.c_str());
          wdelch(m_window);                        // currently at the end, wipe the last character
          m_row[1] = getcury(m_window);            // update new end (row,col)
          m_col[1] = getcurx(m_window);
          wmove(m_window, row, col);               // move back to the new (row,col)
        }
        break;
    }
  }

  return false;
}
