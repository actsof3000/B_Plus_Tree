#ifndef B_PLUS_TREE_H
#define B_PLUS_TREE_H

#include <iostream>

enum NodeType
{
  internal = 0,
  leaf
};

class Node
{
public:
  int *data;
  Node *nodes;
  int type;
  Node() : data(NULL), nodes(NULL), type(0) {}
  Node(NodeType type)
  {
    this->type = type;
    data = new int[3];
    for (int i = 0; i < 3; i++)
      data[i] = 0;
    nodes = NULL;
  }

  int size()
  {
    int size = 0;
    while (data[size])
      size++;
    return size;
  }

  static void print(std::ostream &out, Node *node, int level);
};

class B_Plus_Tree
{
  // int level;
  Node *root;

  int insert(Node *node, int num);

public:
  B_Plus_Tree() : root(NULL){};
  void insert(int num)
  {
    if (!root)
      root = new Node(leaf);
    insert(root, num);
  }
  friend std::ostream &operator<<(std::ostream &out, B_Plus_Tree &tree)
  {
    Node::print(out, tree.root, 0);
  }
};

#endif