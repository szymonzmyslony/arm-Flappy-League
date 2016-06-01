#include <stdio.h>
#include <assert.h>
#include <inttypes.h>
#include "ass_symbols.h"
#define NDEBUG

//Initialises the list such that there are only head and tail nodes.
void initialiseList(list *l) {
  l->head = allocateNode();
  l->tail = allocateNode();
  l->head->prev = NULL;
  l->head->next = l->tail;
  l->tail->next = NULL;
  l->tail->prev = l->head;
}

//Creates a space in heap for a node using dynamic memory allocation
node* allocateNode(void) {
  node *newNode = malloc(sizeof(node));
  if (newNode == NULL) {
    perror("NodeAllocationFailure!");
    exit(EXIT_FAILURE);
  }
  return newNode;
}

//Frees space previously occupied by the node "curr"
void freeNode(node *curr) {
  free(curr);
}

//Returns a pointer to the first node after head of the list
node* startList(list *l) {
  return l->head->next;
}

//Returns a pointer to the tail of the list
node* endList(list *l) {
  return l->tail;
}

//Inserts a node with key and value before the node iter in the list
void insertNode(list *l, node *iter, KEY key, VAL val) {
  node *newNode    = allocateNode();
  newNode->key     = key;
  newNode->val     = val;
  newNode->prev    = iter->prev;
  newNode->next    = iter;
  iter->prev->next = newNode;
  iter->prev       = newNode;
}

//Returns the next node of the curr node
node* nextNode(node *curr) {
  return curr->next;
}

//Returns the vgalue of the curr node
VAL getVal(node *curr) {
  assert (listIsInternal(curr));
  NDEBUG
  return curr->val;
}

//Returns the value of the node corresponding to the key in the symbol table
//assuming the key and the corresponding key is in the list
VAL getValFromStruct(list *symbolsTable, KEY key){
	node *curNode = startList(symbolsTable);
	while (equals(getKey(curNode), key) != 1) {
		curNode = curNode->next;
	}
	return getVal(curNode);
}

//Returns the key corresponding to the curr node
KEY getKey(node *curr) {
  assert (listIsInternal(curr));
  NDEBUG
  return curr->key;
}

//Checks whether or not the given node curr is in the list. Returns one if it is
//in the list and returns 0 otherwise
uint8_t listIsInternal(node *curr) {
  return (curr->prev != NULL) && (curr->next != NULL);
}

//Inserts a new node with key and value just after the head of the list
void insertFront(list *l, KEY key, VAL val) {
  insertNode( l, startList(l), key, val);
}

//Inserts a new node with key and value just before the tail of the list
void insertBack(list *l, KEY key, VAL val) {
  insertNode( l, endList(l), key, val);
}

//Checks weather two keys, key1 and key2, are equal. Returns 1 if they are
//equal and returns 0 otherwise
uint8_t equals(KEY key1, KEY key2) {
  int comparator = key1 - key2;
  if (comparator == 0) {
    return 1;
  } else {
    return 0;
  }
}

//Iteratively frees the memory spaces occupied by the nodes in the list
void destroyList(list *l) {
  node *curr = l->head;
  while (curr != NULL){
    node *next = nextNode(curr);
    freeNode(next);
    curr = next;
  }
}

//Prints the key and value of given node curr
void printData(node *curr) {
  printf("Key: %s and Val: %" PRIu64 "\n", curr->key, curr->val);
}

//Iteratively prints data of all nodes in the list
void printAll(list *l) {
  node *curr = startList(l);
  node *tail = endList(l);
  while (curr != tail) {
    printData(curr);
    curr = nextNode(curr);
  }
}
