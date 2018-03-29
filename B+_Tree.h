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
  Node() : data(nullptr), nodes(nullptr), type(0) {}
  Node(NodeType type)
  {
    this->type = type;
    data = new int[3]{0};
    nodes = nullptr;
  }

  ~Node()
  {
    if (data)
      delete[] data;
    if (nodes)
      delete[] nodes;
    data = nullptr;
    nodes = nullptr;
  }

  int size()
  {
    int size = 0;
    while (data[size])
      size++;
    return size;
  }

  void print(std::ostream &out, Node *node, int level);
  void print(std::ostream &out);
};

class B_Plus_Tree
{
  // int level;
  Node *root;

  int insert(Node *node, int num);

public:
  B_Plus_Tree() : root(nullptr){};
  ~B_Plus_Tree()
  {
    delete root;
  }
  void insert(int num)
  {
    if (!root)
      root = new Node(leaf);
    insert(root, num);
  }
  friend std::ostream &operator<<(std::ostream &out, B_Plus_Tree &tree);
};

#endif