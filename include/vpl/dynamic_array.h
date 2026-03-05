#pragma once

#include "util.h"


/**
 * @file dynamic_array.h
 * @brief Dynamic Array (heterogenous data types)
 *
 * @note array size doubles every time it fills up
 * @note element_t struct is used to hold different types of data
 */


/// Struct to represent the dynamic array
typedef struct darray {
  /// Get the data at the index position
  element_t* (* get)(struct darray *self, int index);

  /// Append the data at the end of the array
  int (* append)(struct darray *self, etype_t etype, void *value);

  /// Insert the data at the index position
  /// Automatically resize in case the size equals capacity
  /// Move all the data from insert positon to right
  bool (* insert)(struct darray *self, int index, etype_t etype, void *value);

  /// Pop the last data
  element_t* (* pop)(struct darray *self);

  /// Remove the first occurrence of the data in the array
  bool (* remove)(struct darray *self, etype_t etype, void *value);

  /// Get the index of first occurrence of the data, -1 if not present
  int (* index)(struct darray *self, etype_t etype, void *value);

  /// Reverse the array
  void (* reverse)(struct darray *self);

  element_t **arr;       ///< array that holds pointers to element_t
  size_t capacity;       ///< no of elements the array can hold (resizes automatically)
  int size;              ///< no of elements in array
} darray_t;



// ============================================
// Function Declarations
// ============================================
/**
 * @brief Allocate memory for darray_t and initialize with value
 * @params capacity - initial size of the array to allocate
 * @return darray_t - reference to newly created darray_t
 */
darray_t* darray_init(size_t capacity);

/**
 * @brief Free the dynamic array along with individual elements
 * @params **da - reference of reference to darray_t
 */
void darray_free(darray_t **da);
