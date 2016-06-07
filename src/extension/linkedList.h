/*
Header File for linked list ADT
*/
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "Tminus.h"

#define VAL GameObject

typedef struct node node;

struct node {
  node *prev;
  node *next;
  VAL val;
};

typedef struct list list;

struct list {
  node *head;
  node *tail;
};

void initialiseList(list *l);
node* allocateNode(void);
void freeNode(node *curr);
node* startList(list *l);
node* endList(list *l);
void insertNode(list *l, node *iter, VAL val);
node* nextNode(node *iter);
VAL getVal(node *curr);
uint8_t listIsInternal(node *curr);
void insertFront(list *l, VAL val);
void insertBack(list *l, VAL val);
void destroyList(list *l);
#endif
