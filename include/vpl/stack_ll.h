#pragma once

#include "util.h"

/**
 * @file stackll.h
 * @brief Stack implemented using Linked List
 *
 * Heterogenous stack -  contain values of different data type
 * Linked list based stack don't need top to keep track of latest element
 * @note head node act as top
 * @note push() - update new node as head
 * @note pop()  - remove the node at head
 */


/// Struct to represent the stack  (function pointer members consume more memory
///   (8 bytes each), so they should be placed first, followed by members that
///   consume less memory)
typedef struct stackll {
  /// Insert value at the top position
  bool (* push)(struct stackll *self, etype_t etype, void *value);

  /// Pop the element at top position
  element_t (* pop)(struct stackll *self);

  /// Peek at the element at top position
  element_t  (* peek)(struct stackll *self);

  /// Is stack empty?
  bool (* is_empty)(struct stackll *self);

  node_t *head;      ///< Head node of the linked list (act as top)
  int size;          ///< Specify no of nodes in stack
} stackll_t;



// ======================
// Function Declarations
// ======================
/**
 * @brief Allocate memory for the stackll_t struct to hold the specified capacity of elements.
 * @return pointer to initialized stack_t or NULL on error
 */
stackll_t* stackll_init();

/**
 * @brief Free the stack's memory and set the pointer to NULL
 * @param **stack - stack pointer to stack pointer
 */
void stackll_free(stackll_t **stack);
