#include <iostream>

#include "src/factorial.hpp"
#include "src/AVLTree.h"

int main()
{
  AVLTree<int> avl;

  std::cout << "Inserting values into AVLTree..." << std::endl;
  avl.insert(3);
  avl.insert(4);
  avl.insert(5);
  avl.insert(6);

  avl.printInOrder();

  std::cout << "\nSerializing the tree...\n";
  std::string str = avl.serialize();

  std::cout << str << std::endl;

  std::cout << "\nDeserializing the tree..." << std::endl;
  AVLTree<int> DeSerAV = avl.deserialize(str);
  DeSerAV.printInOrder();

  std::cout << "\nRemoving values from 4 and 6 from the tree..." << std::endl;
  avl.remove(6);
  avl.remove(4);
  avl.printInOrder();



}