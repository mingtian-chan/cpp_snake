#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <string>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>
#include <iostream>
#include <string>

#define Max 5 //?

struct termios oldterm, new_term;
int ch_read;


void set_stdin(void);
void reset_stdin(void);
int kgetch();

using namespace std;

typedef struct Node{ //뱀을 구현하기 위한 연결리스트
  const char* data; //뱀에 담겨있는 const char* 숫자 정보
  struct Node *next; //노드를 연결하여 뱀을 구현하기 위한 next node
  struct  Node *prev; //노드를 연결하여 뱀을 구현하기 위한 prev node
  int x; //각 뱀 정보들의 x좌표
  int y; //각 뱀 정보들의 y좌표
}Node;

typedef struct itemNode{
  int x;
  int y;
  struct  itemNode *next;
  struct  itemNode *prev;
  const char* item;
  int itemTime = 0;
}itemNode;

typedef struct wallNode{
  int x;
  int y;
  struct  wallNode *next;
  struct  wallNode *prev;
  const char* item = "1";
}wallNode;

static struct termios initial_settings, new_settings;
static int peek_character = -1;

void init_keyboard() //linux 코드 가져온것, keyboard 입력을 사용하기 위해 초기화
{
    tcgetattr(0,&initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
}

int _kbhit() //linux에는 window cohit이 없으므로, 이렇게 따로 정의한 함수를 사용해야 한다.
//ncurses에 있는 getch 함수의 경우, 무조건 wait한다는 특성이 있기 때문에 이를 사용할시, 뱀이 연속적으로 움직일 수 없다.
//따라서 다른 사람이 linux 용으로 구현해둔 다음의 kbhit 함수를 따와서 이용한다.
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

//wait 기능이 없는 getch를 사용하기 위해, 다음과 같은 외부 정의 함수를 따로 사용한다.

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

int initx = 10; //초기에 뱀의 head가 위치하는 x값
int inity = 10; //초기에 뱀의 head가 위치하는 y값



class Game{
public :
  WINDOW *win1;
  WINDOW *win2;
  WINDOW *score;
  WINDOW *mission;

  int i = 0;
  int j = 0;

  int keyhead = 1;
  int prekeyhead = keyhead;
  int TrueCounter = 1;
  double times = 0.0;
  int lengthNode = 0;

  int x_item = 18; //임의로 지정한 플러스 아이템의 위치 좌표
  int y_item = 20;

  int mx_item = 5; //임의로 지정한 마이너스 아이템의 위치 좌표
  int my_item = 10;


  int wallx[10] = {7,7,7,7,7,7,8,9,10,11}; //walll
  int wally[10] = {20,21,22,23,24,25,25,25,25,25};

  const char* map1[21][22] = {
    {"2", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "2"},
    {"1", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "1"},
    {"1", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "1"},
    {"1", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "1"},
    {"1", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "1"},
    {"1", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "1"},
    {"1", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "1"},
    {"1", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "1"},
    {"1", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "1"},
    {"1", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "1"},
    {"1", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "1"},
    {"1", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "1"},
    {"1", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "1"},
    {"1", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "1"},
    {"1", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "1"},
    {"1", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "1"},
    {"1", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "1"},
    {"1", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "1"},
    {"1", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "1"},
    {"1", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "1"},
    {"2", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "2"},
  };

  int gate_truex[100]{};
  int gate_truey[100]{};

  int randnum = 0;

  int gateend_x; //#4-8
  int gateend_y; //#4-8

  int gatestart_x; //#4-8
  int gatestart_y; //#4-8

  int length = 0;
  //int score_int = 0;

  int grow_score_int = 0;
  int pois_score_int = 0;
  int gate_score_int = 0;

  

  Game(){
     //tetris
    initscr();
    resize_term(60,50);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    curs_set(0);

    border('|','|','-','-','+','+','+','+');

    refresh();
    //getch();

    win1 = newwin(21,22,4,4); //size, x, y
    //init_pair(2, COLOR_GREEN, COLOR_BLACK);

    //wattron(win1, COLOR_PAIR(2));

    for(int i = 0; i<21; i++){
      for(int j = 0; j<22; j++){
        mvwprintw(win1,i,j,map1[i][j]);
      }
    }

    wrefresh(win1);
    ////////fail!/////////////////////
    win2 = newwin(40,30,3,3); //size, x, y

    wborder(win2,'|','|','-','-','+','+','+','+');
    //init_pair(2, COLOR_GREEN, COLOR_BLACK);

    wattron(win2, COLOR_PAIR(2));
    wbkgd(win2,'0');
    wattroff(win2, COLOR_PAIR(2));


    mvwprintw(win2,0,5,"FAIL");

    ////////fail!/////////////////////

    ///score
    score  = newwin(6,30,4,30);
    wborder(score,'|','|','-','-','+','+','+','+');
    mvwprintw(score,0,4,"score");

    wrefresh(score);


    /// mission
    mission = newwin(6, 30, 16,30);
    wborder(mission,'|','|','-','-','+','+','+','+');
    mvwprintw(mission,0,4,"mission");
    mvwprintw(mission,2,3,"Grow   : ( )/3");
    mvwprintw(mission,3,3,"poison : ( )/2");
    mvwprintw(mission,4,3,"Gate   : ( )/1"); 
    
    wrefresh(mission);

  }
  //snake
  void grow_score_print(){
    const char* grow_score_char = to_string(grow_score_int).c_str();
    mvwprintw(score,2,3,"Grow   : ");
    mvwprintw(score,2,12,grow_score_char);
    wrefresh(score);
  }
  void pois_score_print(){
    const char* pois_score_char = to_string(pois_score_int).c_str();
    mvwprintw(score,3,3,"Poison : ");
    mvwprintw(score,3,12,pois_score_char);
    wrefresh(score);
  }
  void gate_score_print(){
    const char* gate_score_char = to_string(gate_score_int).c_str();
    mvwprintw(score,4,3,"Gate   : ");
    mvwprintw(score,4,12,gate_score_char);
    wrefresh(score);

    // const char* score_char = to_string(score_int).c_str();
    //
    // mvwprintw(score,2,4,score_char);
    // wrefresh(score);
  }

  void grow_mission_print(){ 
    int grow_mission_goal = 3;
    if (grow_score_int >= grow_mission_goal){
      mvwprintw(mission,2,3,"Grow   : (O)/3");
    }
    wrefresh(mission);
  }

  void pois_mission_print(){
    int pois_mission_goal = 2;
    if (pois_score_int >= pois_mission_goal) {
      mvwprintw(mission,3,3,"Poison : (O)/2");
    }
    wrefresh(mission);
  }

  void gate_mission_print(){
    int gate_mission_goal = 1;
        if (gate_score_int >= gate_mission_goal) {
      mvwprintw(mission,4,3,"Gate   : (O)/1");
    }
    wrefresh(mission);
  }

  void fail(){

    //endwin?
    //werase!
    //werase(win1);

    //wclear(win1);
    //wrefresh(win2);

    mvwprintw(win2,15,15,"fail!");
    wrefresh(win2);
    sleep(10);
    //initscr()?

  }

  void set_gate_true(){

    int n = 0;

    gate_truex[100] = {};
    gate_truey[100] = {};

    for(int i = 0; i<22; i++){
      for(int j = 0; j<21; j++){
        if(map1[i][j] == "1"){
          gate_truex[n] = j;
          gate_truey[n] = i;
          n++;
          randnum++;
        }
        //mvwprintw(win1,i+1,j+1,map1[i][j]);
      }
    }
  }

  void set_gate_xy(){

    int start;
    int end;

    srand(time(NULL));

    start = rand() % randnum; // x좌표 생성기
    end = rand() % randnum;

    gatestart_x = gate_truex[start];
    gatestart_y = gate_truey[start];


    gateend_x = gate_truex[end];
    gateend_y = gate_truey[end];

    map1[gatestart_y][gatestart_x] = "$"; //?????
    map1[gateend_y][gateend_x] = "@";

    mvwprintw(win1,gatestart_y,gatestart_x,"$"); //gate start
    mvwprintw(win1,gateend_y, gateend_x,"@"); //gate end
    wrefresh(win1);


  }

  Node* insertfirstNode(Node *headernode,const char *num,int x, int y){ //뱀의 크기를 늘릴때, 앞부분을 늘리기 위한 함수, 초기 뱀을 설정하기 위해 만든 함수이다.

    Node *plusnode;
    plusnode = (Node *)malloc(sizeof(Node));; //node의 메모리 배정

    plusnode->x = x; //새로 배정된 node의 x 좌표값 설정
    plusnode->y = y; //새로 배정된 node의 y 좌표값 설정

    plusnode->prev = NULL; //일단, 초기에는 연결이 이뤄지지 않았으므로 이전 node를 가리키는 prev 정보를 null로 둔다.
    plusnode->next = NULL;
    plusnode->data = num; //해당하는 const char * 정보를 data에 넣는다.

    if(headernode == NULL){ //header가 없으면, 처음 node라는 의미이므로, header로 설정해준다.
      headernode = plusnode;
    }else{
      plusnode->next = headernode; //기존의 header node를 새로운 header가 될 plusnode의 다음으로 순서를 미루기 위해 next로 연결한다.
      headernode->prev = plusnode; //기존의 header node 앞에 새로운 header가 될 plusnode가 와야 하므로, 다음과 같이 설정한다.
      headernode = plusnode; //새로운 plusnode를 headernode로 변경 설정한다.

    }

    lengthNode++;

    return headernode; //headernode를 return 해서 headernode를 연결할 수 있도록 해야한다.
  }

  Node* deleteNode(Node *headernode){ //삭제연산, 줄어드는 아이템을 먹었을때 발생한다.

    Node *plusnode = headernode;
    Node *plusnode2 = headernode;


    while(plusnode -> next != NULL){
      plusnode = plusnode->next; //꼬리 부분을 줄어들게 해야하므로, header를 가리키고 있는 node를 계속 끝으로 이동해준다.
    }

    plusnode2 = plusnode -> prev; //plusnode2가 plusnode의 이전 node를 가리키게 한다.
    plusnode2 -> next = NULL; //마지막 node를 삭제해야 하므로, plusnode2의 연결을 끊는다.

    free(plusnode); //마지막 node의 메모리를 반납한다.

    lengthNode--;
    //score_int--;
    pois_score_int++;

    return headernode; //headernode를 return 해서 연결되도록 한다.
  }

  Node* insertNode(Node *headernode,const char *num){ //증가하는 아이템을 먹었을때를 해결하기 위한 insert 연산이다.
    Node *plus = headernode;

    Node *plusnode;
    plusnode = (Node *)malloc(sizeof(Node)); //새로 추가되는 plus node의 메모리를 배정한다.

    while(plus->next != NULL){
      plus = plus->next; //끝 부분에 추가를 해야 하기 때문에 끝으로 계속 이동한다.
    }

    plusnode->prev = plus; //새로 만든 node의 앞 연결 node를 원래 tailnode로 설정
    plus->next = plusnode; //원래 tail이었던 node뒤에 새로 만든 추가 node를 설정
    plusnode->x = (plus -> x)-1; //뒤로 와야 하기 때문에, x값을 하나 적게 설정
    plusnode->y = (plus -> y); //y값은 그대로 설정
    plusnode->next = NULL; //마지막 node가 되었으므로, next는 null로 설정해준다.
    plusnode->data = num; //data를 넣어준다.

    lengthNode++;
    //score_int++;

    grow_score_int++;

    return headernode; //header를 return 해서 연결해준다.
  }

  void snake_make(Node *headernode){ //초기에 생성된 스네이크 mvwprintw로 화면에 출력

    Node* temp = headernode;

    while(temp != NULL){ //마지막 노드 나올때까지
      mvwprintw(win1,temp->y,temp->x,(temp->data)); //타고 가면서 print 하기
      temp = temp->next;
    }

    wrefresh(win1);

  }

  void snake_point(int input){ //key입력으로 들어온 input값에 따라서 head 방향 지정
    prekeyhead = keyhead; //이전 key 방향과 비교하기 위한 변수
    switch (input) {
      case 68 :

      keyhead = 1; //왼쪽 방향키

      break;

      case 67:

      keyhead = 2; //오른쪽 방향키
      break;

      case 65:
      keyhead = 3; //위쪽 방향키
      break;

      case 66:
      keyhead = 4; //아래쪽 방향키
      break;
    }

    //실패조건 (아직 미구현)

}

  void snake_move(Node *headernode){ //snake의 계속되는 이동을 위한 함수
    // clock_t start,end;
    Node* head = headernode;
    Node* temp = headernode;

    if(keyhead == 1){
      initx = initx-1; //왼쪽으로 이동이니까 초기 x좌표에서 -1 해주기
    }else if(keyhead == 2){ //오른쪽으로 이동이니까 초기 x좌표에서 +1 해주기
      initx = initx+1;
    }else if(keyhead == 3){ //위쪽으로 이동이니까 초기 y좌표에서 -1 해주기
      inity = inity-1;
    }else if(keyhead == 4){ //아래쪽으로 이동이니까 초기 y좌표에서 +1 해주기
      inity = inity+1;
    }

    while(temp -> next != NULL){ //tail 부분(끝 부분)으로 이동하기 위함, 꼬리 부분부터 이동이므로
      temp = temp -> next;
    }

    while(temp != NULL){ //있는 node들을 전부 탐색하기 위한 null 조건
      if(temp->prev == NULL){ //첫 node의 경우는, 바뀐 initx를 넣어서 갱신해 줘야 한다.
        mvwprintw(win1,temp->y,temp->x,"0");

        temp->x = initx;
        temp->y = inity;
        temp = temp->prev;

      }else{
        mvwprintw(win1,temp->y,temp->x,"0"); //지나온 자리를 전부 0으로 바꿈

        temp->x = temp->prev->x; //이전에 있던 x,y값으로 변경해서 한칸씩 이동해 줘야 유려한 이동이 가능하다.
        temp->y = temp->prev->y;
        temp = temp->prev;
      }
    }

    while(head != NULL){
      mvwprintw(win1,head->y,head->x,(head->data));//이제 바꾼 x,y값을 바탕으로 mvwprintw하면서 이동
      head = head->next;

    }

    wrefresh(win1);

    //usleep(80000); //fast -> sleep 0.08s

    usleep(300000); //임시 러닝용 0.3s

    // end = clock();
    // double result = end-start;
    // times += (result/ CLOCKS_PER_SEC);
    //printf("%d\n", times);

  }

  //item

itemNode* makeItem(itemNode *headernode, const char* item) {

  length += 1;

  itemNode *itemnode;
  itemnode = (itemNode *)malloc(sizeof(itemNode));
  itemnode-> next = NULL;
  itemnode-> prev = NULL;

  //srand(time(NULL));
  int pos_x, pos_y;

  pos_x = rand() % 19 + 1; // x좌표 생성기
  pos_y = rand() % 20 + 1; // y좌표 생성기

  itemnode-> x = pos_x;
  itemnode-> y = pos_y;

  itemnode-> item = item;

  if(headernode == NULL){
    headernode = itemnode;
  }else{
    itemnode -> next = headernode;
    headernode-> prev = itemnode;
    itemnode -> prev = NULL;
    headernode = itemnode;
  }

  return headernode;

}

void printItem(itemNode *itemnode){ //초기에 생성된 스네이크 mvwprintw로 화면에 출력

  int i = 0;

  itemNode* temp = itemnode;

  while(temp != NULL){ //마지막 노드 나올때까지
    mvwprintw(win1,temp->y,temp->x,(temp->item)); //타고 가면서 print 하기
    temp = temp->next;
    i++;
  }

  wrefresh(win1);

}

itemNode* finditem(itemNode *headernode, int x, int y){
    itemNode *itemnode = headernode;

    while(itemnode-> x != x||itemnode-> y != y){
      itemnode = itemnode->next;
    }

    return itemnode;
}

itemNode* deleteItem(itemNode *headernode, itemNode *itemnode) {

  itemNode* pre_node = NULL;

  const char* itemcopy = itemnode -> item;

  int dx = itemnode -> x;
  int dy = itemnode -> y;

  //printf("head->x = %d\n", head->x);

  if(itemnode -> prev != NULL){ //not first node
    pre_node= itemnode -> prev;
  }else{
    pre_node = itemnode -> next;
  }

  if(itemnode -> next == NULL){ //itemnode = end
    pre_node -> next = NULL;
    free(itemnode);
  }else if(itemnode -> prev == NULL){ //itemnode = first
    pre_node -> prev = NULL;
    headernode =  pre_node;
    free(itemnode);
  }else{
    itemnode->prev->next = pre_node;
    pre_node->prev = itemnode->prev;
    pre_node -> next = itemnode -> next;
    itemnode -> next -> prev = pre_node;
    free(itemnode);
  }

  mvwprintw(win1,dy,dx,"0");

  return headernode;
}

//gate

Node* gateMove(Node *headernode){ //gate를 지날때 header를 바꿔주기 위한 move

    int walltrue_y = 0;
    int walltrue_x = 0;

    Node* head = headernode;

    //std::cout << gatestart_x << std::endl;

    if(gatestart_x == 0||gatestart_x == 20||gatestart_y == 0||gatestart_y == 21){ //end wall이 벽에 있는 경우
      if(gateend_x == 0){ //끝나는 게이트가 x=0 방향으로 열려있어서 오른쪽으로 진출해야 하는 경우 left -> right

        initx = gateend_x; //초기값을 게이트 방향으로 옮겨서, head가 통과할 수 있도록 지정
        inity = gateend_y;

        head->x = gateend_x; //head의 x에 gateend_x 적용
        head->y = gateend_y;

        keyhead = 2; //방향을 오른쪽을 변경
      }else if(gateend_x == 20){ //끝나는 게이트가 x = 오른쪽 방향으로 열려있어서 right ->left 방향으로 왼쪽으로 빠져나가야 하는 경우
        initx = gateend_x;
        inity = gateend_y;

        head->x = gateend_x;
        head->y = gateend_y;

        keyhead = 1; //head 방향을 왼쪽으로 바꿔줌
      }else if(gateend_y == 0){ //끝나는 게이트가 y = 위쪽으로 열려있어서 아래방향으로 빠져나가야 하는 경우 up-> down
        initx = gateend_x;
        inity = gateend_y;

        head->x = gateend_x;
        head->y = gateend_y;

        keyhead = 4; //head 방향을 아래쪽으로 바꿔줌
      }else if(gateend_y == 21){ //끝나는 게이트가 y = 아래쪽으로 열려있어서 위 방향으로 빠져나가야 하는 경우 down -> up
        initx = gateend_x;
        inity = gateend_y;

        head->x = gateend_x;
        head->y = gateend_y;

        keyhead = 3; //방향을 위쪽으로 열어줌
      }else{ //끝나는 게이트가 벽에 붙어있지 않은 경우 = 진행 방향을 바꾸지 않고, head의 x,y만 게이트 쪽으로 열어주면 된다.
        initx = gateend_x;
        inity = gateend_y;

        head->x = gateend_x;
        head->y = gateend_y;
      }
  }else{ //end = not wall //start부분이 벽에 붙어있지 않은경우 (아직 미구현)

    std::cout << walltrue_y << std::endl;

    for(int i = 0; i < 10; i++){ //y = 2
      if(wally[i] == gateend_y -1 ||wally[i] == gateend_y +1){
        walltrue_y += 1;
      }else if(wallx[i] == gateend_x -1 ||wallx[i] == gateend_x +1){
        walltrue_x += 1; //진출방향이 상+하 인경우
      }
    }

    if(walltrue_y == 2){ //나가는 곳에 벽이 위 아래로 막혀 있는 경우 = 진출 방향이 좌+우 인경우
      if(keyhead == 2||keyhead == 3){ //right
        initx = gateend_x;
        inity = gateend_y;

        head->x = gateend_x;
        head->y = gateend_y;

        keyhead = 2; //right
      }else if(keyhead == 4||keyhead == 1){
        initx = gateend_x;
        inity = gateend_y;

        head->x = gateend_x;
        head->y = gateend_y;

        keyhead = 1; //left
      }
    }else if(walltrue_x == 2){
      if(keyhead == 1||keyhead == 3){ //right
        initx = gateend_x;
        inity = gateend_y;

        head->x = gateend_x;
        head->y = gateend_y;

        keyhead = 3; //up
      }else if(keyhead == 2||keyhead == 4){
        initx = gateend_x;
        inity = gateend_y;

        head->x = gateend_x;
        head->y = gateend_y;

        keyhead = 4; //down
      }
    }
  }

    //score_int++;
    gate_score_int++;
    return head;
}
};

//main

int main(){

  Game game;

  Node *head = NULL;
  itemNode *ihead = NULL;
  wallNode *whead = NULL;

  int change = 0;
  int count = 0;

  head = game.insertfirstNode(head,"4",initx+2,inity); //초기 뱀을 insertfirstNode로 만들기
  head = game.insertfirstNode(head,"4",initx+1,inity);
  head = game.insertfirstNode(head,"3",initx,inity);


  game.snake_make(head); //초기 뱀 print

  init_keyboard();

  ihead =  game.makeItem(ihead,"5"); //init?
  ihead =  game.makeItem(ihead,"6");
  game.printItem(ihead);

  game.length = 0;

  game.set_gate_true();
  game.set_gate_xy();

  while(1){ //무한루프, 종료 조건이 만들어진가면 !종료조건으로 넣을 예

    itemNode *itemhead = ihead;
    Node *lenghead = head;
    Node *wellhead = head;


    game.grow_score_print();
    game.pois_score_print();
    game.gate_score_print();
    game.grow_mission_print();
    game.pois_mission_print();
    game.gate_mission_print();

    //fail?

    if((game.prekeyhead == 1&&game.keyhead == 2)||(game.prekeyhead == 2&&game.keyhead == 1)){
      game.fail();
      break;
    }else if((game.prekeyhead == 3&&game.keyhead == 4)||(game.prekeyhead == 4&&game.keyhead == 3)){
      game.fail();
      break;
    }

    if(game.lengthNode < 3){
      game.fail();
      break;
    }

    //std::cout << lengthNode << std::endl;

    if(game.map1[wellhead->y][wellhead->x] == "1"){
      game.fail();
      break;
    }

    while(itemhead != NULL&&game.length <= 2){ //game.length  한번에 생길 수 있는 node의 갯수를 제한
      if(count < 60){
        if(head->x == itemhead-> x&&head->y ==  itemhead -> y){
          itemNode * inode  =  game.finditem(itemhead, itemhead-> x, itemhead -> y);
          const char* data = inode -> item;
          ihead =  game.deleteItem(ihead,inode);

          if(data == "5"){ //minus
            head = game.deleteNode(head);
          }else if(data == "6"){
            head = game.insertNode(head,"4");
          }

          ihead =  game.makeItem(ihead, data);
          itemhead = itemhead->next;
        }else{
          itemhead = itemhead->next;
        }
      }else{

        itemNode * inode  =  game.finditem(itemhead, itemhead-> x, itemhead -> y);
        const char* data = inode -> item;
        ihead =  game.deleteItem(ihead,inode);
        ihead =  game.makeItem(ihead, data);
        itemhead = itemhead->next;

      }

      game.printItem(ihead);
    }

    if(_kbhit()){ //키보드 입력이 들어온다면
      int input = _getch(); //입력값을 int로 받기
      game.snake_point(input); //int 받은 값을 snake_point로 전달해서 방향 변환
      usleep(5000); //헤더 방향을 바꾸는데 들어가는 0.05 tick
    }else{
      game.snake_move(head); //키보드 입력값이 없을 경우 계속 움직이기

      if(head->x == game.gatestart_x&&head->y == game.gatestart_y){ //gate를 만나면
        head = game.gateMove(head); //gateMove를 이용해서 gate조건에 따른 move 실행
      }
    }

    if(count >= 60){
      count = 0;
      game.length = 0;
    }else{
      count += 1;
    }

  }
  return 0;
}