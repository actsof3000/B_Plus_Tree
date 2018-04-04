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
  Node *nodes[4];
  int type;
  Node() : data(nullptr), type(0) {}
  Node(NodeType type)
  {
    this->type = type;
    data = new int[4]{0};
    for(int i = 0; i<4; i++)
      nodes[i] = nullptr;
  }

  ~Node()
  {
    if (data)
      delete[] data;
    data = nullptr;
    for(int i = 0; i<4; i++)
      delete nodes[i];
  }

  Node& operator=(const Node& node)
  {
    // delete[] data;
    // delete[] nodes;
    type = node.type;
    data = node.data;
    for(int i = 0; i<4; i++)
      nodes[i] = node.nodes[i];
  }

  int maxNum();

  int size()
  {
    int size = 0;
    if (data)
    {
      while (data[size])
        size++;
    }
    return size;
  }

  void print(std::ostream &out, Node *node, int level);
  void print(std::ostream &out);
};

class B_Plus_Tree
{
  // int level;
  Node *root;

  Node *insert(Node *node, int num);

public:
  B_Plus_Tree() : root(nullptr){};
  ~B_Plus_Tree()
  {
    delete root;
  }
  void insert(int num);
  friend std::ostream &operator<<(std::ostream &out, B_Plus_Tree &tree);
};

#endif