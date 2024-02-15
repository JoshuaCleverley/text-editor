#include <ncurses.h>

int main() {
  int ch, y, x;

  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();

  do {
    ch = getch();

    // Check if key is backspace
    // If it is, move the cursor back one space and delete the character
    if (ch == KEY_BACKSPACE) {
      getyx(stdscr, y, x);
      move(y, x - 1);
      delch();
      refresh();
      continue;
    } else
      addch(ch);
    refresh();
  } while (ch != 'q');

  endwin();

  return 0;
}
