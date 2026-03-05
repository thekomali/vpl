#pragma once

#include "util.h"


/**
 * @brief Struct to represent the stack
 *
 * Function pointer members consume more memory (8 bytes each), so they
 * should be placed first, followed by members that consume less memory
 */
typedef struct stack {
  /// Insert value at the top position
  bool (* push)(struct stack *self, etype_t etype, void *value);

  /// Pop the element at top positon
  element_t (* pop)(struct stack *self);

  /// Peek at the element at top position
  element_t (* peek)(struct stack *self);

  /// Is stack empty?
  bool (* is_empty)(struct stack *self);

  /// Is stack full?
  bool (* is_full)(struct stack *self);

  element_t *data;   ///< array of element_t structs to hold the data
  int top;           ///< point the index of top element
  int capacity;      ///< total elements the stack can hold
} stack_t;



// ======================
// Function Declarations
// ======================

/// Allocate memory for the stack_t struct to hold the specified capacity of elements.
stack_t* stack_init(int capacity);

/// Free the stack's memory and set the pointer to NULL
void stack_free(stack_t **stack);
