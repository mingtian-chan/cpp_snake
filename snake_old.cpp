#include <ncurses.h>
#include <iostream>
#include <string>
#include <fstream>

//  g++ -std=c++11 -o hello Helloworld.cpp -lncursesw

string MapPath;
int mapHeight;
int mapWidth;

int **map;

Map::Map(){}

Map::Map(int stage) { // stage마다 다른 맵 
}

Map::LoadMap() {
    mapPath = "./Map/level0.txt";
    ifstream in(mapPath);
    in >> mapHeight >> mapWidth;
    map = new int *[mapHeight];
    for (int i = 0; i < mapHeight; i++) {
        map[i] = new int [mapWidth];
        for (int j = 0; j < mapWidth; j++) {
            in >> map[i][j];
        }
    }
    
}
int Map::getMapValue(int y, int x) {
    return map[y][x];
}

void Map::setMapValue(int y, int x, int value) {
    map[y][x] = value;
}

vector<int> Map::getMapPos(int value) {
    vector<int> v;
    for (int i = 0; i < mapHeight; i++) {
        for (int j = 0; j < mapWidth; j++) {
            if (map[i][j] == value) {
                v.push_back(i);
                v.push_back(j);
            }
        }
    }
    return v;
}

void Map::View() {
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
// -Tutorial End

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
}


int main(){
    Map();
}