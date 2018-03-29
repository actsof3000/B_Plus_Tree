#include "B+_Tree.h"

void Node::print(std::ostream &out)
{
  print(out, this, 0);
}

void Node::print(std::ostream &out, Node *node, int level)
{
  if (!node || !(node->data))
    return;
  for (int i = 0; i < level; i++)
    out << "\t";
  out << "<";
  for (int i = 0; i < 3; i++)
  {
    out << node->data[i] << "|";
  }
  out << "\b>" << std::endl;
  if (!node->nodes)
    return;
  Node *n = node->nodes;
  while ((bool)n && n < (node->nodes + 4))
  {
    print(out, n, level + 1);
    n++;
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
    else
    {
      int nData[4] = {0};
      for (int i = 0; i < 3; i++)
        nData[i] = node->data[i];
      int pos = 0;
      while (node->data[pos] < num && node->data[pos] != 0)
        pos++;
      if (pos < 3)
      {
        for (int i = 3; i > pos; i--)
          node->data[i] = node->data[i - 1];
      }
      nData[pos] = num;
      
      Node *nNode = new Node(internal);
      nNode->nodes = new Node[4]{{leaf},{leaf}};
      for (int i = 0; i < 2; i++)
      {
        nNode->nodes[0].data[i] = nData[i];
        nNode->nodes[1].data[i] = nData[i + 2];
      }
      nNode->data[0] = nNode->nodes[0].data[1];
      delete node;
      *node = *nNode;
    }
  }
}

std::ostream &operator<<(std::ostream &out, B_Plus_Tree &tree)
{
  tree.root->print(out);
  return out;
}
