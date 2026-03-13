#pragma once

#include "util.h"

/**
 * @file queue.h
 * @brief Queue Implementation using Array (Circular Queue)
 *
 * Using flexible array memebers, so that memory is continuous with the struct
 * and it's easy for caching and avoid fragmentation, which will happen if
 * array of element_t pointers is used.
 *
 * @note enqueue - increment rear and add element
 * @note dequeue - remove element and decrement front
 *
 * @note rear == front == -1 ; queue is is_empty
 * @note rear == front != -1 ; queue has only one element
 * @note rear + 1 == front   ; queue is full
 */



/// Struct to represent the Circular Queue
typedef struct queue {
  /// Insert a value at the rear part
  bool (* enqueue)(struct queue *self, etype_t etype, void *value);

  /// Pop the value at the front part
  element_t (* dequeue)(struct queue *self);

  /// peek at the front part
  const element_t* (* peek)(struct queue *self);

  /// Is queue empty?
  bool (* is_empty)(struct queue *self);

  /// Is queue full?
  bool (* is_full)(struct queue *self);

  int front;         ///< front of queue - elements are removed here
  int rear;          ///< rear of queue - elements are added here
  int size;          ///< No of elements in queue
  int capacity;      ///< Total no of elements this queue can hold
  element_t data[];  ///< element_t array to hold the data
                     ///< flexible arrays members should be the last member
} queue_t;



// ======================
// Function Declarations
// ======================
/**
 * @brief Allocate memory for the queue_t struct and initialized the memebers
 *
 * @param capacity - size of the queue
 * @return pointer to initialized queue_t or NULL on error
 */
queue_t* queue_init(int capacity);

/**
 * @brief Free the Queue's memory and set the pointer to NULL
 * @param **queue - Queue pointer to pointer
 */
void queue_free(queue_t **queue);
