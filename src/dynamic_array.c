#include "../include/vpl/dynamic_array.h"

// ============================================
// Forward declarations (what this module provides)
// ============================================
static element_t* get(darray_t *self, int index);

static int append(darray_t *self, etype_t etype, void *value);

static bool insert(darray_t *self, int index, etype_t etype, void *value);

static element_t* pop(darray_t *self);

//decleration of 'remove' present in stdio.h, so renamed to remove_imp
static bool remove_imp(darray_t *self, etype_t etype, void *value);

// decleration of 'index' present in string.h, renamed to index_imp
static int index_imp(darray_t *self, etype_t etype, void *value);

static void reverse(darray_t *self);

// UTILITY FUNCTIONS
static bool resize(darray_t *da);

static bool is_full(darray_t *da);

static void free_element(element_t *ele);

static element_t* new_element(etype_t etype, void *value);


// ============================================
// Public API
// ============================================
darray_t* darray_init(size_t capacity) {
  // if capacity is not specified, don't initialize
  if (capacity <= 0) return NULL;

  darray_t *da = calloc(1, sizeof(darray_t));
  if (!da) return NULL;

  // initialize the arr member
  da->arr = calloc(capacity, sizeof(element_t *));
  if (!da->arr) {
    free(da);
    return NULL;
  }

  // initialize other members
  da->size = 0;
  da->capacity = capacity;

  // initialize the function pointers
  da->get = get;
  da->append = append;
  da->insert = insert;
  da->pop = pop;
  da->remove = remove_imp;
  da->index = index_imp;
  da->reverse = reverse;

  return da;
}


void darray_free(darray_t **da) {
  if (!da || !(*da)) return;

  for (int i = 0; i < (*da)->size; i++) {
    free_element( (*da)->arr[i] );
  }

  free((*da)->arr);   // free the array of pointers
  free(*da);          // free the darray_t struct

  *da = NULL;         // set the value to NULL
}


// ============================================
// Core Functions Implementation
// ============================================
static element_t* get(darray_t *self, int index) {
  if (!self || index >= self->size || index < 0) return NULL;

  return self->arr[index];
}


static int append(darray_t *self, etype_t etype, void *value) {
  if (!self || !value) return -1;

  // check if the array is full & do resize, if needed
  if (is_full(self) && !resize(self)) return -1;

  // create a new element
  element_t *ele = new_element(etype, value);
  if (!ele) return -1;

  // add an element and increment the size
  // size always points to empty space (as it starts from Zero)
  self->arr[self->size++] = ele;
  return self->size - 1;
}


static bool insert(darray_t *self, int index, etype_t etype, void *value) {
  if (!self || !value || index > self->size || index < 0) return false;

  // let's resize the array if it's full
  if (is_full(self) && !resize(self)) return false;

  // if index equals size, then perform append
  if (index == self->size) return append(self, etype, value);

  // create a new element
  element_t *ele = new_element(etype, value);
  if (!ele) return false;

  // move elements to the right to make space for the new element
  for (int i = self->size; i > index; i--)
    self->arr[i] = self->arr[i - 1];

  // insert the new element into the proper position
  self->arr[index] = ele;

  self->size++;
  return true;
}


static element_t* pop(darray_t *self) {
  if (!self || self->size == 0) return NULL;

  // decrement the size first and then reutn the last element
  // caller is expected to free the memory of elment_t struct
  return self->arr[--self->size];
}


static bool remove_imp(darray_t *self, etype_t etype, void *value) {
  if (!self || !value || self->size == 0) return false;

  // identify if the element is present in the array
  int index = index_imp(self, etype, value);
  if (index == -1) return false;

  // element to remove
  element_t *to_remove = self->arr[index];

  // in case the element is present b/w elements, then we need to move elements
  // one step towards left - using memmove(to, from, size)
  if (index < self->size - 1) {
    size_t elements_to_move = (size_t)self->size - (size_t)index - 1;

    // move the elements one step towards left
    memmove(&self->arr[index], &self->arr[index + 1], elements_to_move * sizeof(element_t *));
  }

  self->size--;
  self->arr[self->size] = NULL;   // clear the un-used slot

  // free the element
  free_element(to_remove);
  return true;
}


static int index_imp(darray_t *self, etype_t etype, void *value) {
  if (!self || !value || self->size == 0) return -1;

  // loop though the arry and identify if the element is present
  for (int i = 0; i < self->size; i++) {
    // ensure the element type matches
    if (self->arr[i]->etype != etype) continue;

    // value comparison happens, only if etype matches
    switch (etype) {
      case INT:
        if (self->arr[i]->value.ival == *(int *)value) return i;
        break;
      case DOU:
        if (fabs(self->arr[i]->value.dval - *(double *)value) < 1E-9) return i;
        break;
      case STR:
        if (strcmp(self->arr[i]->value.sval, (char *)value) == 0) return i;
        break;
      default:
        // unknown element type
        return -1;
    }
  }

  // if we reached here, then no matching found
  return -1;
}


static void reverse(darray_t *self) {
  if (!self || self->size <= 1) return;

  for (int l = 0, r = self->size-1; l < r; l++, r--)
    SWAP(self->arr[l], self->arr[r]);
}


// ============================================
// Util Functions Implementation
// ============================================
static bool resize(darray_t *da) {
  if (!da) return false;

  // verify if the array is full
  if (!is_full(da)) return false;

  // allocate double the size, and copy current ref to new array
  size_t new_capacity = da->capacity ? da->capacity * 2 : 1;
  element_t **new_arr = realloc(da->arr, new_capacity * sizeof(element_t *));
  if (!new_arr) return false;

  // update the darray_t struct
  da->capacity = new_capacity;
  da->arr = new_arr;
  return true;
}


static bool is_full(darray_t *da) {
  // size start with 0 (size points to empty positon)
  // to add an element, insert it and increment size
  return (size_t)da->size == da->capacity;
}


static element_t* new_element(etype_t etype, void *value) {
  if (!value) return NULL;

  element_t *ele = calloc(1, sizeof(element_t));
  if (!ele) return NULL;

  // based on the etype, dereference and assign the values
  switch (etype) {
    case INT: ele->value.ival = *(int *)value; break;
    case DOU: ele->value.dval = *(double *)value; break;
    case STR: {
      ele->value.sval = strdup((char *)value);
      if (!ele->value.sval) {
        free(ele);
        return NULL;
      }
      break;
    }
    default: return NULL;
  }

  ele->etype = etype;
  return ele;
}


static void free_element(element_t *ele) {
  if (!ele) return;

  // free the memory allocated for string
  if (ele->etype == STR) free(ele->value.sval);

  free(ele);
}
