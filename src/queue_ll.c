#include "../include/vpl/queue_ll.h"

// ============================================
// Forward declarations (what this module provides)
// ============================================
static bool enqueue(queuell_t *self, etype_t etype, void *value);

static element_t dequeue(queuell_t *self);

static element_t peek(const queuell_t *self);

static bool is_empty(const queuell_t *self);

// UTILITY FUNCTION
static node_t* new_node(etype_t etype, void *val);


// ============================================
// Public API
// ============================================
queuell_t* queuell_init() {
  // allocate memory for queuell_t struct
  queuell_t *qll = calloc(1, sizeof(queuell_t));
  if (!qll) return NULL;

  // initialize with values
  qll->head = NULL;
  qll->tail = NULL;
  qll->size = 0;

  qll->enqueue = enqueue;
  qll->dequeue = dequeue;
  qll->peek = peek;
  qll->is_empty = is_empty;

  return qll;
}


void queuell_free(queuell_t **queue) {
  if (!queue || !(*queue)) return;

  node_t *curr = (*queue)->head;
  node_t *nxt = NULL;

  while (curr) {
    nxt = curr->next;

    // if it contains str data, then free the memory
    if (curr->data.etype == STR && curr->data.value.sval != NULL)
      free(curr->data.value.sval);

    free(curr);
    curr = nxt;
  }

  free((*queue));
  (*queue) = NULL;
}



// ============================================
// Core Function Implementation
// ============================================
static bool enqueue(queuell_t *self, etype_t etype, void *value) {
  if (!self || !value) return false;

  // create a new node and update it with value
  node_t *n = new_node(etype, value);
  if (!n) return false;

  // if queue is empty, then update the node in both head and tail
  if (is_empty(self)) {
    self->head = n;
    self->tail = n;
  }
  else {
    self->tail->next = n;
    self->tail = n;
  }

  self->size++;
  return true;
}


static element_t dequeue(queuell_t *self) {
  if (!self || is_empty(self)) return (element_t){0};

  // isolate head node and get it's value
  node_t *pop = self->head;
  element_t ele = pop->data;

  // update the head node to next node
  self->head = pop->next;

  // check if the queue is empty, if so then mark tail to null
  if(self->head == NULL) self->tail = NULL;

  free(pop);
  self->size--;
  return ele;     // caller is expected to free data, if element contains string
}


static element_t peek(const queuell_t *self) {
  if (!self || is_empty(self)) return (element_t){0};

  return self->head->data;
}


static bool is_empty(const queuell_t *self) {
  return !self || (self->head == NULL);
}


// ============================================
// Util Function Implementation
// ============================================
static node_t* new_node(etype_t etype, void *val) {
  if (!val) return NULL;

  node_t *n = malloc(sizeof(node_t));
  if (!n) return NULL;

  n->next = NULL;
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
