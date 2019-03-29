#include <stdio.h>
#include "array.hpp"
#include "list.hpp"
#include "heap.hpp"
#include "tree.hpp"
#include "test.hpp"

int main()
{
  using namespace sdizo::tests;
  using sdizo::Tree;

  TEST("Array test", run_array_tests);
  TEST("List test", run_list_tests);
  TEST("Heap test", run_heap_tests);
  TEST("BST test", run_bst_tests); 

  //Tree tree;
  //tree.insert(23);
  //tree.insert(123);
  //tree.insert(12);
  //tree.insert(1);
  //tree.insert(2);
  //tree.display();
  //tree.dsw();
  //tree.display();

  //Tree tree2;
  //tree2.insert(10);
  //tree2.insert(7);
  //tree2.insert(13);
  //tree2.insert(6);
  //tree2.insert(12);
  //tree2.insert(15);
  //tree2.insert(14);
  //tree2.dsw();
  //tree2.display();
  //if(tree2.verify_connections())
  //  puts("Childs OK");

  return 0;
}
