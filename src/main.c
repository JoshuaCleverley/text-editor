#include <ncurses.h>
#include <stdbool.h>
#include <string.h>

#include "global.h"
#include "mode.h"

int main() {
  // Declare variables
  int ch, y, x;

  // Initialize ncurses
  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();
  set_escdelay(25);

  init_global();
  init_modes();

  while (global.running) {
    // Print mode in bottom right
    getyx(stdscr, y, x);
    mvprintw(global.rows - 2, global.cols - strlen(global.current_mode.name),
             "%s", global.current_mode.name);
    move(y, x);

    // Get input
    ch = getch();

    // Handle input
    global.current_mode.handler(ch);
    refresh();
  };

  endwin();
}
