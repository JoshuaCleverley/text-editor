#include <ncurses.h>
#include <stdbool.h>
#include <string.h>

typedef struct mode {
  char *name;
  void (*handler)(int ch);
} Mode;

typedef struct global {
  Mode modes[2];
  Mode current_mode;
  bool running;
} Global;
Global global;

void normal_mode(int ch) {
  switch (ch) {
  case 'i':
    global.current_mode = global.modes[1];
    break;
  case 'q':
    global.running = false;
    break;
  default:
    break;
  }
}

void insert_mode(int ch) {
  int y, x;

  switch (ch) {
  case 27:
    curs_set(2);
    global.current_mode = global.modes[0];
    break;
  case KEY_BACKSPACE:
    getyx(stdscr, y, x);
    move(y, x - 1);
    delch();
    break;
  default:
    addch(ch);
    break;
  }
}

int main() {
  // Declare variables
  int ch, y, x, row, col;
  global.modes[0] = (Mode){"NORMAL MODE", normal_mode};
  global.modes[1] = (Mode){"INSERT MODE", insert_mode};
  global.current_mode = global.modes[0];
  global.running = true;

  // Initialize ncurses
  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();
  set_escdelay(25);
  getmaxyx(stdscr, row, col);

  while (global.running) {
    // Print mode in bottom right
    getyx(stdscr, y, x);
    mvprintw(row - 2, col - strlen(global.current_mode.name), "%s",
             global.current_mode.name);
    move(y, x);

    // Get input
    ch = getch();

    // Handle input
    global.current_mode.handler(ch);
    refresh();
  };

  endwin();
}
