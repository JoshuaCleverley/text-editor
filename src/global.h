#pragma once

#include <stdbool.h>
#include "mode.h"

// Set the buffer size for file reading to 1MB
#define FILE_BUFFER_SIZE 1024 * 1024

typedef struct global {
  Mode modes[3];
  Mode current_mode;
  bool running;
  int rows, cols;
  int ret_y, ret_x;
  char command_buf[256];
  char *file_name;
  char file_buf[FILE_BUFFER_SIZE];
} Global;

void init_global();

extern Global global;
