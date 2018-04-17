#ifndef B_PLUS_TREE_H
#define B_PLUS_TREE_H

#include <iostream>
#include <fstream>

enum NodeType
{
  internal = 0,
  leaf
};

class Node
{
public:
  //Holds data for node
  int *data;
  //Children for node
  Node *nodes[4];
  //Node type
  int type;

  Node() : data(nullptr), type(0) {}
  Node(NodeType type)
  {
    this->type = type;
    data = new int[4]{0};
    for (int i = 0; i < 4; i++)
      nodes[i] = nullptr;
  }

  ~Node()
  {
    delete[] data;
  }

  //Recursively deletes node and all children
  void destroy()
  {
    if (type == internal)
    {
      int i = 0;
      while (i < 4 && nodes[i])
      {
        nodes[i]->destroy();
        delete nodes[i];
        i++;
      }
    }
  }

  Node &operator=(const Node &node)
  {
    delete[] data;
    data = new int[4]{0};
    type = node.type;
    for (int i = 0; i < 4; i++)
    {
      nodes[i] = node.nodes[i];
      data[i] = node.data[i];
    }
  }

  //returns maximum number in data
  int maxNum();

  //returns number of children
  int numChildren()
  {
    int num = 0;
    while (nodes[num])
      num++;
    return num;
  }

  //Returns number of data elements
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

  //Splits the node into two, returns the one on the left
  Node *split();

  //Collects all data from leaf nodes in tree
  int *collect();

  //recursive function for graphing tree
  void graph(std::ostream &out, int &numNode, int &numLeaf);

  void print(std::ostream &out, Node *node, int level);
  void print(std::ostream &out);
};

class B_Plus_Tree
{
  Node *root;

  //recursive function for insertion
  Node *insert(Node *node, int num);
  //recursive function for deletion
  int *deleteNum(Node *node, int num);

public:
  B_Plus_Tree() : root(nullptr){};
  ~B_Plus_Tree()
  {
    delete root;
  }
  //Insert Number into tree
  void insert(int num);
  //Delete number from tree
  //Does nothing if number is non-existent in tree
  void deleteNum(int num);
  //Parses tree into dot language in file
  void graphvizPrint(std::fstream &file);
  friend std::ostream &operator<<(std::ostream &out, B_Plus_Tree &tree);
};

#endif