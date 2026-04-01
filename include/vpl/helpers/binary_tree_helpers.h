#include "../util.h"


/// Struct representation of Stack to hold TNodes (use in non-recursive traversal of tree)
typedef struct TNodeStack {
    /// Push element into stack
    void (* push)(struct TNodeStack *st, TNode *node);

    /// Pop element from stack
    TNode* (* pop)(struct TNodeStack *st);

    /// Peek the top element
    TNode* (* peek)(struct TNodeStack *st);

    /// Is stack empty?
    bool (* is_empty)(struct TNodeStack *st);

    /// Is stack full?
    bool (* is_full)(struct TNodeStack *st);

    int top;          ///< points to index of top element
    int capacity;     ///< max elements the stack can hold
    TNode *tnodes[];  ///< flexiable array member - array of pointers to TNode
} TNodeStack;


/// Struct representation of Queue to hold TNodes (use in Level Order traversal of tree)
typedef struct TNodeQueue {
    /// Push element into stack
    void (* enqueue)(struct TNodeQueue *q, TNode *node);

    /// Pop element from stack
    TNode* (* dequeue)(struct TNodeQueue *q);

    /// Peek the top element
    TNode* (* peek)(struct TNodeQueue *q);

    /// Is stack empty?
    bool (* is_empty)(struct TNodeQueue *q);

    /// Is stack full?
    bool (* is_full)(struct TNodeQueue *q);

    int front;        ///< front of queue - nodes are removed here
    int rear;         ///< rear of queue - nodes are added here
    int capacity;     ///< max elements the stack can hold
    TNode *tnodes[];  ///< flexible array memeber - array of pointers to TNode
} TNodeQueue;


// ======================
// Function Declarations
// ======================
/**
 * @brief Allocate memory for TNodeStack and initialize with default values
 * @param capacity - maximum size of stack
 * @return pointer to initialized TNodeStack or NULL
 */
TNodeStack* tnodestack_init(int capacity);

/**
 * @brief Allocate memory for TNodeQueue and initialize with default values
 * @param capacity - maximum size of queue
 * @return pointer to initialized TNodeQueue or NULL
 */
TNodeQueue* tnodequeue_init(int capacity);
