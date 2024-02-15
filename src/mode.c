#include <ncurses.h>
#include <string.h>

#include "global.h"
#include "mode.h"

void init_modes() {
  global.modes[0] = (Mode){" NORMAL MODE", normal_mode};
  global.modes[1] = (Mode){" INSERT MODE", insert_mode};
  global.modes[2] = (Mode){"COMMAND MODE", command_mode};
  global.current_mode = global.modes[0];
}

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
  case KEY_ESC:
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
  case KEY_RETURN: // ENTER
    if (strcmp(global.command_buf, "q") == 0) {
      global.running = false;
    }
    // fall through
  case KEY_ESC:
    move(global.rows - 1, 0);
    clrtoeol();
    move(global.ret_y, global.ret_x);
    global.current_mode = global.modes[0];
    break;
  case KEY_BACKSPACE:
    if (strlen(global.command_buf) == 0) {
      break;
    }
    getyx(stdscr, y, x);
    move(y, x - 1);
    delch();
    global.command_buf[strlen(global.command_buf) - 1] = '\0';
    break;
  default:
    // Add character to command buffer
    if (strlen(global.command_buf) >= sizeof(global.command_buf) - 1) {
      // Buffer is full
      break;
    }
    global.command_buf[strlen(global.command_buf)] = ch;
    addch(ch);
    break;
  }
}
