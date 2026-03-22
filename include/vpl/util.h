#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

// ======================
// MACROS
// ======================
/// do..while block is used not to provide looping but, it's
/// a trick used to consider all the statments in one block
#define SWAP(a, b) \
  do { \
    __typeof__(a) _tmp = (a); \
    (a) = (b);  \
    (b) = _tmp; \
  } while (0)



// ======================
// TYPE DECLERATIONS
// ======================

/// Enum to identify the type of value in the array
typedef enum {INT, DOU, STR} etype_t;



/// Struct to represent a Data
typedef struct {
  union {                  ///< holds data of any one of the type (12 bytes)
    int    ival;
    double dval;
    char   *sval;
  } value;
                           ///< padding is added after union (4 bytes)
  etype_t etype;           ///< determines the type of data in the union (4 bytes)
} element_t;               ///< 12 + 4 + 4 = 16 bytes



/// Struct to represent a Node in Linked list
typedef struct node{
  struct node *next;       ///< pointer refers to next node (8 byte)
  element_t data;          ///< data memeber of the node (16 byte)
} node_t;                  ///< 8 + 16 = 24 bytes (multiple of 8 - no padding needed)



/// Struct to represent a Node in Doubly linked list
typedef struct dnode{
  struct dnode *next;      ///< pointer refers to next node (8 byte)
  struct dnode *prev;      ///< pointer refers to previous node (8 byte)
  element_t data;          ///< data memeber of the node (16  byte)
} dnode_t;



/// Struct to represent a Node in a Tree data structure
typedef struct TNode {
  struct TNode *left;      ///< pointer refers to left node of tree (8 bytes)
  struct TNode *right;     ///< pointer refers to right node of tree (8 bytes)
  element_t data;          ///< data member of the node (16 bytes)
} TNode;                   ///< 16 + 8 + 8 = 32 bytes (multiple of 8 - no padding needed)
