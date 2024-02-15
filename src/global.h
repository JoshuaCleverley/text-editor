#pragma once

#include <stdbool.h>
#include "mode.h"

typedef struct global {
  Mode modes[3];
  Mode current_mode;
  bool running;
  int rows, cols;
  int ret_y, ret_x;
  char command_buf[256];
} Global;

void init_global();

extern Global global;
