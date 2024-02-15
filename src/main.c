#include <ncurses.h>
#include <stdbool.h>
#include <string.h>

typedef struct mode {
  char *name;
  void (*handler)(int ch);
} Mode;

typedef struct global {
  Mode modes[3];
  Mode current_mode;
  bool running;
  int rows, cols;
  int ret_y, ret_x;
  char command_buf[256];
} Global;
Global global;

void normal_mode(int ch) {
  // In normal mode:
  // - 'i' switches to insert mode
  switch (ch) {
  case 'i':
    global.current_mode = global.modes[1];
    break;
  case ':':
    getyx(stdscr, global.ret_y, global.ret_x);
    move(global.rows - 1, 0);
    clrtoeol();
    addch(':');
    memset(global.command_buf, 0, sizeof(global.command_buf));
    global.current_mode = global.modes[2];
    break;
  default:
    break;
  }
}

void insert_mode(int ch) {
  // In insert mode:
  // - 'ESC' returns to normal mode
  // - 'BACKSPACE' deletes the previous character
  // - Any other character is added to the screen
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

void command_mode(int ch) {
  // In command mode:
  // - 'ESC' returns to normal mode
  // - 'BACKSPACE' deletes the previous character
  // - 'ENTER' executes the command
  // - Any other character is added to the command buffer
  int y, x;
  switch (ch) {
  case 10:
    if (strcmp(global.command_buf, "q") == 0) {
      global.running = false;
    }
    // fall through
  case 27:
    move(global.rows - 1, 0);
    clrtoeol();
    move(global.ret_y, global.ret_x);
    global.current_mode = global.modes[0];
    break;
  case KEY_BACKSPACE:
    getyx(stdscr, y, x);
    move(y, x - 1);
    delch();
    global.command_buf[strlen(global.command_buf) - 1] = '\0';
    break;
  default:
    // Add character to command buffer
    global.command_buf[strlen(global.command_buf)] = ch;
    addch(ch);
    break;
  }
}

int main() {
  // Declare variables
  int ch, y, x;
  global.modes[0] = (Mode){" NORMAL MODE", normal_mode};
  global.modes[1] = (Mode){" INSERT MODE", insert_mode};
  global.modes[2] = (Mode){"COMMAND MODE", command_mode};
  global.current_mode = global.modes[0];
  global.running = true;

  // Initialize ncurses
  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();
  set_escdelay(25);
  getmaxyx(stdscr, global.rows, global.cols);

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
