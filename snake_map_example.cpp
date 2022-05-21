#include <ncurses.h>
#include <stdio.h>
#include <cstdlib>
#include <time.h>
#include <string>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>
#define Max 5
struct termios oldterm, new_term;
int ch_read;
void set_stdin(void);
void reset_stdin(void);
int kgetch();

using namespace std;

typedef struct Node{
  const char* data;
  struct Node *next;
  struct  Node *prev;
  int x;
  int y;
}Node;

static struct termios initial_settings, new_settings;
static int peek_character = -1;

void init_keyboard()
{
    tcgetattr(0,&initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
}

int _kbhit()
{
    unsigned char ch;
    int nread;

    if (peek_character != -1) return 1;
    new_settings.c_cc[VMIN]=0;
    tcsetattr(0, TCSANOW, &new_settings);
    nread = read(0,&ch,1);
    new_settings.c_cc[VMIN]=1;
    tcsetattr(0, TCSANOW, &new_settings);
    if(nread == 1)
    {
        peek_character = ch;
        return 1;
    }
    return 0;
}

int _getch()
{
    char ch;

    if(peek_character != -1)
    {
        ch = peek_character;
        peek_character = -1;
        return ch;
    }
    read(0,&ch,1);
    return ch;
}

int initx = 13;
int inity = 20;

Node* insertfirstNode(Node *headernode,const char *num,int x, int y){

  Node *plusnode;
  plusnode = (Node *)malloc(sizeof(Node));

  plusnode->x = x;
  plusnode->y = y;

  plusnode->prev = NULL;

  plusnode->data = num;

  if(headernode == NULL){
    headernode = plusnode;
  }else{
    plusnode->next = headernode;
    headernode->prev = plusnode;
    headernode = plusnode;

  }
  return headernode;
}

Node* deleteNode(Node *headernode){

  Node *plusnode = headernode;
  Node *plusnode2 = headernode;

  while(plusnode -> next != NULL){
    plusnode = plusnode->next;
  }

  plusnode2 = plusnode -> prev;

  plusnode2 -> next = NULL;

  free(plusnode);


  return headernode;
}

Node* insertNode(Node *headernode,const char *num){
  Node *plus = headernode;

  Node *plusnode;
  plusnode = (Node *)malloc(sizeof(Node));

  while(plus->next != NULL){
    plus = plus->next;
  }

  plusnode->prev = plus;
  plus->next = plusnode;
  plusnode->x = (plus -> x)-1;
  plusnode->y = (plus -> y);
  plusnode->next = NULL;
  plusnode->data = num;

  return headernode;
}

class Game{
public :
  //char snakec[leng];
  WINDOW *win1;
  int i = 0;
  int j = 0;

  int keyhead = 1;
  int prekeyhead = keyhead;
  int TrueCounter = 1;
  double times = 0.0;

  int x_item = 18;
  int y_item = 20;

  int mx_item = 5;
  int my_item = 10;

  int gatestart_x = 20; //벽에 있는 경우를 먼저 만들어 보기
  int gatestart_y = 0;

  // int gatestart_x = 20; //not wall
  // int gatestart_y = 30;

  int gateend_x= 0; //left
  int gateend_y = 15;

  // int gateend_x= 29; //right
  // int gateend_y = 15;

  // int gateend_x = 10; //up
  // int gateend_y = 0;
  //
  // int gateend_x = 10; //down
  // int gateend_y = 39;

  //int gateend_x = 10; //not wall
  //int gateend_y = 15;

  Game(){
     //tetris
    initscr();
    resize_term(60,50);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);

    border('|','|','-','-','+','+','+','+');
    attron(COLOR_PAIR(1));
    mvprintw(1,1,"Welcome To the Tetris Game");
    attroff(COLOR_PAIR(1));
    refresh();
    //getch();

    win1 = newwin(40,30,3,3); //size, x, y

    wborder(win1,'|','|','-','-','+','+','+','+');
    init_pair(2, COLOR_GREEN, COLOR_BLACK);

    wattron(win1, COLOR_PAIR(2));
    wbkgd(win1,'0');
    wattroff(win1, COLOR_PAIR(2));


    mvwprintw(win1,0,5,"Tetris");

    //mvwprintw(win1,15,0,"2");

    //nodelay(win1,TRUE); //non-blocking
    //cbreak();

    wrefresh(win1); //update wprintw etc...
    //getch();

    //mvwprintw(win1,6,5,"="); //minus
    //mvwprintw(win1,14,15,"=");

    mvwprintw(win1,y_item,x_item,"#"); //plus
    mvwprintw(win1,my_item,mx_item,"="); //minus

    mvwprintw(win1,gatestart_y,gatestart_x,"@"); //gate start
    mvwprintw(win1,gateend_y, gateend_x,"@"); //gate end


  }

  void snake_make(Node *headernode){

    int i = 0;

    Node* temp = headernode;

    while(temp != NULL){
      mvwprintw(win1,temp->y,temp->x,(temp->data));
      temp = temp->next;
      i++;
    }

    wrefresh(win1);

  }

//linked list snake ok

  void snake_point(int input){
    prekeyhead = keyhead;

    //printf("getch() = %d\n", getch());
    switch (input) {
      case 68 :

      keyhead = 1;

      break;

      case 67:

      keyhead = 2;
      break;

      case 65:
      keyhead = 3;
      break;

      case 66:
      keyhead = 4;
      break;
    }

    // if((prekeyhead == 1&&keyhead == 2)||(prekeyhead == 2&&keyhead == 1)){
    //   printf("Fail!\n");
    // }else if((prekeyhead == 3&&keyhead == 4)||(prekeyhead == 4&&keyhead == 3)){
    //   printf("Fail!\n");
    // }

}

  void snake_move(Node *headernode){
    // clock_t start,end;
    Node* head = headernode;
    Node* temp = headernode;

    if(keyhead == 1){
      initx = initx-1;
    }else if(keyhead == 2){
      initx = initx+1;
    }else if(keyhead == 3){
      inity = inity-1;
    }else if(keyhead == 4){
      inity = inity+1;
    }

      //times = 0;

    while(temp -> next != NULL){
      temp = temp -> next;
    }

    while(temp != NULL){
      if(temp->prev == NULL){
        mvwprintw(win1,temp->y,temp->x,"0");

        temp->x = initx;
        temp->y = inity;
        temp = temp->prev;

      }else{
        mvwprintw(win1,temp->y,temp->x,"0");

        temp->x = temp->prev->x;
        temp->y = temp->prev->y;
        temp = temp->prev;
      }
    }

    while(head != NULL){
      mvwprintw(win1,head->y,head->x,(head->data));
      head = head->next;

    }

    wrefresh(win1);

    usleep(80000); //fast -> sleep

    // end = clock();
    // double result = end-start;
    // times += (result/ CLOCKS_PER_SEC);
    //printf("%d\n", times);

  }

Node* gateMove(Node *headernode){

    Node* head = headernode;
    if(gatestart_x == 0||gatestart_x == 29||gatestart_y == 0||gatestart_y == 39){ //start = wall
      if(gateend_x == 0){ //left -> right

        initx = gateend_x;
        inity = gateend_y;

        head->x = gateend_x;
        head->x = gateend_y;

        keyhead = 2;
      }else if(gateend_y == 29){ //right ->left
        initx = gateend_x;
        inity = gateend_y;

        head->x = gateend_x;
        head->y = gateend_y;

        keyhead = 1;
      }else if(gateend_y == 0){ //up-> down
        initx = gateend_x;
        inity = gateend_y;

        head->x = gateend_x;
        head->y = gateend_y;

        keyhead = 4;
      }else if(gateend_y == 39){ //down -> up
        initx = gateend_x;
        inity = gateend_y;

        head->x = gateend_x;
        head->y = gateend_y;

        keyhead = 3;
      }else{
        initx = gateend_x;
        inity = gateend_y;

        head->x = gateend_x;
        head->y = gateend_y;
      }
    }
    // }else{ //start = not wall
    //   if(){
    //     initx = gateend_x;
    //     inity = gateend_y;
    //
    //     head->x = gateend_x;
    //     head->y = gateend_y;
    //
    //     keyhead = 2; //right
    //   }else if(keyhead == 2){ //go right
    //
    //   }
    // }


    return head;
  }

};



int main(){

  Node *head = NULL;

  head = insertfirstNode(head,"4",initx+2,inity);
  head = insertfirstNode(head,"4",initx+1,inity);
  head = insertfirstNode(head,"3",initx,inity);

  Game game;
  game.snake_make(head);

  init_keyboard();

  while(1){
    if(_kbhit()){
      int input = _getch();
      game.snake_point(input);
      usleep(500); //0.5 tick
    }else{
      game.snake_move(head);

      if(head->x == game.x_item&&head->y == game.y_item){
        head = insertNode(head,"4");
        game.x_item = 0;
        game.y_item = 0;
      }else if(head->x == game.mx_item&&head->y == game.my_item){
        head = deleteNode(head);
        game.mx_item = 0;
        game.my_item = 0;
      }else if(head->x == game.gatestart_x&&head->y == game.gatestart_y){ //gate
        head = game.gateMove(head);
      }
    }
  }
  return 0;
}
