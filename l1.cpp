// #include <locale.h> // 이거 해도 유니코드가 실행이 안됨.
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
  init_pair(3, COLOR_RED, COLOR_BLACK);

  border('|','|', '-', '-', '+','+','+','+'); // 게임 전체 테두리
  mvprintw(2,2, "SNAKE GAME");
  refresh();
  getch();

  // // Tutorial
  // // level 1 Lets grow (Grow Item Only)
  // win1 = newwin(22, 60, 4, 10);
  // wbkgd(win1, '0');
  //
  // wborder(win1, '|','|', '-', '-', '+','+','+','+');
  // mvwprintw(win1, 0, 2, "Lets Grow");
  // wrefresh(win1);


  //
  // // level 2 be smaller (poison Item Only)
  // win1 = newwin(22, 60, 4, 10);
  // wbkgd(win1, '0');
  // wborder(win1, '|','|', '-', '-', '+','+','+','+');
  // mvwprintw(win1, 0, 2, "Be Smaller");
  // wrefresh(win1);


  // // level 3 use the gate (Gate Only)
  // win1 = newwin(22, 60, 4, 10);
  // wbkgd(win1, ' ');
  // wborder(win1, '|','|', '-', '-', '+','+','+','+');
  // mvwprintw(win1, 0, 2, "Use The Gate");
  // mvwprintw(win1, 8, 0, "+----------------------------------------------------------+");
  // // 중간에 스페이스로 비우면 이빠진 모양도 만들 수 있음.
  //
  // wrefresh(win1);

  // -Tutorial End

  // level 4 welcome to snake_world
  win1 = newwin(22, 60, 4, 10);
  wbkgd(win1, COLOR_PAIR(3));
  wattron(win1, COLOR_PAIR(3));
  wborder(win1, '|','|', '-', '-', '+','+','+','+');
  mvwprintw(win1, 0, 2, "welcome to snake_world");
  //
  // mvwprintw(win1, 5, 0, "■                            ■                             ■"); // 이거 locale.h 불러와도 ~V 로 출력되고, 출력이 완됨..
  // mvwprintw(win1, 6, 0, "■                            ■                             ■");
  // mvwprintw(win1, 7, 0, "■                            ■                             ■");
  // mvwprintw(win1, 8, 0, "■                            ■                             ■");
  // mvwprintw(win1, 9, 0, "■                            ■                             ■");
  // mvwprintw(win1, 10, 0, "■                 ■■■■■■■■■■■+■■■■■■■■■■                   ■");
  // mvwprintw(win1, 11, 0, "■                            ■                             ■");
  // mvwprintw(win1, 12, 0, "■                            ■                             ■");
  // mvwprintw(win1, 13, 0, "■                            ■                             ■");
  // mvwprintw(win1, 14, 0, "■                            ■                             ■");
  // mvwprintw(win1, 15, 0, "■                            ■                             ■");


    mvwprintw(win1, 5, 0, "|                            |                             |");
    mvwprintw(win1, 6, 0, "|                            |                             |");
    mvwprintw(win1, 7, 0, "|                            |                             |");
    mvwprintw(win1, 8, 0, "|                            |                             |");
    mvwprintw(win1, 8, 0, "|                            |                             |");
    mvwprintw(win1, 9, 0, "|                            |                             |");
    mvwprintw(win1, 10, 0, "|                 -----------+----------                   |");
    mvwprintw(win1, 11, 0, "|                            |                             |");
    mvwprintw(win1, 12, 0, "|                            |                             |");
    mvwprintw(win1, 13, 0, "|                            |                             |");
    mvwprintw(win1, 14, 0, "|                            |                             |");
    mvwprintw(win1, 15, 0, "|                            |                             |");
  // 중간에 스페이스로 비우면 이빠진 모양도 만들 수 있음.
    wattroff(win1, COLOR_PAIR(3));

  wrefresh(win1);



  win2 = newwin(8, 10, 5, 80);
  wbkgd(win2, COLOR_PAIR(2));
  wattron(win2, COLOR_PAIR(2));
  mvwprintw(win2, 1, 1, "score : ");
  mvwprintw(win2, 2, 1, "0");
  wborder(win2, '|','|', '-', '-', '+','+','+','+');
  wattroff(win2 , COLOR_PAIR(2));
  wrefresh(win2);

  win3 = newwin(8, 10, 16, 80);
  wbkgd(win3, COLOR_PAIR(2));
  wattron(win3, COLOR_PAIR(2));
  mvwprintw(win3, 1, 1, "name : ");
  mvwprintw(win3, 2, 1, "user ");
  wborder(win3, '|','|', '-', '-', '+','+','+','+');
  wrefresh(win3);
  wattroff(win3, COLOR_PAIR(2));

  getch();
  delwin(win1);
  delwin(win2);
  delwin(win3);
  endwin();


  return 0;
}
