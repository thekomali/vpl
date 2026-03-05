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



/// Struct to represent a single value
/// Make sure in struct, largest members are placed first followed by smallest members
/// so that the overall size of struct is minimized by avoiding padding spaces
typedef struct {
  union {                  ///< holds data of any one of the type
    int    ival;
    double dval;
    char   *sval;
  } value;

  etype_t etype;           ///< determines the type of data in the union
} element_t;



/// Struct to represent a single node in the single linked list
typedef struct node{
  struct node *next;       ///< pointer refers to next node
  element_t data;          ///< data memeber of the node
} node_t;



/// Struct to represent a single node in the doubly linked list
typedef struct dnode{
  struct dnode *next;      ///< pointer refers to next node (8 byte)
  struct dnode *prev;      ///< pointer refers to previous node (8 byte)
  element_t data;          ///< data memeber of the node (12  byte)
} dnode_t;
