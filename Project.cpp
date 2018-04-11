#include "B+_Tree.h"
#include <iostream>
#include <memory>

std::string exec(const char *cmd)
{
  std::array<char, 128> buffer;
  std::string result;
  std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
  if (!pipe)
    throw std::runtime_error("popen() failed!");
  while (!feof(pipe.get()))
  {
    if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
      result += buffer.data();
  }
  return result;
}

int main()
{
  // int arr[8] = {8, 5, 1, 7, 3, 12, 9, 6};
  B_Plus_Tree tree;
  for (int i = 1; i < 21; i++)
  {
    // std::cout << tree << std::endl;
    tree.insert(i);
  }
  std::cout << tree << std::endl;

  for(int i = 1; i<13; i++)
  {
    std::cout << "Delete " << i << std::endl;
    tree.deleteNum(i);
    std::cout<< tree << std::endl;
  }
  // std::cout << exec("dot -Tjpg -O graph.gv") << std::endl;
  // exec("eog graph.gv.jpg");

  return 0;
}