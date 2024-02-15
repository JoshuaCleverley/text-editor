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

void handle_command(char *command_out, int buf_size) {
#define SET_COMMAND_OUT(str, ...)                                              \
  snprintf(command_out, buf_size, str, ##__VA_ARGS__);
#define CMP_COMMAND(str) strcmp(global.command_buf, str) == 0

  if (CMP_COMMAND("q")) {
    SET_COMMAND_OUT("Quitting editor.");
    global.running = false;
  } else if (CMP_COMMAND("w")) {
    SET_COMMAND_OUT("Saving file.");
  } else if (CMP_COMMAND("help") || CMP_COMMAND("?") || CMP_COMMAND("h")) {
    SET_COMMAND_OUT("Available commands: q, w, help");
  } else if (CMP_COMMAND("")) {
    // No output if no command is entered (just return to normal mode)
  } else {
    SET_COMMAND_OUT("ERROR: Command '%s' not found.", global.command_buf);
  }

#undef CMP_COMMAND
#undef SET_COMMAND_OUT
}

void command_mode(int ch) {
  // In command mode:
  // - 'ESC' returns to normal mode
  // - 'BACKSPACE' deletes the previous character
  // - 'ENTER' executes the command
  // - Any other character is added to the command buffer
  int y, x;
  char command_out[256] = {0};

  switch (ch) {
  case KEY_RETURN:
    handle_command(command_out, sizeof(command_out));
    // fall through
  case KEY_ESC:
    move(global.rows - 1, 0);
    clrtoeol();
    if (strlen(command_out) > 0) {
      mvprintw(global.rows - 1, 0, "%s", command_out);
    }
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
