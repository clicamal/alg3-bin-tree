#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  int val;
  struct node *left, *right;
} node;

node *create_node(int val) {
  node *n = (node *) malloc(sizeof(node));

  if (n == NULL) return NULL;

  n->val = val;
  n->left = NULL;
  n->right = NULL;

  return n;
}

void destroy_node(node **n) {
  if ((*n)->left == NULL && (*n)->right == NULL) {
    free(*n);
    *n = NULL;
  }

  else if ((*n)->left == NULL) {
    destroy_node(&(*n)->right);
  }

  else {
    destroy_node(&(*n)->left);
  }
}

void insert_node(node **insert_root, int val) {
  if (*insert_root == NULL) *insert_root = create_node(val);
  else if ((*insert_root)->val > val) insert_node(&(*insert_root)->left, val);
  else insert_node(&(*insert_root)->right, val);
}

void print_node(node *n, int lvl) {
  for (int i = 0; i < lvl; i++)
    printf("  ");

  if (n == NULL) {
    printf("#\n");
  } else {
    printf("%d\n", n->val);

    lvl++;

    print_node(n->left, lvl);
    print_node(n->right, lvl);
  }
}

int main(void) {
  node *root = create_node(0);

  insert_node(&root, 5);
  insert_node(&root, 10);

  insert_node(&root, 4);
  insert_node(&root, 3);

  print_node(root, 0);

  destroy_node(&root);

  return 0;
}
