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

  /// Preorder non-recursive traversal
  void (* preorder)(struct BinaryTree *bt);

  /// Inorder non-recursive traversal
  void (* inorder)(struct BinaryTree *bt);

  /// Postorder non-recursive traversal
  void (* postorder)(struct BinaryTree *bt);

  TNode *root;     ///< points to the root node of the tree
  int size;        ///< no of nodes in the tree
} BinaryTree;



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


// ======================
// Function Declarations
// ======================
BinaryTree* binarytree_init();

TNode* new_node(etype_t etype, void *val);
