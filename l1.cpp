#include <ncurses.h>
#include <iostream>

//  g++ -std=c++11 -o hello Helloworld.cpp -lncursesw

int main(){
  WINDOW *win1, *win2, *win3;

  initscr();
  resize_term(30,100); // 윈도우의 크기는 30, 100
  start_color();
  init_pair(1, COLOR_GREEN, COLOR_BLACK); // 색깔 설정 ( 1번 : 초록 , 검정)
  init_pair(2, COLOR_YELLOW, COLOR_BLACK); // 색깔 설정  (2번 : 노랑, 검정)

  border('|','|', '-', '-', '+','+','+','+'); // 게임 전체 테두리
  mvprintw(2,2, "SNAKE GAME");
  refresh();
  getch();

  // // Tutorial
  // // level 1 Lets grow (Grow Item Only)
  // win1 = newwin(22, 60, 4, 10);
  // wbkgd(win1, '*');
  //
  // wborder(win1, '|','|', '-', '-', '+','+','+','+');
  // mvwprintw(win1, 0, 2, "Lets Grow");
  // wrefresh(win1);


  //
  // // level 2 be smaller (poison Item Only)
  // win1 = newwin(22, 60, 4, 10);
  // wbkgd(win1, '*');
  // wborder(win1, '|','|', '-', '-', '+','+','+','+');
  // mvwprintw(win1, 0, 2, "Be Smaller");
  // wrefresh(win1);


  // level 3 use the gate (Gate Only)
  win1 = newwin(22, 60, 4, 10);
  wbkgd(win1, '*');
  wborder(win1, '|','|', '-', '-', '+','+','+','+');
  mvwprintw(win1, 0, 2, "Use The Gate");
  mvwprintw(win1, 8, 0, "011111111111111111111111111111111111111111111111111111111110");
  // 중간에 스페이스로 비우면 이빠진 모양도 만들 수 있음.

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
