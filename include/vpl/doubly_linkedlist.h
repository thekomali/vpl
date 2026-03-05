#pragma once

#include "util.h"


/**
 * @file doubly_linkedlist.h
 * @brief Doubly Linked List Implementation
 *
 * @note node contains => data + prev_ptr + next_ptr
 * @note tail node reference is tracked (makes pop() operates at O(1) time)
 * @note size is used to track no of nodes
 */


/// Struct to represent a Doubly Linked List
typedef struct dlinkedlist{
  /// Get the node at the index position
  dnode_t* (* get)(struct dlinkedlist *self, int index);

  /// Append the data at the end of the list
  int (* append)(struct dlinkedlist *self, etype_t etype, void *val);

  /// Insert the data at the index positon
  bool (* insert)(struct dlinkedlist *self, int index, etype_t etype, void *val);

  /// Pop the last data
  dnode_t* (* pop)(struct dlinkedlist *self);

  /// Remove the data at the index position
  bool (* remove)(struct dlinkedlist *self, etype_t etype, void *val);

  /// Get the index of first occurrence of the given value
  int (* index)(struct dlinkedlist *self, etype_t etype, void *val);

  /// Reverse the list
  void (* reverse)(struct dlinkedlist *self);

  dnode_t *head;       ///< reference to head node
  dnode_t *tail;       ///< reference to tail node
  int size;            ///< no of nodes in a linked list
}dlinkedlist_t;


// ============================================
// Function Declarations
// ============================================
/**
 * @brief Allocated memory for dlinkedlist_t struct
 * @return Pointer to initialized linkedlist_t struct or NULL on error
 */
dlinkedlist_t* dlinkedlist_init();

/**
 * @brief Free the dlinkedlist_t's memory and set it to NULL pointer
 * @param **ll - pointer to pointer to dlinkedlist_t struct
 */
void dlinkedlist_free(dlinkedlist_t **dll);
