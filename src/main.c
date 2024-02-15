#include <ncurses.h>

int main() {
  int ch;

  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();

  while (true) {
    ch = getch();
    if (ch == 27)
      break;
    printw("Keycode: %d\n", ch);
    refresh();
  }

  endwin();

  return 0;
}
