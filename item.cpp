#include <iostream>
#include <stdlib.h>
#include <time.h>
  public int pos[2]; // public 으로 배열 만듬

  void Item_Pos_Generator() {
    srand(time(NULL));
    int pos_x, pos_y;

    pos_x = rand() % 22 + 1; // x좌표 생성기
    pos_y = rand() % 60 + 1; // y좌표 생성기
    int pos[2] = {pos_x, pos_y};
}

  int main() {
    std::cout<< "im here" << std::endl;
    Item_Pos_Generator();
    std::cout << pos[2] << std::endl;
    std::cout << "im out" << std::endl;
  }
