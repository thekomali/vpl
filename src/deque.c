#include "../include/vpl/deque.h"

// ============================================
// Forward declarations (what this module provides)
// ============================================
static bool enqueue_front(deque_t *self, etype_t etype, void *value);

static bool enqueue_rear(deque_t *self, etype_t etype, void *value);

static element_t dequeue_front(deque_t *self);

static element_t dequeue_rear(deque_t *self);

static const element_t* peek_front(deque_t *self);

static const element_t* peek_rear(deque_t *self);

static bool is_empty(deque_t *self);

static bool is_full(deque_t *self);


// ============================================
// Public API
// ============================================
deque_t* deque_init(int capacity) {
  if (capacity <= 0) return NULL;

  // allocate memory for the deque struct
  deque_t *dq = calloc(1, sizeof(deque_t) + ((size_t)capacity * sizeof(element_t)));
  if (!dq) return NULL;

  // assigning values
  dq->front = -1;
  dq->rear = -1;
  dq->size = 0;
  dq->capacity = capacity;

  // assigning function pointers
  dq->enqueue_front = enqueue_front;
  dq->enqueue_rear = enqueue_rear;
  dq->dequeue_front = dequeue_front;
  dq->dequeue_rear = dequeue_rear;
  dq->peek_front = peek_front;
  dq->peek_rear = peek_rear;
  dq->is_empty = is_empty;
  dq->is_full = is_full;

  return dq;
}


void deque_free(deque_t **self) {
  if (!self || !(*self)) return;

  deque_t *dq = (* self);

  // identiy if any element has string, if so clear that first
  // else it will cause memory leaks
  for (int i = 0; i < dq->capacity; i++) {
    if (dq->data[i].etype == STR && dq->data[i].value.sval != NULL)
      free(dq->data[i].value.sval);
  }

  // finally free the queue
  free(dq);
  (* self) = NULL;
}

// ============================================
// Core Function Implementation
// ============================================
static bool enqueue_front(deque_t *self, etype_t etype, void *value) {
  if (!self || !value || is_full(self)) return false;

  int insert_at;
  bool is_first = self->is_empty(self);

  // calculate the insert position (handle first insertion vs subsequent insertion)
  if (is_first) insert_at = 0;
  else          insert_at = (self->front - 1 + self->capacity) % self->capacity;

  // update the element_t with value
  switch (etype) {
    case INT: self->data[insert_at].value.ival = *(int *)value; break;
    case DOU: self->data[insert_at].value.dval = *(double *)value; break;
    case STR:
      self->data[insert_at].value.sval = strdup((char *)value);
      if (!self->data[insert_at].value.sval) return false;
      break;
    default:
      return false;
  }

  // update the element type and front position
  self->data[insert_at].etype = etype;
  self->front = insert_at;

  // only update rear, if it's first insertion
  if (is_first) self->rear = 0;

  self->size++;    // finally increament the elements count
  return true;
}


static bool enqueue_rear(deque_t *self, etype_t etype, void *value) {
  if (!self || !value || is_full(self)) return false;

  int insert_at;
  bool is_first = self->is_empty(self);

  if (is_first) insert_at = 0;
  else          insert_at = (self->rear + 1) % self->capacity;

  // update the element_t with value
  switch (etype) {
    case INT: self->data[insert_at].value.ival = *(int *)value; break;
    case DOU: self->data[insert_at].value.dval = *(double *)value; break;
    case STR:
      self->data[insert_at].value.sval = strdup((char *)value);
      if (!self->data[insert_at].value.sval) return false;
      break;
    default:
      return false;
  }

  // update the element type and rear position
  self->data[insert_at].etype = etype;
  self->rear = insert_at;

  // only update front, if it's first insertion
  if (is_first) self->front = 0;

  self->size++;    // finally increament the elements count
  return true;
}


static element_t dequeue_front(deque_t *self) {
  if (!self || is_empty(self)) return (element_t){0};

  element_t to_pop = self->data[self->front];

  // clean the array slot
  self->data[self->front] = (element_t){0};

  // check if only one element is present, then reset positions
  if (self->front == self->rear) {
    self->front = -1;              // last element removed, reset to empty state
    self->rear = -1;
  } else {
    self->front = (self->front + 1) % self->capacity;
  }

  self->size--;      // update the elements count
  return to_pop;
}


static element_t dequeue_rear(deque_t *self) {
  if (!self || is_empty(self)) return (element_t){0};

  element_t to_pop = self->data[self->rear];

  // clean the array slot
  self->data[self->rear] = (element_t){0};

  // check if only one element is present, then reset positions
  if (self->front == self->rear) {
    self->front = -1;              // last element removed, reset to empty state
    self->rear = -1;
  } else {
    self->rear = (self->rear - 1 + self->capacity) % self->capacity;
  }

  self->size--;      // update the elements count
  return to_pop;
}


static const element_t* peek_front(deque_t *self) {
  if (!self || is_empty(self)) return NULL;
  return &self->data[self->front];
}


static const element_t* peek_rear(deque_t *self) {
  if (!self || is_empty(self)) return NULL;
  return &self->data[self->rear];
}


static bool is_empty(deque_t *self) {
  return (self->front == -1 && self->rear ==  -1);
}


static bool is_full(deque_t *self) {
  return (self->rear + 1 % self->capacity == self->front);
}
