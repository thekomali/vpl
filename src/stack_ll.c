#include "../include/vpl/stack_ll.h"

// ============================================
// Forward declarations (what this module provides)
// ============================================
static bool push(stackll_t *self, etype_t etype, void *value);

static element_t pop(stackll_t *self);

static element_t peek(stackll_t *self);

static bool is_empty(stackll_t *self);

// UTILITY FUNCTION
static node_t* new_node(etype_t etype, void *val);

// ============================================
// Public API
// ============================================
stackll_t* stackll_init() {
  // allocate memory for stackll_t struct
  stackll_t *sll = calloc(1, sizeof(stackll_t));
  if (!sll) return NULL;

  // initialize with values
  sll->head = NULL;
  sll->size = 0;

  sll->push = push;
  sll->pop = pop;
  sll->peek = peek;
  sll->is_empty = is_empty;

  return sll;
}


void stackll_free(stackll_t **stack) {
  if (!stack || !(*stack)) return;

  node_t *curr = (*stack)->head;
  node_t *nxt = NULL;

  while (curr) {
    nxt = curr->next;

    // if it contains str data, then free the memory
    if (curr->data.etype == STR && curr->data.value.sval != NULL)
      free(curr->data.value.sval);

    free(curr);
    curr = nxt;
  }

  free((*stack));
  (*stack) = NULL;
}


// ============================================
// Core Function Implementation
// ============================================
static bool push(stackll_t *self, etype_t etype, void *value) {
  if (!self || !value) return false;

  // create a new node
  node_t *n = new_node(etype, value);
  if (!n) return false;

  n->next = self->head;
  self->head = n;

  self->size++;
  return true;
}


static element_t pop(stackll_t *self) {
  if (!self || is_empty(self)) return (element_t){0};

  // update references and isolate the head node
  node_t *pop = self->head;
  self->head = pop->next;

  // extract the value alone and free the node
  element_t ele = pop->data;
  free(pop);

  self->size--;
  return ele;     // caller is expected to free data, if element contains string
}


static element_t peek(stackll_t *self) {
  if (!self || is_empty(self)) return (element_t){0};

  return self->head->data;
}


static bool is_empty(stackll_t *self) {
  return (self->head == NULL);
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


