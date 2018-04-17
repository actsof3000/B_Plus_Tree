#include "B+_Tree.h"

void Node::print(std::ostream &out)
{
  print(out, this, 0);
}

void Node::print(std::ostream &out, Node *node, int level)
{
  if (!node || !(node->data[0]))
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


void Node::graph(std::ostream &out, int &numNode, int &numLeaf)
{
  if (this && this->data)
  {
    // static int nodeNum = 0;
    // static int leafNum = 0;
    int number = numNode;
    if (this->type == internal)
    {
      out << "node" << numNode << "[label = \"";
      for (int i = 0; i < 3; i++)
      {
        out << "<f" << i << "> ·|";
        if (this->data[i])
          out << this->data[i];
        else
          out << " ";
        out << "|";
      }
      out << "<f3> ·\"];" << std::endl;
      numNode++;

      for (int i = 0; i < 4; i++)
      {
        if (this->nodes[i])
        {
          out << "\"node" << number << "\":f" << i;
          if (this->nodes[i]->type == leaf)
            out << " -> \"leaf" << numLeaf << "\";" << std::endl;
          else
            out << " -> \"node" << numNode << "\";" << std::endl;
        }
        this->nodes[i]->graph(out, numNode, numLeaf);
      }
    }
    else
    {
      out << "leaf" << numLeaf << "[label = \"";
      for (int i = 0; i < 3; i++)
      {
        if (this->data[i])
          out << this->data[i] << "|o|";
        else
          out << " |";
      }
      out << "<f0> ·\"];" << std::endl;
      if (numLeaf)
      {
        out << "\"leaf" << numLeaf - 1 << "\":f0"
            << " -> \"leaf" << numLeaf << "\";" << std::endl;
      }
      numLeaf++;
    }
  }
}

int Node::maxNum()
{
  if (!this)
    return 0;
  int max = 0;
  if (this->type == leaf)
  {
    for (int i = 0; i < 3; i++)
    {
      max = (this->data[i] > max) ? this->data[i] : max;
    }
    return max;
  }

  //If not leaf, return max of children
  int i = 0;
  while (this->nodes[i])
  {
    int m = this->nodes[i]->maxNum();
    max = (m > max) ? m : max;
    i++;
  }
  return max;
}

Node *Node::split()
{
  if (!this)
    return nullptr;

  int nData[4] = {0};
  for (int i = 0; i < 4; i++)
    nData[i] = this->data[i];

  Node *nNode = new Node((NodeType)this->type);
  if (this->type == leaf)
  {
    if (this->size() < 2)
    {
      delete nNode;
      return nullptr;
    }
    nNode->nodes[0] = this;
    int numSplit = (int)((float)this->size() / 2 + 0.5f);
    for (int i = 0; i < numSplit; i++)
      nNode->data[i] = this->data[i];
    for (int i = 0; i < 4; i++)
    {
      if (i < numSplit)
        this->data[i] = this->data[i + numSplit];
      else
        this->data[i] = 0;
    }
  }
  else
  {
    if (this->size() > 2) //4 nodes, split 2 and 2
    {
      nNode->nodes[0] = this->nodes[0];
      nNode->nodes[1] = this->nodes[1];
      this->nodes[0] = this->nodes[2];
      this->nodes[0] = this->nodes[3];
      this->nodes[2] = nullptr;
      this->nodes[3] = nullptr;

      nNode->data[0] = nNode->nodes[0]->maxNum();
      this->data[0] = this->nodes[0]->maxNum();
    }
    else
    {
      int pos = 0;
      Node *nChild = nullptr;
      while (this->nodes[pos] && !(nChild = this->nodes[pos]->split()))
        pos++;
      if (pos < 2)
      {
        for (int i = 0; i < pos; i++)
          nNode->nodes[i] = this->nodes[i];
        nNode->nodes[pos] = nChild;
        for (int i = pos + 1; i < 1; i++)
          nNode->nodes[i] = this->nodes[i - 1];
        nNode->data[0] = nNode->nodes[0]->maxNum();

        this->nodes[0] = this->nodes[2];
        this->nodes[1] = this->nodes[3];
        for (int i = 2; i < 4; i++)
          this->nodes[i] = nullptr;
        this->data[0] = this->nodes[0]->maxNum();
      }
      else
      {
        //Giving newNode children and data
        for (int i = 0; i < 2; i++)
          nNode->nodes[i] = this->nodes[i];
        nNode->data[0] = nNode->nodes[0]->maxNum();

        this->nodes[0] = nChild;
        this->nodes[1] = this->nodes[3];
        for (int i = 2; i < 4; i++)
          this->nodes[i] = nullptr;

        for (int i = 0; i < 4; i++)
          this->data[i] = 0;
        this->data[0] = this->nodes[0]->maxNum();
      }
    }
  }
  return nNode;
}

Node *B_Plus_Tree::insert(Node *node, int num)
{
  int pos = 0;
  while (node->data[pos] < num && node->data[pos] != 0)
    pos++;

  if (node->type == leaf)
  {
    if (node->data[pos] == num)
      return nullptr;
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
        for (int i = 0; i < 4; i++)
          node->data[i] = 0;
        node->data[0] = data;
        node->nodes[0] = node->nodes[2];
        node->nodes[1] = node->nodes[3];
      }
      else
      {
        //Giving newNode children and data
        for (int i = 0; i < 3; i++)
          nNode->nodes[i] = node->nodes[i];
        for (int i = 0; i < 2; i++)
          nNode->data[i] = node->data[i];

        node->nodes[0] = nChild;
        node->nodes[1] = node->nodes[3];
        for (int i = 2; i < 4; i++)
          node->nodes[i] = nullptr;

        for (int i = 0; i < 4; i++)
          node->data[i] = 0;
        node->data[0] = carry;
      }
      return nNode;
    }

    //No split required
    for (int i = 3; i > pos; i--)
    {
      node->nodes[i] = node->nodes[i - 1];
    }

    node->nodes[pos] = nChild;
    return nullptr;
  }
  return nullptr;
}

int *Node::collect()
{
  int *values;
  if (this->type == leaf)
  {
    values = new int[4]{0};
    for (int i = 0; i < 4; i++)
      values[i] = this->data[i];
  }
  else
  {
    int size = 16;
    values = new int[size]{0};
    int valPos = 0;
    int i = 0;
    while (this->nodes[i])
    {
      int *coll = this->nodes[i]->collect();
      int j = 0;
      while (coll[j])
      {
        if (valPos == size - 1)
        {
          size *= 2;
          int *nValues = new int[size]{0};
          for (int k = 0; k < valPos; k++)
            nValues[k] = values[k];
          delete values;
          values = nValues;
        }
        values[valPos] = coll[j];
        valPos++;
        j++;
      }
      i++;
    }
  }
  return values;
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

int *B_Plus_Tree::deleteNum(Node *node, int num)
{
  if (node)
  {
    int pos = 0;
    while (node->data[pos] < num && node->data[pos] != 0)
      pos++;

    if (node->type == leaf)
    {
      if (node->data[pos] == num)
      {
        for (int i = pos; i < 3; i++)
        {
          node->data[i] = node->data[i + 1];
        }
      }
      return nullptr;
    }
    //Only internal nodes
    int *data = deleteNum(node->nodes[pos], num);
    if (node->nodes[pos]->type == leaf)
    {
      for (int i = 0; i < 3; i++)
        node->data[i] = node->nodes[i]->maxNum();
      int numChildren = node->numChildren();
      for (int i = 3; i >= numChildren - 1; i--)
        node->data[i] = 0;
      if (node->nodes[pos]->size() < 1)
      {
        delete node->nodes[pos];
        node->nodes[pos] = nullptr;
        for (int i = pos; i < 3; i++)
        {
          node->nodes[i] = node->nodes[i + 1];
        }
        node->data[numChildren - 2] = 0;

        if (node->numChildren() < 2)
        {
          Node *nNode = node->nodes[0]->split();
          if (nNode)
          {
            node->nodes[1] = node->nodes[0];
            node->nodes[0] = nNode;
            node->data[0] = node->nodes[0]->maxNum();
          }
          else
          {
            return node->collect();
          }
        }
      }
      return nullptr;
    }
    else
    {
      if (data)
      {
        node->nodes[pos]->destroy();
        delete node->nodes[pos];
        node->nodes[pos] = nullptr;

        for (int i = pos; i < 3; i++)
        {
          node->nodes[i] = node->nodes[i + 1];
        }
        int numChildren = node->numChildren();
        for (int i = 0; i < 3; i++)
        {
          if (i < numChildren - 1)
            node->data[i] = node->nodes[i]->maxNum();
          else
            node->data[i] = 0;
        }

        if (node->numChildren() < 2)
        {
          Node *nChild = node->nodes[0]->split();
          if (nChild)
          {
            node->nodes[1] = node->nodes[0];
            node->nodes[0] = nChild;
            node->data[0] = node->nodes[0]->maxNum();
          }
          else
          {
            //add collect to data
            int dataSize = 0;
            while (data[dataSize])
              dataSize++;
            int *coll = node->nodes[0]->collect();
            int collSize = 0;
            while (coll[collSize])
              collSize++;
            int *nData = new int[dataSize + collSize + 1]{0};
            for (int i = 0; i < dataSize; i++)
              nData[i] = data[i];
            for (int i = 0; i < collSize; i++)
              nData[i + dataSize] = coll[i];
            delete data;
            data = nData;
          }
        }
        return data;
      }
    }
  }
  return nullptr;
}

void B_Plus_Tree::deleteNum(int num)
{
  if (root)
  {
    int pos = 0;
    while (root->data[pos] < num && root->data[pos] != 0)
      pos++;
    int *data = deleteNum(root->nodes[pos], num);

    if (data)
    {
      if (root->nodes[pos]->numChildren() < 2)
      {
        root->nodes[pos]->destroy();
        delete root->nodes[pos];
        root->nodes[pos] = nullptr;

        for (int i = pos; i < 3; i++)
        {
          root->nodes[i] = root->nodes[i + 1];
          root->data[i] = root->nodes[i]->maxNum();
        }

        if (root->numChildren() < 2)
        {
          Node *oRoot = root;
          root = root->nodes[0];
          delete oRoot;
        }
      }
      int i = 0;
      while (data[i])
      {
        insert(data[i]);
        i++;
      }
    }
  }
}

std::ostream &operator<<(std::ostream &out, B_Plus_Tree &tree)
{
  tree.root->print(out);
  return out;
}

void B_Plus_Tree::graphvizPrint(std::fstream &file)
{
  file << "digraph\n{" << std::endl;
  file << "node [shape = record,height=.1];" << std::endl;
  int x = 0, y = 0; //Just used to init numNode and numLeaf
  root->graph(file, x, y);
  file << std::endl;
  file << "}" << std::endl;
}
