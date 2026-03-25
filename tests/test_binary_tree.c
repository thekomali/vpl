#include "../include/vpl/binary_tree.h"


int main() {
  char *d[] = {"A", "B", "C", "D", "E", "F", "G"};

  // initialize a binary tree
  BinaryTree *bt = binarytree_init();
  bt->size = 7;

  TNode *na = new_node(STR, d[0]);
  TNode *nb = new_node(STR, d[1]);
  TNode *nc = new_node(STR, d[2]);
  TNode *nd = new_node(STR, d[3]);
  TNode *ne = new_node(STR, d[4]);
  TNode *nf = new_node(STR, d[5]);
  TNode *ng = new_node(STR, d[6]);

  // link the nodes to tree from
  na->left = nb;
  na->right = nc;

  nb->left = nd;
  nb->right = ne;

  nc->left = nf;
  nc->right = ng;

  // let's traverse the tree
  bt->root = na;
  printf("preorder: ");
  bt->preorder_recv(bt->root);
  puts("");

  printf("inorder: ");
  bt->inorder_recv(bt->root);
  puts(" ");

  printf("postorder: ");
  bt->postorder_recv(bt->root);
  puts(" ");

  printf("preorder non-recurv: ");
  bt->preorder(bt);
  puts(" ");

  printf("inorder non-recurv: ");
  bt->inorder(bt);
  puts(" ");

  printf("postorder non-recurv: ");
  bt->postorder(bt);
  puts(" ");

  return 0;
}
