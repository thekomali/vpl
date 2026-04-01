#include "../../include/vpl/helpers/binary_tree_helpers.h"

// ============================================
// Forward declarations (what this module provides)
// ============================================
// TNodeStack ASSOCIATED FUNCTIONS
static void push(TNodeStack *st, TNode *node);

static TNode* pop(TNodeStack *st);

static TNode* peek(TNodeStack *st);

static bool is_empty(TNodeStack *st);

static bool is_full(TNodeStack *st);

// TNodeQueue ASSOCIATED FUNCTIONS
static void enqueue(TNodeQueue *q, TNode *node);

static TNode* dequeue(TNodeQueue *q);

static TNode* q_peek(TNodeQueue *q);

static bool q_is_empty(TNodeQueue *q);

static bool q_is_full(TNodeQueue *q);


// ============================================
// Public API
// ============================================
TNodeStack* tnodestack_init(int capacity) {
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


TNodeQueue* tnodequeue_init(int capacity) {
  if (capacity <= 0) return NULL;

  // allocate memory for the struct
  TNodeQueue *q = calloc(1, sizeof(TNodeQueue) + (sizeof(TNode *) * (size_t)capacity));
  if (!q) return NULL;

  // initialize values
  q->front = -1;
  q->rear = -1;
  q->capacity = capacity;

  q->enqueue = enqueue;
  q->dequeue = dequeue;
  q->peek = q_peek;
  q->is_empty = q_is_empty;
  q->is_full  = q_is_full;

  return q;
}

// ==============================================
// TNode Stack Associated Functions Implementation
// ==============================================
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

// ==============================================
// TNode Queue Associated Functions Implementation
// ==============================================
static void enqueue(TNodeQueue *q, TNode *node) {
  if (!q || !node || q_is_full(q)) return;

  // get the insert postion
  int insert_at = (q->rear + 1) % q->capacity;

  q->tnodes[insert_at] = node;
  q->rear = insert_at;

  // handle the front position for first enqueue operation
  if (q->front == -1) q->front = 0;
}


static TNode* dequeue(TNodeQueue *q) {
  if (!q || q_is_empty(q)) return NULL;

  TNode* pop = q->tnodes[q->front];

  // check for only one element
  if (q->rear == q->front) {
    q->rear = -1;
    q->front = -1;
  }
  else {
    q->front = (q->front + 1) % q->capacity;
  }

  return pop;
}


static TNode* q_peek(TNodeQueue *q) {
  if (!q || q_is_empty(q)) return NULL;

  return q->tnodes[q->front];
}


static bool q_is_empty(TNodeQueue *q) {
  return (q->front == -1 && q->rear == -1);
}


static bool q_is_full(TNodeQueue *q) {
  return (q->rear + 1) % q->capacity == q->front;
}
