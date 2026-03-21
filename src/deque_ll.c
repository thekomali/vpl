#include "../include/vpl/deque_ll.h"
#include <string.h>

// ============================================
// Forward declarations (what this module provides)
// ============================================
static bool enqueue_front(DequeLL *self, etype_t etype, void *value);

static bool enqueue_rear(DequeLL *self, etype_t etype, void *value);

static element_t dequeue_front(DequeLL *self);

static element_t dequeue_rear(DequeLL *self);

static const element_t* peek_front(DequeLL *self);

static const element_t* peek_rear(DequeLL *self);

static bool is_empty(DequeLL *self);

// UTILITY FUNCTION
static dnode_t* new_node(etype_t etype, void *val);


// ============================================
// Public API
// ============================================
DequeLL* dequell_init() {
  DequeLL *dqll = calloc(1, sizeof(DequeLL));
  if (!dqll) return NULL;

  // assign values
  dqll->head = NULL;
  dqll->tail = NULL;
  dqll->size = 0;

  // assigning function pointers
  dqll->enqueue_front = enqueue_front;
  dqll->enqueue_rear = enqueue_rear;
  dqll->dequeue_front = dequeue_front;
  dqll->dequeue_rear = dequeue_rear;
  dqll->peek_front = peek_front;
  dqll->peek_rear = peek_rear;
  dqll->is_empty = is_empty;

  return dqll;
}


void dequell_free(DequeLL **dqll) {
  if (!dqll || !(*dqll)) return;

  dnode_t* curr = (* dqll)->head;
  dnode_t* to_del = NULL;

  while (curr) {
    // free the string, if the element type is string
    if (curr->data.etype == STR) free(curr->data.value.sval);

    to_del = curr;
    curr = curr->next;

    free(to_del);   // free the node
  }

  // finally free DequeLL struct
  free(* dqll);
  *dqll = NULL;
}


// ============================================
// Core Function Implementation
// ============================================
static bool enqueue_front(DequeLL *self, etype_t etype, void *value) {
  if (!self || !value) return false;

  // create a new node and assign value
  dnode_t* dnode = new_node(etype, value);
  if (!dnode) return false;

  // in case the deque is empty (then update the head upon adding new node)
  if (is_empty(self)) {
    self->head = dnode;
    self->tail = dnode;
  }
  else {
    dnode->next = self->head;
    self->head->prev = dnode;
    self->head = dnode;
  }

  self->size++;
  return true;
}


static bool enqueue_rear(DequeLL *self, etype_t etype, void *value) {
  if (!self || !value) return false;

  // create a new node and assign value
  dnode_t* dnode = new_node(etype, value);
  if (!dnode) return false;

  // in case the deque is empty (then update the tail upon adding new node)
  if (is_empty(self)) {
    self->head = dnode;
    self->tail = dnode;
  }
  else {
    dnode->prev = self->tail;
    self->tail->next = dnode;
    self->tail = dnode;
  }

  self->size++;
  return true;
}


static element_t dequeue_front(DequeLL *self) {
  if (!self || is_empty(self)) return (element_t){0};

  dnode_t *pop = self->head;
  element_t ele = pop->data;

  // update head to next node
  self->head = self->head->next;

  // check if the deque is empty, then update tail to null
  if (self->head == NULL) self->tail = NULL;   // update tail to null, in case empty
  else self->head->prev = NULL;                // detach new head from pop node

  free(pop);
  self->size--;

  return ele;      // caller is expected to free data, if element contains string
}


static element_t dequeue_rear(DequeLL *self) {
  if (!self || is_empty(self)) return (element_t){0};

  dnode_t *pop = self->tail;
  element_t ele = pop->data;

  // update tail to previous node
  self->tail = self->tail->prev;

  // check if the deque is empty
  if (self->tail == NULL) self->head = NULL;   // update head to null, in case empty
  else self->tail->next = NULL;                // detach last node from tail

  free(pop);
  self->size--;

  return ele;      // caller is expected to free data, if element contains string
}


static const element_t* peek_front(DequeLL *self) {
  if (!self || is_empty(self)) return NULL;

  return &self->head->data;
}


static const element_t* peek_rear(DequeLL *self) {
  if (!self || is_empty(self)) return NULL;

  return &self->tail->data;
}


static bool is_empty(DequeLL *self) {
  return !self || self->size == 0;
}



// ============================================
// Util Function Implementation
// ============================================
static dnode_t* new_node(etype_t etype, void *val) {
  if (!val) return NULL;

  dnode_t *n = malloc(sizeof(dnode_t));
  if (!n) return NULL;

  n->next = NULL;
  n->prev = NULL;
  n->data.etype = etype;

  // update the value
  switch (etype) {
    case INT:
      n->data.value.ival = *(int *)val; break;
    case DOU:
      n->data.value.dval = *(double *)val; break;
    case STR:
      n->data.value.sval = strdup((char *)val);
      if (!n->data.value.sval) {
        free(n);
        return NULL;
      }
      break;
    default:
      // invalid element type
      free(n);
      return NULL;
  }

  return n;
}
