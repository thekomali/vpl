#include "../include/vpl/linked_list.h"

// ============================================
// Forward declarations (what this module provides)
// ============================================
static node_t* get(linkedlist_t *self, int index);

static int append(linkedlist_t *self, etype_t etype, void *val);

static bool insert(linkedlist_t *self, int index, etype_t etype, void *val);

static bool remove_imp(linkedlist_t *self, etype_t etype, void *val);

static node_t* pop(linkedlist_t *self);

static int index_imp(linkedlist_t *self, etype_t etype, void *val);

static void reverse(linkedlist_t *self);

// UTILITY FUNCTION
static void free_node(node_t *n);

static node_t* new_node(etype_t etype, void *val);

static bool elements_equal(element_t *ele, etype_t etype, void *val);


// ============================================
// Public API
// ============================================
linkedlist_t* linkedlist_init() {
  linkedlist_t *ll = calloc(1, sizeof(linkedlist_t));
  if (!ll) return NULL;

  // initialize members
  ll->size = 0;
  ll->head = NULL;
  ll->tail = NULL;

  // initialize function pointers
  ll->get = get;
  ll->append = append;
  ll->insert = insert;
  ll->pop = pop;
  ll->remove = remove_imp;
  ll->index = index_imp;
  ll->reverse = reverse;

  return ll;
}


void linkedlist_free(linkedlist_t **ll) {
  if (!ll || !(*ll)) return;

  node_t *to_remove = NULL;
  node_t *head = (*ll)->head;

  while (head) {
    to_remove = head;
    head = head->next;

    // free the memory
    free_node(to_remove);
  }

  free(*ll);     // free the linkedlist_t struct
  *ll = NULL;    // set value to NULL
}


// ============================================
// Core Function Implementation
// ============================================
static node_t* get(linkedlist_t *self, int index) {
  if (!self || !self->head || index >= self->size) return NULL;

  node_t *curr = self->head;

  while (index--)
    curr = curr->next;

  return curr;
}


static int append(linkedlist_t *self, etype_t etype, void *val) {
  if (!self || !val) return -1;

  // create a node
  node_t *n = new_node(etype, val);
  if (!n) return -1;

  // linked list has no nodes
  if (!self->head) {
    self->head = n;
    self->tail = n;
  }
  else {
    self->tail->next = n;
    self->tail = n;
  }

  return self->size++; // return index where node is added and increment the size
}


static bool insert(linkedlist_t *self, int index, etype_t etype, void *val) {
  if (!self || !val || index > self->size || index < 0) return false;

  // if index equals size, then use append function
  if (index == self->size)
    return append(self, etype, val) != -1;

  // create a node and update it with value
  node_t *n = new_node(etype, val);
  if (!n) return false;

  // insert positon is 0, then update the head node too
  if (index == 0) {
    n->next = self->head;
    self->head = n;
  }
  else {
    // get the node one before the index positon
    node_t* prev = get(self, index-1);
    if (!prev) {
      free_node(n);
      return false;
    }
    n->next = prev->next;
    prev->next = n;
  }

  self->size++;
  return true;
}


static bool remove_imp(linkedlist_t *self, etype_t etype, void *val) {
  if (!self || !self->head || !val) return false;

  node_t *curr = self->head;
  node_t *prev = NULL;

  // search for the node and its note it's previous node
  while (curr) {
    // Assuming you have a way to compare the element_t values
    if (elements_equal(&curr->data, etype, val)) {
      break;
    }
    prev = curr;
    curr = curr->next;
  }

  // value not found
  if (!curr) return false;

  // if the element matches the node at head
  if (prev == NULL) {
    self->head = curr->next;

    // check if the linked list is empty after removel
    // if so update the tail node too
    if (self->head == NULL) {
      self->tail = NULL;
    }
  }
  else {
    // removel of element in b/w or tail
    prev->next = curr->next;

    // update tail if the last node was removed
    if (curr == self->tail) {
      self->tail = prev;
    }
  }

  free_node(curr);
  self->size--;
  return true;
}


static node_t* pop(linkedlist_t *self) {
  if (!self || !self->head) return NULL;

  node_t *to_pop = NULL;

  // case 1: only one element is present
  if (self->head == self->tail) {
    to_pop = self->head;

    self->head = NULL;
    self->tail = NULL;
  }
  else {
    // case 2: multiple node is present
    // go to previous node to tail node
    // tail node is at size-1, prev  to tail node is at size-2
    node_t *prev = get(self, self->size - 2);

    to_pop = self->tail;
    prev->next = NULL;
    self->tail = prev;
  }

  self->size--;
  return to_pop;   // caller is expected to free the memory of the node
}


static int index_imp(linkedlist_t *self, etype_t etype, void *val) {
  if (!self || !self->head || !val) return -1;

  node_t *curr = self->head;
  for (int i = 0; curr != NULL; curr=curr->next, i++) {
    if (elements_equal(&curr->data, etype, val))
      return i;
  }

  // if we reached here, then no match found
  return -1;
}


static void reverse(linkedlist_t *self) {
  if (!self || !self->head) return;

  node_t* old_head = self->head;    // old head will become the tail node

  node_t *curr = self->head;
  node_t *prev = NULL;
  node_t *next = NULL;

  while (curr) {
    // store the next reference
    next = curr->next;

    curr->next = prev;    // update the curr node's next ref to previous
    prev = curr;          // mark the curr node as prev node

    curr = next;          // update curr to run the loop
  }

  // prev will point to the head node, update that in linkedlist_t struct
  self->head = prev;
  self->tail = old_head;
}


// ============================================
// Util Function Implementation
// ============================================
static void free_node(node_t *n) {
  if (!n) return;

  // if the element_t contains string data, then free it first
  if (n->data.etype == STR) free(n->data.value.sval);

  free(n);
}


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


static bool elements_equal(element_t *el, etype_t type, void *val) {
  if (el->etype != type) return false;
  if (type == INT) return el->value.ival == *(int*)val;
  if (type == DOU) return el->value.dval == *(double*)val;
  if (type == STR) return strcmp(el->value.sval, (char*)val) == 0;
  return false;
}
