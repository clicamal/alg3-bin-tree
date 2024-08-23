#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LINE_SIZE 256

typedef enum command {
  INSERT_NODE,
  PRINT_NODE,
  RM_NODE,
  NOP,
  WRG_CMD,
  EXIT
} command;

typedef enum error {
  INV_CMD,
  INV_VAL,
  NO_ERR
} error;

typedef struct node {
  int val;
  struct node *left, *right;
} node;

node *create_node(int);
void destroy_node(node **);
void insert_node(node **, int);
node *get_gt_sub_left(node *);
void rm_node(node **, int);
void print_node(node *, int);
char *next_token(char *);
void read_line(char *);
bool str_is_equal(char *, char *);
command parse_command(char *);
int parse_value(void);

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

node *get_gt_sub_left(node *n) {
  if (n == NULL) return NULL;

  node *aux = n->left;

  while (aux->right != NULL) aux = aux->right;

  return aux;
}

void rm_node(node **n, int val) {
  if (*n == NULL) return;

  if ((*n)->val > val) {
    rm_node(&(*n)->left, val);
  }

  else if ((*n)->val < val) {
    rm_node(&(*n)->right, val);
  }

  else {
    if ((*n)->left == NULL && (*n)->right == NULL) {
      destroy_node(n);
    }

    else if ((*n)->left == NULL) {
      node *aux = *n;

      *n = (*n)->right;

      destroy_node(&aux);
    }

    else if ((*n)->right == NULL) {
      node *aux = *n;

      *n = (*n)->left;

      destroy_node(&aux);
    }

    else {
      node *gt_sub_left = get_gt_sub_left(*n);
      (*n)->val = gt_sub_left->val;
      rm_node(&(*n)->left, gt_sub_left->val);
    }
  }
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

char *next_token(char *buffer) { return strtok(buffer, " "); }

void read_line(char *buffer) {
  fgets(buffer, LINE_SIZE, stdin);

  buffer[(int8_t) (strchr(buffer, '\n') - buffer)] = '\0';
}

bool str_is_equal(char *str1, char *str2) { return strcmp(str1, str2) == 0; }

command parse_command(char *buffer) {
  char *token = next_token(buffer);

  if (token == NULL) return NOP;

  if (str_is_equal(token, "insert")) return INSERT_NODE;
  if (str_is_equal(token, "print")) return PRINT_NODE;
  if (str_is_equal(token, "remove")) return RM_NODE;
  if (str_is_equal(token, "exit")) return EXIT;

  return WRG_CMD;
}

// Must be called after parse_cmd;
int parse_value(void) {
  char end, *endptr = &end, *token = next_token(NULL);
  int val;

  if (token != NULL) val = strtol(token, &endptr, 10);
  else *endptr = '\0';

  return *endptr == '\0' ? val : -32767;
}

int main(void) {
  node *root = NULL;
  command cmd = NOP;
  error err = NO_ERR;

  while (cmd != EXIT) {
    if (err != NO_ERR) {
      if (err == INV_CMD) printf("*Invalid command*\n");
      else if (err == INV_VAL) printf("*Invalid value*\n");

      err = NO_ERR;
    }

    char buffer[LINE_SIZE];
    int val;

    printf("Command > ");
    read_line(buffer);

    cmd = parse_command(buffer);
    val = parse_value();

    if (val == -32767) err = INV_VAL;

    switch (cmd) {
    case PRINT_NODE:
      print_node(root, 0);
      break;

    case INSERT_NODE:
      if (err == INV_VAL) continue;

      insert_node(&root, val);
      break;

    case RM_NODE:
      if (err == INV_VAL) continue;

      rm_node(&root, val);
      break;

    case NOP:
      continue;

    default:
      err = INV_CMD;
      continue;
    }
  }

  destroy_node(&root);

  return 0;
}
