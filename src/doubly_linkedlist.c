#include "../include/vpl/doubly_linkedlist.h"


// ============================================
// Forward declarations (what this module provides)
// ============================================
static dnode_t* get(dlinkedlist_t *self, int index);

static int append(dlinkedlist_t *self, etype_t etype, void *val);

static bool insert(dlinkedlist_t *self, int index, etype_t etype, void *val);

static bool remove_imp(dlinkedlist_t *self, etype_t etype, void *val);

static dnode_t* pop(dlinkedlist_t *self);

static int index_imp(dlinkedlist_t *self, etype_t etype, void *val);

static void reverse(dlinkedlist_t *self);

// UTILITY FUNCTION
static void free_node(dnode_t *n);

static dnode_t* new_node(etype_t etype, void *val);

static bool elements_equal(element_t *ele, etype_t etype, void *val);


// ============================================
// Public API
// ============================================
dlinkedlist_t* dlinkedlist_init() {
  dlinkedlist_t *dll = calloc(1, sizeof(dlinkedlist_t));
  if (!dll) return NULL;

  // initialize members
  dll->size = 0;
  dll->head = NULL;
  dll->tail = NULL;

  // initialize function pointers
  dll->get = get;
  dll->append = append;
  dll->insert = insert;
  dll->pop = pop;
  dll->remove = remove_imp;
  dll->index = index_imp;
  dll->reverse = reverse;

  return dll;
}


void dlinkedlist_free(dlinkedlist_t **dll) {
  if (!dll || !(*dll)) return;

  dnode_t *to_remove = NULL;
  dnode_t *head = (*dll)->head;

  while (head) {
    to_remove = head;
    head = head->next;

    // free the memory
    free_node(to_remove);
  }

  free(*dll);     // free the linkedlist_t struct
  *dll = NULL;    // set value to NULL
}


// ============================================
// Core Function Implementation
// ============================================
static dnode_t* get(dlinkedlist_t *self, int index) {
  if (!self || !self->head || index >= self->size) return NULL;

  dnode_t *curr;

  // as in doubly linked list, we can traverse from head or tail
  // we use this to optimize the traversal.
  // index b/w first half (head -> size/2)  -> start from head
  // index b/w second half (size/2 -> tail) -> start from tail

  if (index < self->size/2) {
    // traverse in first half
    curr = self->head;
    for (int i = 0; i < index; i++) curr = curr->next;
  }
  else {
    // tranverse in second half
    curr = self->tail;
    for (int i = self->size - 1; i > index; i--) curr = curr->prev;
  }

  return curr;
}


static int append(dlinkedlist_t *self, etype_t etype, void *val) {
  if (!self || !val) return -1;

  // create a node
  dnode_t *n = new_node(etype, val);
  if (!n) return -1;

  // linked list has no nodes
  if (!self->head) {
    self->head = n;
    self->tail = n;
  }
  else {
    self->tail->next = n;
    n->prev = self->tail;
    self->tail = n;
  }

  return self->size++; // return index where node is added and increment the size
}


static bool insert(dlinkedlist_t *self, int index, etype_t etype, void *val) {
  if (!self || !val || index > self->size || index < 0) return false;

  // if index equals size, then use append function
  if (index == self->size)
    return append(self, etype, val) != -1;

  // create a node and update it with value
  dnode_t *n = new_node(etype, val);
  if (!n) return false;

  // insert positon is 0, then update the head node too
  if (index == 0) {
    n->next = self->head;
    self->head->prev = n;
    self->head = n;
  }
  else {
    // get the node one before the index positon
    dnode_t* prev = get(self, index-1);
    if (!prev) {
      free_node(n);
      return false;
    }
    n->next = prev->next;    // new node - next ref update
    n->prev = prev;          // new node - prev ref update

    prev->next = n;          // prev node - next ref update with new node
    n->next->prev = n;       // next node's - prev ref update with new node
  }

  self->size++;
  return true;
}


static bool remove_imp(dlinkedlist_t *self, etype_t etype, void *val) {
  if (!self || !self->head || !val) return false;

  dnode_t *curr = self->head;

  // search for the node
  while (curr) {
    if (elements_equal(&curr->data, etype, val)) break;
    curr = curr->next;
  }

  // value not found
  if (!curr) return false;

  // handle the previous link
  if (curr->prev == NULL)
    self->head = curr->next;     // head node removel, so update head link
  else
    curr->prev->next = curr->next;

  // handle next link
  if (curr->next == NULL)
    self->tail = curr->prev;     // tail node removel, so update tail link
  else
    curr->next->prev = curr->prev;

  free_node(curr);
  self->size--;
  return true;
}


static dnode_t* pop(dlinkedlist_t *self) {
  if (!self || self->size == 0) return NULL;

  dnode_t *to_pop = self->tail;

  if (self->head == self->tail) {
    // only one node is present
    self->head = NULL;
    self->tail = NULL;
  }
  else {
    // more nodes were present
    self->tail = to_pop->prev;
    self->tail->next = NULL;
  }

  // remove the pointer befor popping & update the size
  to_pop->next = NULL;
  to_pop->prev = NULL;

  self->size--;
  return to_pop;    // caller is expected to free memory of the node
}


static int index_imp(dlinkedlist_t *self, etype_t etype, void *val) {
  if (!self || !self->head || !val) return -1;

  dnode_t *curr = self->head;
  for (int i = 0; curr != NULL; curr=curr->next, i++) {
    if (elements_equal(&curr->data, etype, val))
      return i;
  }

  // if we reached here, then no match found
  return -1;
}


static void reverse(dlinkedlist_t *self) {
  if (!self || self->size == 0) return;

  dnode_t* curr = self->head;
  dnode_t* next = NULL;

  while (curr) {
    next = curr->next;
    SWAP(curr->prev, curr->next);

    curr = next;
  }

  // finally swap the pointer which denote head and tail pointers
  SWAP(self->head, self->tail);
  return;
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


static void free_node(dnode_t *n) {
  if (!n) return;

  // if the element_t contains string data, then free it first
  if (n->data.etype == STR) free(n->data.value.sval);

  free(n);
}


static bool elements_equal(element_t *el, etype_t type, void *val) {
  if (el->etype != type) return false;
  if (type == INT) return el->value.ival == *(int*)val;
  if (type == DOU) return el->value.dval == *(double*)val;
  if (type == STR) return strcmp(el->value.sval, (char*)val) == 0;
  return false;
}








