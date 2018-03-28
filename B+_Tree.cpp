#include "B+_Tree.h"

void Node::print(std::ostream &out, Node *node, int level)
{
  if (node)
  {
    for (int i = 0; i < level; i++)
      out << "\t";
    out << "<";
    for (int i = 0; i < 3; i++)
    {
      out << node->data[i] << "|";
    }
    out << "\b>" << std::endl;
    Node *n = node->nodes;
    while (n)
    {
      print(out, n, level + 1);
      n++;
    }
  }
}

int B_Plus_Tree::insert(Node *node, int num)
{
  if (node->type == leaf)
  {
    if (node->size() < 3)
    {
      int pos = 0;
      while (node->data[pos] < num && node->data[pos] != 0)
        pos++;
      if (pos < 2)
      {
        for (int i = 2; i > pos; i--)
          node->data[i] = node->data[i - 1];
      }
      node->data[pos] = num;
      return 0;
    }
  }
}