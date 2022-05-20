#include <iostream>
#include <stdlib.h>
#include <time.h>

int[] Item_Pos_Generator() {
   srand(time(NULL));
   int item_x, item_y;

   item_x = rand() % 22 + 1; // x좌표 생성기
   item_y = rand() % 60 + 1; // y좌표 생성기
   int[] arr = new int[2];


  return (item_x, item_y);
 }

  int main() {
    std::cout<< "im here" << std::endl;
    std::cout<< arr << std::endl;
    std::cout<< "im out" << std::endl;
  }
