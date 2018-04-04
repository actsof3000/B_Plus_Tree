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
  if (node->type == internal)
  {
    int i = 0;
    while ((bool)node->nodes[i] && i < 4)
    {
      print(out, node->nodes[i], level + 1);
      i++;
    }
  }
}

int Node::maxNum()
{
  int max = 0;
  if (this->type == leaf)
  {
    for (int i = 0; i < 3; i++)
    {
      max = (this->data[i] > max) ? this->data[i] : max;
    }
    return max;
  }

  for (int i = 0; i < 4; i++)
  {
    int m = this->nodes[i]->maxNum();
    max = (m > max) ? m : max;
  }
  return max;
}

Node *B_Plus_Tree::insert(Node *node, int num)
{
  int pos = 0;
  while (node->data[pos] < num && node->data[pos] != 0)
    pos++;

  if (node->type == leaf)
  {
    for (int i = 4; i > pos; i--)
      node->data[i] = node->data[i - 1];
    node->data[pos] = num;

    if (node->size() == 4)
    {
      int nData[4] = {0};
      for (int i = 0; i < 4; i++)
        nData[i] = node->data[i];

      Node *nNode = new Node(leaf);
      nNode->nodes[0] = node;
      for (int i = 3; i > 1; i--)
      {
        nNode->data[i - 2] = nData[i - 2];
        node->data[i - 2] = nData[i];
        node->data[i] = 0;
      }
      return nNode;
    }
    return nullptr;
  }
  //if type == internal

  Node *nChild = insert((node->nodes[pos]), num);
  if (nChild)
  {
    int carry = nChild->maxNum();

    for (int i = 4; i > pos; i--)
      node->data[i] = node->data[i - 1];
    node->data[pos] = carry;

    if (node->size() == 4)
    {
      Node *nNode = new Node(internal);
      node->type = internal;
      if (pos != 3)
      {
        for (int i = 0; i < pos; i++)
          nNode->nodes[i] = node->nodes[i];
        nNode->nodes[pos] = nChild;
        for (int i = pos + 1; i < 2; i++)
          nNode->nodes[i] = node->nodes[i - 1];
        for (int i = 0; i < 2; i++)
          nNode->data[i] = node->data[i];

        int data = node->data[2];
        delete[] node->data;
        node->data = new int[4]{0};
        node->data[0] = data;
        node->nodes[0] = node->nodes[2];
        node->nodes[1] = node->nodes[3];
      }
      else
      {
        for (int i = 0; i < 2; i++)
        {
          nNode->nodes[i] = node->nodes[i];
          nNode->data[i] = node->data[i];
        }

        node->nodes[0] = nChild;
        node->nodes[1] = node->nodes[2];

        int data = node->data[2];
        delete[] node->data;
        node->data = new int[4]{0};
        node->data[0] = data;
      }
      return nNode;
    }

    //No split required
    int i = 3;
    do
    {
      i--;
      node->data[i + 1] = node->data[i];
      node->nodes[i + 1] = node->nodes[i];
    } while (node->nodes[i] != nChild->nodes[0]);

    node->nodes[i] = nChild;
    return nullptr;
  }
  return nullptr;
}

void B_Plus_Tree::insert(int num)
{
  if (!root)
    root = new Node(leaf);
  Node *nNode = insert(root, num);
  if (nNode) //Root has already split
  {
    Node *oNode = root;
    root = new Node(internal);
    root->nodes[0] = nNode;
    root->nodes[1] = oNode;
    root->data[0] = nNode->maxNum();
    // root->data[1] = oNode->maxNum();
  }
}

std::ostream &operator<<(std::ostream &out, B_Plus_Tree &tree)
{
  tree.root->print(out);
  return out;
}
