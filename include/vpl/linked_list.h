#pragma once

#include "util.h"


/**
 * @file linked_list.h
 * @brief Single Linked List Implementation
 *
 * @note node contains => data + next_ptr
 * @note size is used to track no of nodes
 * @note tail node is tracked to have O(1) time for append operation
 */


/// Struct to represent a linked list
typedef struct linkedlist{
  /// Get the node at the index positon
  node_t* (* get)(struct linkedlist *self, int index);

  /// Insert value at the end of the linked list
  int (* append)(struct linkedlist *self, etype_t etype, void *val);

  /// Insert value at the index positon
  bool (* insert)(struct linkedlist *self, int index, etype_t etype, void *val);

  /// Pop the last value
  node_t* (* pop)(struct linkedlist *self);

  /// Remove the data at the index positon
  bool (* remove)(struct linkedlist *self, etype_t etype, void *val);

  /// Get the index of first occurrence of the given value
  int (* index)(struct linkedlist *self, etype_t etype, void *val);

  /// Reversee the linked list
  void (* reverse)(struct linkedlist *self);

  node_t *head;        ///< reference to head node
  node_t *tail;        ///< reference to tail node
  int size;            ///< no of nodes in a linked list
}linkedlist_t;


// ============================================
// Function Declarations
// ============================================
/**
 * @brief Allocated memory for linkedlist_t struct
 * @return Pointer to initialized linkedlist_t struct or NULL on error
 */
linkedlist_t* linkedlist_init();

/**
 * @brief Free the linkedlist_t's memory and set it to NULL pointer
 * @param **ll - pointer to pointer to linkedlist_t struct
 */
void linkedlist_free(linkedlist_t **ll);
