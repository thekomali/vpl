#include "../include/vpl/binary_tree.h"\

// ============================================
// Forward declarations (what this module provides)
// ============================================
static void preorder_recv(TNode *root);

static void inorder_recv(TNode *root);

static void postorder_recv(TNode *root);

static void preorder(BinaryTree *bt);

static void inorder(BinaryTree *bt);

static void postorder(BinaryTree *bt);

// UTILITY FUNCTION
static void print_element(element_t ele);

// TNodeStack ASSOCIATED FUNCTIONS
static TNodeStack* tnodestack_init(int capacity);

static void push(TNodeStack *st, TNode *node);

static TNode* pop(TNodeStack *st);

static TNode* peek(TNodeStack *st);

static bool is_empty(TNodeStack *st);

static bool is_full(TNodeStack *st);


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

  while (true) {
    // step 1: push the curr node into the stack, until curr->left becomes NULL
    if (curr != NULL) {
      st->push(st, curr);
      curr = curr->left;
    }
    else {
      // case 2: if stack is empty return
      if (st->is_empty(st)) break;

      // step 3: pop the element from the stack
      curr = st->pop(st);

      // step 4: print the element
      print_element(curr->data);

      // step 5 : update the curr to right link
      curr = curr->right;
    }
  }
}


static void postorder(BinaryTree *bt) {
  if (!bt || !bt->root) return;

  // initialize the stack
  TNodeStack *st = tnodestack_init(bt->size);
  if (!st) return;

  TNode *curr = bt->root;
  TNode *tmp = NULL;

  while (curr != NULL || !st->is_empty(st)) {
    // step 1: traverse along left link & push it to stack until NULL
    if (curr != NULL) {
      st->push(st, curr);
      curr = curr->left;
    }
    else {
      // step 2: check if right link is present
      tmp = st->peek(st)->right;

      if (tmp == NULL) {
        // step 4: pop a node from stack if right link is not present
        tmp = st->pop(st);

        // step 5: print the element
        print_element(tmp->data);

        // step 6: if tmp is the right link of the node in stack
        while (!st->is_empty(st) && tmp == st->peek(st)->right) {
          // step 7: then pop the node from stack & print it
          tmp = st->pop(st);
          print_element(tmp->data);
        }
      }
      else {
        // step 3: update the curr to right link
        curr = tmp;
      }
    }
  } // while
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


// ==============================================
// TNode Stack Associated Functions Implementation
// ==============================================
static TNodeStack* tnodestack_init(int capacity) {
  if (capacity <= 0) return NULL;

  // allocate memory for the struct
  TNodeStack *st = calloc(1, sizeof(TNodeStack) + (sizeof(TNode *) * (size_t)capacity));
  if (!st) return NULL;

  // initialize values
  st->top = -1;
  st->capacity = capacity;

  st->push = push;
  st->pop = pop;
  st->peek = peek;
  st->is_empty = is_empty;
  st->is_full  = is_full;

  return st;
}


static void push(TNodeStack *st, TNode *node) {
  if (!st || !node || is_full(st)) return;

  st->tnodes[++st->top] = node;
}


static TNode* pop(TNodeStack *st) {
  if (!st || is_empty(st)) return NULL;

  return st->tnodes[st->top--];
}


static TNode* peek(TNodeStack *st) {
  if (!st || is_empty(st)) return NULL;

  return st->tnodes[st->top];
}


static bool is_empty(TNodeStack *st) {
  return (!st || st->top == -1);
}


static bool is_full(TNodeStack *st) {
  return (st->top == (st->capacity - 1));
}
