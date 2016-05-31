/*
Header File for LL_ADT
*/
#ifndef ASS_SYMBOLS_H
#define ASS_SYMBOLS_H

#include <stdint.h>
#include <stdlib.h>

#define VAL uint64_t
#define KEY char*

typedef struct node node;

struct node {
  node *prev;
  node *next;
  KEY key;
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
void insertNode(list *l, node *iter, KEY key, VAL val);
node* nextNode(node *iter);
void printData(node *curr);
KEY getKey(node *curr);
VAL getVal(node *curr);
VAL getValFromStruct(struct list *symbolsTable, KEY key);
uint8_t equals(KEY key1, KEY key2);
uint8_t listIsInternal(node *curr);
void insertFront(list *l, KEY key, VAL val);
void insertBack(list *l, KEY key, VAL val);
void destroyList(list *l);
#endif
