#include <stdio.h>
#include <assert.h>
#include "ass_symbols.h"
#define NDEBUG

void initialiseList(list *l) {
  l->head = allocateNode();
  l->tail = allocateNode();
  l->head->prev = NULL;
  l->head->next = l->tail;
  l->tail->next = NULL;
  l->tail->prev = l->head;
}

node* allocateNode(void) {
  node *newNode = malloc(sizeof(node));
  if (newNode == NULL) {
    perror("NodeAllocationFailure!");
    exit(EXIT_FAILURE);
  }
  return newNode;
}

void freeNode(node *curr) {
  free(curr);
}

node* startList(list *l) {
  return l->head->next;
}

node* endList(list *l) {
  return l->tail;
}

void insertNode(list *l, node *iter, KEY key, VAL val) {
  node *newNode    = allocateNode();
  newNode->key     = key;
  newNode->val     = val;
  newNode->prev    = iter->prev;
  newNode->next    = iter;
  iter->prev->next = newNode;
  iter->prev       = newNode;
}

node* nextNode(node *iter) {
  return iter->next;
}

VAL getVal(node *curr) {
  assert (listIsInternal(curr));
  NDEBUG
  return curr->val;
}

//gets value from the symbol table based on on the key
VAL getValFromStruct(struct list *symbolsTable, KEY key){
	// we assume key is in the structure, no need to checking whether key is null
	node *curNode = startList(symbolsTable);
	while (equals(getKey(curNode), key) != 1) {
		curNode = curNode->next;
	}
	return getVal(curNode);
}

KEY getKey(node *curr) {
  assert (listIsInternal(curr));
  NDEBUG
  return curr->key;
}

uint8_t listIsInternal(node *curr) {
  return (curr->prev != NULL) && (curr->next != NULL);
}

void insertFront(list *l, KEY key, VAL val) {
  insertNode( l, startList(l), key, val);
}

void insertBack(list *l, KEY key, VAL val) {
  insertNode( l, endList(l), key, val);
}

uint8_t equals(KEY key1, KEY key2) {
  int comparator = key1 - key2;
  if (comparator == 0) {
    return 1;
  } else {
    return 0;
  }
}

void destroyList(list *l) {
  node *curr = l->head;
  while (curr != NULL){
    node *next = curr->next;
    freeNode(next);
    curr = next;
  }
}

void printData(node *curr) {
  printf("Key: %s and Val: %d \n", curr->key, curr->val);
}
