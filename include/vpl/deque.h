#pragma once

#include "util.h"

/**
 * @file deque.h
 * @brief Double Ended Queue implementation using circular array
 *
 * flexiable array members is used instead of pointer of arrays
 * so that memory is allocated along with the struct and each element is continuous
 * which allows CPU to access data at faster rate, without hopping to different
 * memory locations pointed by the array of pointers
 *
 * @note enqueue_front - decrement front and add element
 * @note enqueue_rear  - increment rear and add element
 * @note dequeue_front - remove element and increment front
 * @note dequeue_rear  - remove element and decrement rear
 *
 * @note rear + 1 == front            ; queue full
 * @note rear == -1  && front == -1   ; queue is empty
 * @note rear == front                ; queue has only one element
 */


/// Struct to represent the Deque
typedef struct deque {
  /// Insert a value at front of queue
  bool (* enqueue_front)(struct deque *self, etype_t etype, void *value);

  /// Insert a value at rear of queue
  bool (* enqueue_rear)(struct deque *self, etype_t etype, void *value);

  /// Pop a value from the front of the queue
  element_t (* dequeue_front)(struct deque *self);

  /// Pop a value from the rear of the queue
  element_t (* dequeue_rear)(struct deque *self);

  /// Peek at the front value
  const element_t* (* peek_front)(struct deque *self);

  /// Peek at the rear value
  const element_t* (* peek_rear)(struct deque *self);

  /// Is queue empty?
  bool (* is_empty)(struct deque *self);

  /// Is queue full?
  bool (* is_full)(struct deque *self);

  int front;         ///< front of queue - elements are removed here
  int rear;          ///< rear of queue - elements are added here
  int size;          ///< No of elements in queue
  int capacity;      ///< Total no of elements this queue can hold
  element_t data[];  ///< element_t array to hold the data
} deque_t;


// ======================
// Function Declarations
// ======================
/**
 * @brief Allocate memory for the deque_t struct and initialized the memebers
 *
 * @param capacity - size of the deque
 * @return pointer to initialized deque_t or NULL on error
 */
deque_t* deque_init(int capacity);

/**
 * @brief Free the Deque's memory and set the pointer to NULL
 * @param **queue - Deque pointer to pointer
 */
void deque_free(deque_t **deque);
