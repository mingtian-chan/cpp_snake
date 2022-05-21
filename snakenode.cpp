#include <stdio.h>
#include <stdio.h>
#include <string>
#include <cstdlib>

using namespace std;

typedef struct Node{
  char data;
  struct Node *next;
  struct  Node *prev;
  int x;
  int y;
}Node;

int initx = 13;
int inity = 20;

Node* insertfirstNode(Node *headernode,char num){

  Node *plusnode;
  plusnode = (Node *)malloc(sizeof(Node));

  plusnode->x = initx;
  plusnode->y = inity;
  plusnode->prev = NULL;
  plusnode->next = headernode;
  plusnode->data = num;

  headernode = plusnode;

  return headernode;
}



Node* insertNode(Node *headernode,char num){
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

void print(Node *headernode){
  Node *temp = headernode;
  while(temp != NULL){
    printf("%c ", temp->data);
    temp = temp->next;
  }
  printf("\n");

}

int main(void){
  Node *head = NULL;

  head = insertfirstNode(head,'4');
  head = insertfirstNode(head,'4');
  head = insertfirstNode(head,'3');
  head = insertNode(head,'4');
  print(head);

}
