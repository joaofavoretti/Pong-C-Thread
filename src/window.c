#include <ncurses.h>
#include <window.h>
#include <game_config.h>

// Allocs a new window and sets a box around it plus displays it
WINDOW *create_newwin(const int height, const int width, const int starty,
                      const int startx) {
  WINDOW *local_win;

  local_win = newwin(height, width, starty, startx);
  /* 0, 0 gives default characters
   * for the vertical and horizontal
   * lines 
   **/
  box(local_win, 0, 0);
  wrefresh(local_win);  /* Show that box */

  return local_win;
}

// Deallocs the window and removes leftover artefacts
void destroy_win(WINDOW *local_win) {
  /* box(local_win, ' ', ' '); : This won't produce the desired
   * result of erasing the window. It will leave it's four corners
   * and so an ugly remnant of window.
   */
  wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wrefresh(local_win);
  delwin(local_win);
}
