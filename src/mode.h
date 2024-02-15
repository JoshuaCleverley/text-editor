#pragma once 

#define KEY_ESC 27
#define KEY_RETURN 10

typedef struct mode {
  char *name;
  void (*handler)(int ch);
} Mode;

void init_modes();
void normal_mode(int ch);
void insert_mode(int ch);
void command_mode(int ch);
