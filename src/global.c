#include <ncurses.h>
#include <string.h>

#include "global.h"

Global global = {0};

void init_global() {
  global.running = true;
  memset(global.command_buf, 0, sizeof(global.command_buf));
  getmaxyx(stdscr, global.rows, global.cols);
}
