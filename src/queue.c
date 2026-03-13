#include "../include/vpl/queue.h"

// ============================================
// Forward declarations (what this module provides)
// ============================================
static bool enqueue(queue_t *self, etype_t etype, void * value);

static element_t dequeue(queue_t *self);

static const element_t* peek(queue_t *self);

static bool is_empty(queue_t *self);

static bool is_full(queue_t *self);


// ============================================
// Public API
// ============================================
queue_t* queue_init(int capacity) {
  if (capacity <= 0) return NULL;

  // allocate the memory for the queue struct & the array of element_t's
  queue_t *q = calloc(1, sizeof(queue_t) + ((size_t) capacity * sizeof(element_t)));
  if (!q) return NULL;

  // initialize with values
  q->front = -1;
  q->rear = -1;
  q->capacity = capacity;
  q->size = 0;

  // assign the function pointers
  q->enqueue = enqueue;
  q->dequeue = dequeue;
  q->peek = peek;
  q->is_empty = is_empty;
  q->is_full = is_full;

  return q;
}


void queue_free(queue_t **self) {
  if (!self || !(*self)) return;

  queue_t *queue = (* self);

  // identiy if any element has string, if so clear that first
  // else it will cause memory leaks
  for (int i = 0; i < queue->capacity; i++) {
    if (queue->data[i].etype == STR && queue->data[i].value.sval != NULL)
      free(queue->data[i].value.sval);
  }

  // finally free the queue
  free(queue);
  (* self) = NULL;
}


// ============================================
// Core Function Implementation
// ============================================
static bool enqueue(queue_t *self, etype_t etype, void *value) {
  if (!self || !value || is_full(self)) return false;

  // calculate the insert position
  int insert_at = (self->rear + 1) % self->capacity;

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

  // update the element type & the rear position
  self->data[insert_at].etype = etype;
  self->rear = insert_at;

  // handle the front position for first enqueue operation
  if (self->front == -1) self->front = 0;

  self->size++;    // finally increament the elements count
  return true;
}


static element_t dequeue(queue_t *self) {
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


static const element_t* peek(queue_t *self) {
  if (!self || is_empty(self)) return NULL;
  return &self->data[self->front];
}


static bool is_empty(queue_t *self) {
  return (self->front == -1 && self->rear == -1);
}


static bool is_full(queue_t *self) {
  return (self->rear + 1) % self->capacity == self->front;
}
