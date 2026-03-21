#pragma once

#include "util.h"

/**
 * @file deque_ll.h
 * @brief Double Ended Queue implementation using doubly linked list
 *
 * @note front - head of the linked list
 * @note rear  - tail of the linked list
 */


/// Struct to represent the deque
typedef struct DequeLL{
  /// Insert a value at front of deque
  bool (* enqueue_front)(struct DequeLL *self, etype_t etype, void *value);

  /// Insert a value at rear of deque
  bool (* enqueue_rear)(struct DequeLL *self, etype_t etype, void *value);

  /// Pop a value from the front of the deque
  element_t (* dequeue_front)(struct DequeLL *self);

  /// Pop a value from the rear of the deque
  element_t (* dequeue_rear)(struct DequeLL *self);

  /// Peek at the front value
  const element_t* (* peek_front)(struct DequeLL *self);

  /// Peek at the rear value
  const element_t* (* peek_rear)(struct DequeLL *self);

  /// Is the deque empty?
  bool (* is_empty)(struct DequeLL *self);

  dnode_t *head;     ///< front of the deque
  dnode_t *tail;     ///< rear of the queue
  int size;          ///< no of elements in queue
} DequeLL;


// ======================
// Function Declarations
// ======================
/**
 * @brief Allocate memory for the DequeLL_t struct and initialized the memebers
 *
 * @return pointer to initialized DequeLL_t or NULL on error
 */
DequeLL* dequell_init();

/**
 * @brief Free the Deque's memory and set the pointer to NULL
 * @param **queue - Deque pointer to pointer
 */
void dequell_free(DequeLL **deque);
