#pragma once

#include "util.h"

/**
 * @file queue_ll.h
 * @brief Queue implemented using Linked list
 *
 * @note keep track of head and tail nodes
 * @note enqueue - add nodes at tail
 * @note dequeue - remove nodes from head
 */


/// Struct to represent the Queue
typedef struct queuell {
  /// Insert a value at the tail of linked list
  bool (* enqueue)(struct queuell *self, etype_t etype, void *value);

  /// Pop the value at the head of the linked list
  element_t (* dequeue)(struct queuell *self);

  /// Peek at the value at head of linked list
  element_t (* peek)(const struct queuell *self);

  /// Is stack empty?
  bool (* is_empty)(const struct queuell *self);

  node_t *head;     ///< Head node of linked list - front of queue
  node_t *tail;     ///< Tail node of linked list - rear of queue
  int size;         ///< No of nodes in the queue
} queuell_t;



// ======================
// Function Declarations
// ======================
/**
 * @brief Allocate memory for the queuell_t struct and initialized the memebers
 * @return pointer to initialized queuell_t or NULL on error
 */
queuell_t* queuell_init();

/**
 * @brief Free the Queue's memory and set the pointer to NULL
 * @param **queue - stack pointer to stack pointer
 */
void queuell_free(queuell_t **queue);
