#pragma once

#include "util.h"

/**
 * @file binary_tree.h
 * @brief Binary tree implementation using Linked Nodes
 *
 */


/// Struct representation of BinaryTree
typedef struct BinaryTree {
  /// Preorder recursive traversal
  void (* preorder_recv)(TNode *root);

  /// Inorder recursive traversal
  void (* inorder_recv)(TNode *root);

  /// Postorder recursive traversal
  void (* postorder_recv)(TNode *root);

  TNode *root;     ///< points to the root node of the tree
  int size;        ///< no of nodes in the tree
} BinaryTree;



/// Struct representation of Stack to hold TNodes (use in non-recursive traversal of tree)
typedef struct TNodeStack {
  /// Push element into stack
  void (* push)(struct TNodeStack, TNode *node);

  /// Pop element from stack
  TNode* (* pop)(struct TNodeStack);

  /// Is stack empty?
  bool (* is_empty)(struct TNodeStack);

  /// Is stack full?
  bool (* is_full)(struct TNodeStack);

  int top;          ///< points to index of top element
  int capacity;     ///< max elements the stack can hold
  int size;         ///< no of elements in the stack
  TNode *tnodes[];  ///< flexiable array member - array of pointers to TNode
} TNodeStack;


// ======================
// Function Declarations
// ======================
