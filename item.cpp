#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cstdlib>

using namespace std;

typedef struct itemNode{
  int x;
  int y;
  struct  itemNode *next;
  struct  itemNode *prev;
  int type;
}itemNode;

itemNode* makeItem(itemNode *headernode) {

  itemNode *itemnode;
  itemnode = (itemNode *)malloc(sizeof(itemNode));
  itemnode-> next = NULL;
  itemnode-> prev = NULL;

  //srand(time(NULL));
  int pos_x, pos_y;

  pos_x = rand() % 29 + 1; // x좌표 생성기
  pos_y = rand() % 39 + 1; // y좌표 생성기

  itemnode-> x = pos_x;
  itemnode-> y = pos_y;

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

int itemx(itemNode *headernode){
    itemNode *itemnode = headernode;
    return headernode->x;
}

int itemy(itemNode *headernode){
    itemNode *itemnode = headernode;
    return headernode->y;
}

itemNode* deleteItem(itemNode *headernode, int x, int y) {

  itemNode* head = headernode;
  itemNode* pre_head = NULL;

  while(head->x != x||head->y != y){
    head = head -> next;
  }

  //printf("head->x = %d\n", head->x);

  if(head -> prev != NULL){ //not first node
    pre_head = head -> prev;
  }else{
    pre_head = head -> next;
  }

  if(head -> next == NULL){ //head = end
    pre_head -> next = NULL;
    free(head);
  }else if(head -> prev == NULL){ //head = first
    pre_head -> prev = NULL;
    headernode =  pre_head;
    free(head);
  }else{
    head->prev->next = pre_head;
    pre_head->prev = head->prev;
    pre_head -> next = head -> next;
    head -> next -> prev = pre_head;
    free(head);
  }

  return headernode;

}

void print(itemNode *headernode){ //check

  itemNode* head = headernode;

  while(head != NULL){
    cout << head->x << ", " << head->y << endl;
    head = head->next;
  }

}

  int main() {

    itemNode *headernode = NULL;

    headernode = makeItem(headernode);
    headernode = makeItem(headernode);
    headernode = makeItem(headernode);
    headernode = makeItem(headernode);

    print(headernode);

    headernode = deleteItem(headernode,itemx(headernode),itemy(headernode));

    printf("______________________________\n");

    print(headernode);


  }
