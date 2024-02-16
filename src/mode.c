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
    global.file_buf[strlen(global.file_buf) - 1] = '\0';
    break;
  default:
    // Add character to screen and file buf
    addch(ch);
    global.file_buf[strlen(global.file_buf)] = ch;
    break;
  }
}

char *write_file_to_disk(char *filename) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    return "ERROR: Could not open file for writing.";
  }
  if (fwrite(global.file_buf, sizeof(char), strlen(global.file_buf), file) ==
      0) {
    return "ERROR: Could not write file to disk.";
  }
  fclose(file);
  return "INFO: File written to disk.";
}

char *read_file_from_disk(char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    return "ERROR: Could not open file for reading.";
  }
  memset(global.file_buf, 0, sizeof(global.file_buf));
  if (fread(global.file_buf, sizeof(char), sizeof(global.file_buf), file) ==
      0) {
    return "ERROR: Could not read file from disk.";
  }
  fclose(file);
  return "INFO: File read from disk.";
}

void handle_command(char *command_out, int buf_size) {
#define SET_COMMAND_OUT(str, ...)                                              \
  snprintf(command_out, buf_size, str, ##__VA_ARGS__);
#define CMP_COMMAND(str) strcmp(global.command_buf, str) == 0
#define UNIMPLEMENTED()                                                        \
  SET_COMMAND_OUT("INFO: Command '%s' not implemented.", global.command_buf);
#define CMP_COMMANDS(str1, str2) (CMP_COMMAND(str1) || CMP_COMMAND(str2))

  if (CMP_COMMANDS("quit", "q")) {
    SET_COMMAND_OUT("Quitting editor.");
    global.running = false;
  } else if (CMP_COMMANDS("w", "write")) {
    // Write file_buf to disk
    SET_COMMAND_OUT("%s", write_file_to_disk("filename.txt"));
  } else if (CMP_COMMANDS("wq", "writequit")) {
    SET_COMMAND_OUT("%s", write_file_to_disk("filename.txt"));
    global.running = false;
  } else if (CMP_COMMANDS("open", "o")) {
    // Read file into file_buf
    SET_COMMAND_OUT("%s", read_file_from_disk("filename.txt"));
    // Clear screen and print file_buf
    clear();
    move(0, 0);
    printw("%s", global.file_buf);
    // find the cursor position at end of file_buf
    int y = 0, x = 0;
    for (long unsigned int i = 0; i < strlen(global.file_buf); i++) {
      if (global.file_buf[i] == '\n') {
        y++;
        x = 0;
      } else {
        x++;
      }
    }
    global.ret_y = y;
    global.ret_x = x;
  } else if (CMP_COMMANDS("help", "h") || CMP_COMMAND("?")) {
    SET_COMMAND_OUT("Available commands: quit, write, writequit, open, help");
  } else if (CMP_COMMAND("")) {
    // No output if no command is entered (just return to normal mode)
  } else {
    SET_COMMAND_OUT("ERROR: Command '%s' not found.", global.command_buf);
  }

#undef CMP_COMMANDS
#undef UNIMPLEMENTED
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
