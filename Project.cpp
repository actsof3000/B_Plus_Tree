#include "B+_Tree.h"
#include <iostream>

int main()
{
  B_Plus_Tree tree;
  for (int i = 1; i < 6; i++)
  {
    std::cout << tree << std::endl;
    tree.insert(i);
  }
  std::cout << tree << std::endl;
  
  return 0;
}