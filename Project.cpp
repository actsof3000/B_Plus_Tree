#include "B+_Tree.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
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

int main(int argc, char *argv[])
{
  B_Plus_Tree tree;
  std::fstream fs;
  //Initial tree
  for (int i = 1; i < argc; i++)
  {
    int x = atoi(argv[i]);
    tree.insert(x);
  }

  char command = '0';
  do
  {
    std::string option = "";
    std::cout << "Choose an option: " << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "1. Insert" << std::endl;
    std::cout << "2. Delete" << std::endl;
    std::cout << "3. Print" << std::endl;
    std::cin >> option;
    command = option[0];

    int number = 0;
    switch (command)
    {
    case '0':
      break;
    case '1':
      std::cout << "Insert: " << std::endl;
      std::cin >> number;
      if (number > 999 || number < 1)
      {
        std::cout << "Give number between 1 and 999" << std::endl;
        break;
      }
      tree.insert(number);
      break;
    case '2':
      std::cout << "Delete: " << std::endl;
      std::cin >> number;
      if (number > 999 || number < 1)
      {
        std::cout << "Give number between 1 and 999" << std::endl;
        break;
      }
      tree.deleteNum(number);
      break;
    case '3':
      fs.open("graph.gv", std::fstream::out);
      tree.graphvizPrint(fs);
      fs.close();
      std::cout << exec("dot -Tjpg -O graph.gv") << std::endl;
      exec("eog graph.gv.jpg");
      break;
    default:
      std::cout << "Please give number of valid option" << std::endl;
    }
  } while (command != '0');

  return 0;
}