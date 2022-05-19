#include <ncurses.h>

//  g++ -std=c++11 -o hello Helloworld.cpp -lncursesw

int main(){
  WINDOW *win1, *win2, *win3;

  initscr();
  resize_term(30,100);
  start_color();
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  init_pair(2, COLOR_YELLOW, COLOR_BLACK);

  border('|','|', '-', '-', '+','+','+','+');
  mvprintw(2,2, "SNAKE GAME");
  refresh();
  getch();

  // Tutorial
  // level 1 Lets grow (Grow Item Only)
  // win1 = newwin(22, 60, 4, 10);
  // wbkgd(win1, '*');
  //
  // wborder(win1, '|','|', '-', '-', '+','+','+','+');
  // wrefresh(win1);
  // //
  //
  // // level 2 be smaller (poison Item Only)
  // win1 = newwin(22, 60, 4, 10);
  // wbkgd(win1, '*');
  //
  // wborder(win1, '|','|', '-', '-', '+','+','+','+');
  // wrefresh(win1);


  // level 3 use the gate (Gate Only)
  win1 = newwin(22, 60, 4, 10);
  wbkgd(win1, '*');
  mvwprintw(win1, 8, 1, "1111111          11111111111111111111111111111111111111111");
  // 중간에 스페이스로 비우면 이빠진 모양도 만들 수 있음.
  wborder(win1, '|','|', '-', '-', '+','+','+','+');
  wrefresh(win1);

// -Tutorial End

  
  win2 = newwin(8, 10, 5, 80);
  wbkgd(win2, COLOR_PAIR(2));
  wattron(win2, COLOR_PAIR(2));
  mvwprintw(win2, 1, 1, "win2");
  wborder(win2, '|','|', '-', '-', '+','+','+','+');
  wrefresh(win2);

  win3 = newwin(8, 10, 16, 80);
  wbkgd(win3, COLOR_PAIR(2));
  wattron(win3, COLOR_PAIR(2));
  mvwprintw(win3, 1, 1, "win3");
  wborder(win3, '|','|', '-', '-', '+','+','+','+');
  wrefresh(win3);

  getch();
  delwin(win1);
  delwin(win2);
  delwin(win3);
  endwin();


  return 0;
}
