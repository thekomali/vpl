#include "../include/vpl/binary_tree.h"\

// ============================================
// Forward declarations (what this module provides)
// ============================================
static void preorder_recv(TNode *root);

static void inorder_recv(TNode *root);

static void postorder_recv(TNode *root);

static void preorder(BinaryTree *self);

static void inorder(BinaryTree *self);

static void postorder(BinaryTree *self);


// UTILITY FUNCTION
static void print_element(element_t ele);

// TNodeStack ASSOCIATED FUNCTIONS
static TNodeStack* tnodestack_init(int capacity);

static void push(TNodeStack *st, TNode *node);

static TNode* pop(TNodeStack *st);

static bool is_empty(TNodeStack *st);

static bool is_full(TNodeStack *st);

static void tnodestack_free(TNodeStack *st);


// ============================================
// Public API
// ============================================




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

  preorder_recv(root->left);
  print_element(root->data);    // node is read after traversing left subtree
  preorder_recv(root->right);
}


static void postorder_recv(TNode *root) {
  // exit condition of recursive loop
  if (!root) return;

  preorder_recv(root->left);
  preorder_recv(root->right);
  print_element(root->data);    // node is read after traversing left & right subtree
}


static void preorder(BinaryTree *bt) {
  // TODO

}


// ============================================
// Util Function Implementation
// ============================================
void print_element(element_t ele) {
  switch (ele.etype) {
    case INT: printf("%d", ele.value.ival);  break;
    case DOU: printf("%lf", ele.value.dval); break;
    case STR: printf("%s", ele.value.sval);  break;
    default: fprintf(stderr, "ERROR: INVALID ELEMENT TYPE");
  }
}



// ==============================================
// TNode Stack Associated Functions Implementation
// ==============================================
static TNodeStack* tnodestack_init(int capacity) {
  if (capacity <= 0) return NULL;

  // allocate memory for the struct
  TNodeStack *st = calloc(1, sizeof(TNodeStack) + (sizeof(TNode *) * capacity));
  if (!st) return NULL;

  // initialize values
  st->top = -1;
  st->size = 0;
  st->capacity = capacity;

  st->push = push;
  st->pop = pop;
  st->is_empty = is_empty;
  st->is_full  = is_full;

  return st;
}


static void push(TNodeStack *st, TNode *node) {
  if (!st || !node || is_full(st)) return;

  st->tnodes[++st->top] = node;
  st->size++;
}


static TNode* pop(TNodeStack *st) {
  if (!st || is_empty(st)) return NULL;

  return st->tnodes[st->top--];
}


static bool is_empty(TNodeStack *st) {
  return (!st || st->top == -1);
}


static bool is_full(TNodeStack *st) {
  return (!st || (st->top == (st->capacity - 1)));
}
