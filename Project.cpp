#include "B+_Tree.h"
#include <iostream>
#include <fstream>
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
  //Initial tree
  int arr[15] = {12,35,3,20,85,22,5,10,1,18,11,55,99,36,47};
  B_Plus_Tree tree;
  for (int i = 0; i < 15; i++)
  {
    // std::cout << tree << std::endl;
    tree.insert(arr[i]);
  }
  std::cout << tree << std::endl;
   std::fstream fs;
  //series of insert/deletes
  tree.deleteNum(85);
  std::cout << "Del: 85\n" << tree << std::endl;
  tree.deleteNum(84);
  std::cout << "Del: 84\n" << tree << std::endl;
  tree.deleteNum(18);
  std::cout << "Del: 18\n" << tree << std::endl;
  tree.insert(19);
  std::cout << "Ins: 19\n" << tree << std::endl;
  tree.deleteNum(5);
  std::cout << "Del: 5\n" << tree << std::endl;
  tree.deleteNum(47);
  std::cout << "Del: 47\n" << tree << std::endl;
  tree.deleteNum(99);
  std::cout << "Del: 99\n" << tree << std::endl;
  tree.insert(11);
  std::cout << "Ins: 11\n" << tree << std::endl;
  
  //Making and opening graph
  fs.open("nGraph2.gv", std::fstream::out);
  tree.graphvizPrint(fs);
  fs.close();
  std::cout << exec("dot -Tjpg -O nGraph2.gv") << std::endl;
  exec("eog nGraph2.gv.jpg");

  return 0;
}