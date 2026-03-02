#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "util.h"


/**
 * @brief Enum to identify the type of value in the array
 */
typedef enum {INT, DOU, STR} etype_t;


/**
 * @brief Struct to represent a single value in the array
 */
typedef struct {
  // determines the value present in the union
  etype_t etype;

  union {
    int    ival;
    double dval;
    char   *sval;
  } value;
} element_t;


/**
 * @brief Struct to represent the dynamic array
 *
 * Contains the function pointers to all the functions that operate on
 * the dynamic array
 */
typedef struct darray {
  int size;              ///< no of elements in array
  size_t capacity;       ///< no of elements the array can hold
  element_t **arr;       ///< array that holds pointers to element_t


  /**
   * @brief Get the element present at the index position
   * @param self - reference to darray_t struct
   * @param index - element to get from
   * @return reference to element_t struct, NULL if invalid index or not present
   */
  element_t* (* get)(struct darray *self, int index);

  /**
   * @brief Append an element at the end of the arrayfv
   * @param self - reference to darray_t struct
   * @param etype - enum type to identify the type of value
   * @param *value - void pointer to the value to be appended
   * @return index of insert position, -1 if append failure
   *
   * automatically resizes the array if the size is equal to capacity
   */
  int (* append)(struct darray *self, etype_t etype, void *value);

  /**
   * @brief Insert an element at the given position
   * @param self - reference to darray_t struct
   * @param index - insert position
   * @param etype - enum type to identify the type of value
   * @param *value - void pointer to the value to be appended
   * @return 1 on success 0 on failure
   *
   * automatic resize happens if the size if equals capacity
   * move all the element from insert position to right
   */
  bool (* insert)(struct darray *self, int index, etype_t etype, void *value);

  /**
   * @brief Pops the last element from the array
   * @param self - reference to darray_t struct
   * @return reference to element_t struct, NULL if no element
   */
  element_t* (* pop)(struct darray *self);

  /**
   * @brief Remove the first occurrence of the element in the array
   * @param self - reference to darray_t struct
   * @param etype - enum type to identify the type of value
   * @param *value - void pointer to the value to be appended
   * @return 1 on success 0 on failure
   */
  bool (* remove)(struct darray *self, etype_t etype, void *value);

  /**
   * @brief Index of first occurrence of the value
   * @param self - reference to darray_t struct
   * @param etype - enum type to identify the type of value
   * @param *value - void pointer to the value to be appended
   * @return index of first occurrence
   */
  int (* index)(struct darray *self, etype_t etype, void *value);

  /**
   * @brief Reverse the array in-place
   * @param self - reference to darray_t struct
   */
  void (* reverse)(struct darray *self);
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

void darray_free(darray_t **da);
