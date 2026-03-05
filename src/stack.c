#include "../include/vpl/stack.h"

// ============================================
// Forward declarations (what this module provides)
// ============================================
static bool push(stack_t *self, etype_t etype, void *value);

static element_t pop(stack_t *self);

static element_t peek(stack_t *self);

static bool is_empty(stack_t *self);

static bool is_full(stack_t *self);


// ============================================
// Public API
// ============================================
stack_t* stack_init(int capacity) {
  if (capacity <= 0) return NULL;

  // allocate memory for stack_t struct
  stack_t *s = calloc(1, sizeof(stack_t));
  if (!s) return NULL;

  // allocate memory for the element_t array
  s->data = calloc((size_t)capacity, sizeof(element_t));
  if (!s->data) {
    free(s);
    return NULL;
  }

  // assign values to function pointers and attributes
  s->top = -1;              // -1 denotes the stack is empty
  s->capacity = capacity;

  s->push = push;
  s->pop = pop;
  s->peek = peek;
  s->is_full = is_full;
  s->is_empty = is_empty;

  return s;
}


void stack_free(stack_t **stack) {
  if (!stack || !(*stack)) return;

  // if any element has string data, then it has to be explicity freed
  for (int i = 0; i <= (*stack)->top; i++) {
    if ((*stack)->data[i].etype == STR)
      free((*stack)->data[i].value.sval);
  }

  free((*stack)->data);    // free the array of element_t

  free((*stack));          // finally free the stack_t struct
  *stack = NULL;           // set the stack_t pointer to NULL
}


// ============================================
// Core Function Implementation
// ============================================
static bool push(stack_t *self, etype_t etype, void *value) {
  if (!self || !value || is_full(self)) return false;

  // increment top and then add the value
  int next_index = self->top + 1;
  element_t *next_element = &self->data[next_index];

  switch (etype) {
    case INT: next_element->value.ival = *(int *)value; break;
    case DOU: next_element->value.dval = *(double *)value; break;
    case STR:
      next_element->value.sval = strdup((char *)value);
      if (!next_element->value.sval) return false;
      break;
    default:
      return false;
  }

  // only update top if all operation is success
  self->top = next_index;
  next_element->etype = etype;
  return true;
}


static element_t pop(stack_t *self) {
  if (!self || is_empty(self)) return (element_t){0};

  // let's take the copy of the element, now this data is owned by user
  // stack operations should not affect the data (element_t)
  element_t ele = self->data[self->top];

  // if the element_t contains string, then let's crear it
  // if we don't do it, and if stack_free is called then it will clear all data
  // including the data user popped (user owned)
  if (ele.etype == STR)
    self->data[self->top].value.sval = NULL;

  self->top--;
  return ele;
}


static element_t peek(stack_t *self) {
  if (!self || is_empty(self)) return (element_t){0};

  return (self->data[self->top]);
}


static bool is_empty(stack_t *self) {
  return (self->top == -1);
}


static bool is_full(stack_t *self) {
  return (self->top == (self->capacity - 1));
}
