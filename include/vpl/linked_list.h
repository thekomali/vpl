#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "util.h"


/**
 * @brief Struct to represent a linked list
 */
typedef struct linkedlist{
  int size;            ///< no of nodes in a linked list
  node_t *head;        ///< reference to head node
  node_t *tail;        ///< reference to tail node

  /**
   * @brief Get the node at the index positon
   * @param self - reference to linkedlist_t
   * @param index - position to get
   * @return reference to node_t else NULL
   */
  node_t* (* get)(struct linkedlist *self, int index);

  /**
   * @brief Insert value at the end of the linked list
   * @param self - reference to linkedlist_t
   * @param etype - type of the value the void pointer points
   * @param *val - value as void pointer
   *
   * Use of tail pointer, make the append with O(1) complexit
   */
  int (* append)(struct linkedlist *self, etype_t etype, void *val);

  /**
   * @brief Insert value at the index position
   * @param self - reference to linkedlist_t
   * @param index - insert positon
   * @param etype - type of the value the void pointer points
   * @param *val - value as void pointer
   */
  bool (* insert)(struct linkedlist *self, int index, etype_t etype, void *val);

  /**
   * @brief Pop the last node
   * @param self - reference to linkedlist_t
   * @return popped node (caller expected to free the memeory)
   */
  node_t* (* pop)(struct linkedlist *self);

  /**
   * @brief Remove node at any given positon
   * @param self - reference to linkedlist_t
   * @param etype - type of the value the void pointer points
   * @param *val - value as void pointer
   */
  bool (* remove)(struct linkedlist *self, etype_t etype, void *val);

  /**
   * @brief Get the index of the given value
   * @param self - reference to linkedlist_t
   * @param etype - type of the value the void pointer points
   * @param *val - value as void pointer
   * @return first occurrence of the value, if not return -1
   */
  int (* index)(struct linkedlist *self, etype_t etype, void *val);

  /**
   * @brief Rever the linked list in-place
   * @param self - reference to linkedlist_t
   */
  void (* reverse)(struct linkedlist *self);
}linkedlist_t;


// ============================================
// Function Declarations
// ============================================

linkedlist_t* linkedlist_init();

void linkedlist_free(linkedlist_t **ll);
