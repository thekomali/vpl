#include "../include/vpl/binary_tree.h"

// ============================================
// Forward declarations (what this module provides)
// ============================================
static void preorder_recv(TNode *root);

static void inorder_recv(TNode *root);

static void postorder_recv(TNode *root);

static void preorder(BinaryTree *bt);

static void inorder(BinaryTree *bt);

static void postorder(BinaryTree *bt);

static void levelorder(BinaryTree *bt);

// UTILITY FUNCTION
static void print_element(element_t ele);


// ============================================
// Public API
// ============================================
BinaryTree* binarytree_init() {
  BinaryTree *bt = calloc(1, sizeof(BinaryTree));
  if (!bt) return NULL;

  // initialize the values
  bt->root = NULL;
  bt->size = 0;

  // assign function pointers
  bt->preorder_recv = preorder_recv;
  bt->inorder_recv = inorder_recv;
  bt->postorder_recv = postorder_recv;
  bt->preorder = preorder;
  bt->inorder = inorder;
  bt->postorder = postorder;
  bt->levelorder = levelorder;

  return bt;
}


// ============================================
// Core Function Implementation
// ============================================
static void preorder_recv(TNode *root) {
  // exit condition of recursive loop
  if (!root) return;

  print_element(root->data);     // node is read first
  preorder_recv(root->left);     // traverse the left subtree
  preorder_recv(root->right);    // traverse the right subtree
}


static void inorder_recv(TNode *root) {
  // exit condition of recursive loop
  if (!root) return;

  inorder_recv(root->left);
  print_element(root->data);    // node is read after traversing left subtree
  inorder_recv(root->right);
}


static void postorder_recv(TNode *root) {
  // exit condition of recursive loop
  if (!root) return;

  postorder_recv(root->left);
  postorder_recv(root->right);
  print_element(root->data);    // node is read after traversing left & right subtree
}


static void preorder(BinaryTree *bt) {
  if (!bt || !bt->root) return;

  // initialize the stack
  TNodeStack *st = tnodestack_init(bt->size);
  if (!st) return;

  // assign the root node and push it to the stack
  TNode *n = bt->root;
  st->push(st, n);

  while (!st->is_empty(st)) {
    // 1. pop the node
    n = st->pop(st);

    // 2. print the node's value
    print_element(n->data);

    // 3. push the right & left nodes to stack
    if (n->right) st->push(st, n->right);
    if (n->left) st->push(st, n->left);
  }
}


static void inorder(BinaryTree *bt) {
  if (!bt || !bt->root) return;

  // initialize the stack
  TNodeStack *st = tnodestack_init(bt->size);
  if (!st) return;

  TNode *curr = bt->root;

  // we have to visit the left subtree before printing the node
  while (true) {
    // step 1: traverse along the left link until the end is reached
    while (curr->left != NULL) {
      st->push(st, curr);
      curr = curr->left;
    }

    // step 2: check if last node has right link; if not
    while (curr->right == NULL) {
      // step 2(a): print the element
      print_element(curr->data);

      // step 2(b): check for exit condition
      if (st->is_empty(st)) return;

      // step 2(c): pop a node from stack and update curr
      curr = st->pop(st);
    }

    // step 3: check if last node has right link, if yes,
    // step 3(a): print the element
    print_element(curr->data);

    // step 3(b): update curr to right link
    curr = curr->right;
  }
}


static void postorder(BinaryTree *bt) {
  if (!bt || !bt->root) return;

  // initialize the stack
  TNodeStack *st = tnodestack_init(bt->size);
  if (!st) return;

  TNode *curr = bt->root;
  TNode *previous_visited = NULL;

  // we have to visit left and right subtree, before printing the node
  while (true) {
    // step 1: traverse along the left link until the end is reached
    while (curr->left != NULL) {
      st->push(st, curr);
      curr = curr->left;
    }

    // step 2: check if last node has right link or
    //         previous node is right link of curr node: if yes
    //         this check prevent entering the same right link again
    while (curr->right == NULL || curr->right == previous_visited) {
      // step 2(a): print the element
      print_element(curr->data);

      // step 2(b): update previous visited node with curr node
      previous_visited = curr;

      // step 2(c): check for exit condition
      if (st->is_empty(st)) return;

      // step 2(d): pop a node from stack and update curr
      curr = st->pop(st);
    }

    // step 3: check if last node has right link: if yes
    // step 3(a): push the element to stack
    st->push(st, curr);

    // step 3(b): update the curr with right link
    curr = curr->right;
  }
}


static void levelorder(BinaryTree *bt) {
  if (!bt || !bt->root) return;

  // initialize queue
  TNodeQueue *q = tnodequeue_init(bt->size);
  if (!q) return;

  TNode *curr = NULL;

  // step 1: add the root node to queue
  q->enqueue(q, bt->root);

  while (!q->is_empty(q)) {
    // step 2: dequeue the element from queue
    curr = q->dequeue(q);

    // step 3: print the element
    print_element(curr->data);

    // step 4: enqueue left and right link if not null
    if (curr->left)  q->enqueue(q, curr->left);
    if (curr->right) q->enqueue(q, curr->right);
  }
}


// ============================================
// Util Function Implementation
// ============================================
static void print_element(element_t ele) {
  switch (ele.etype) {
    case INT: printf("%d ", ele.value.ival);  break;
    case DOU: printf("%lf ", ele.value.dval); break;
    case STR: printf("%s ", ele.value.sval);  break;
    default: fprintf(stderr, "ERROR: INVALID ELEMENT TYPE");
  }
}


TNode* new_node(etype_t etype, void *val) {
  if (!val) return NULL;

  TNode *n = calloc(1, sizeof(TNode));
  if (!n) return NULL;

  n->right = NULL;
  n->left = NULL;
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



