#pragma once

#include "helpers/binary_tree_helpers.h"

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

  /// Level Order non-recursive traversal
  void (* levelorder)(struct BinaryTree *bt);

  TNode *root;     ///< points to the root node of the tree
  int size;        ///< no of nodes in the tree
} BinaryTree;



// ======================
// Function Declarations
// ======================
BinaryTree* binarytree_init();

TNode* new_node(etype_t etype, void *val);
