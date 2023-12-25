
#include <atomic>
#include <chrono>
#include <ncurses.h>
#include <string>
#include <thread>
#include <unistd.h>
/*
 * TODO: Add a pause
 * DONE: Add a menu
 * DONE: Instead of removing the text, color it white from gray
 * DONE: Add a timer
 * DONE: Add a score
 * TODO: Use Classes To make the code more organized
 * TODO: Make the text scrolling from one line to another 
 * TODO: Make the UI responsive (figure out how)
 * TODO: How to add boxes
 */

std::atomic<bool> stopWatchActive(true);

void stopwatch() {
  auto startTime = std::chrono::steady_clock::now();

  while (stopWatchActive) {
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(
                           currentTime - startTime)
                           .count();

    // std::cout << "Elapsed time: " << elapsedTime << " seconds\n";
    int pos_c, pos_r;
    getyx(stdscr, pos_r, pos_c);

    mvprintw(0, 0, "Elapsed time: %ld", elapsedTime);
    move(pos_r, pos_c);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    refresh();
  }
}

void typeLine(std::string text) {
  clear();
  std::thread stopwatchThread(stopwatch);
  attrset(A_DIM);
  int r, c;
  getmaxyx(stdscr, r, c);
  WINDOW *win = newwin(r, c, 0, 0); // Rows, columns, start row, start col
  // const char *text = "This text is centered horizontally.";

  // Calculate the starting position to center the text
  int start_col = (c - static_cast<int>(text.length())) / 2;
  int start_row = (r - 1) / 2;
  // Move the cursor to the calculated position
  move(start_row, start_col);

  // Print the centered text
  printw("%s\n", text.c_str());
  refresh();
  // Print some text
  move(start_row, start_col);
  refresh();

  // Wait for a key press
  int words = 0;
  for (int i = 0; i < text.length(); i++) {
    char ch = '*';
    while ((ch = getch()) != text[i])
      ;
    if (ch == ' ')
      words++;
    mvprintw(0, 30, "Word: %d", words);
    move(start_row, start_col + i);
    attrset(A_NORMAL);
    mvprintw(start_row, start_col + i, "%c", ch);
    refresh();
  }
  // Stop the stopwatch
  stopWatchActive = false;

  // Wait for the stopwatch thread to finish
  stopwatchThread.join();
}

void updateMenu(std::string menuItems[], int items, int ch) {
  int r, c;
  getmaxyx(stdscr, r, c);
  clear();
  // Vertical Padding
  int vpad = (r / 2) - (items / 2);
  move(vpad, 0);
  for (int i = 0; i < items; i++) {
    int hpad = (c / 2) - (menuItems[i].length() / 2);
    if (i == ch)
      mvprintw(vpad + i, hpad - 3, "[[ %s ]]", menuItems[i].c_str());
    else
      mvprintw(vpad + i, hpad, "%s", menuItems[i].c_str());
    refresh();
  }
}

int menuPicker(std::string menuItems[], int items) {
  clear();
  char ch;
  int pos = 0;
  updateMenu(menuItems, items, pos);
  while (ch != '\n') {
    updateMenu(menuItems, items, pos);
    ch = getch();
    switch (ch) {
    case 'w':
      if (pos > 0)
        pos--;
      break;
    case 's':
      if (pos < items - 1)
        pos++;
      break;
    }
  }
  return pos;
}

void main_menu() {
  std::string menuItems[] = {"Start", "Options", "Exit"};
  int ch = menuPicker(menuItems, 3);
  switch (ch) {
  case 0:
    typeLine("This text is centered horizontally.");
    break;
  case 1:
    printw("Options");
    break;
  case 2:
  default:
    break;
  }
}

int main() {
  // Initialize ncurses
  initscr();
  noecho();
  // typeLine("This text is centered horizontally.");
  main_menu();
  endwin();

  return 0;
}
