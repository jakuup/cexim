#include "ui.hpp"

using namespace ui;

int main(int argc, const char *argv[])
{
  const int WIDTH = 30;
  UserInterface user_iface {0, 0, WIDTH, 15};
  
  user_iface.intro = "";
  for (int i = 0; i < WIDTH; ++i) {
    user_iface.intro += std::to_string(i % 10);
  }
  user_iface.prompt = "test>";

  user_iface.execute();
}
