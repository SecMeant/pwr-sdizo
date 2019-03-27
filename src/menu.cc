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

  //TEST("Array test", run_array_tests);
  //TEST("List test", run_list_tests);
  //TEST("Heap test", run_heap_tests);
  
  Tree tree;
  tree.insert(23);
  tree.insert(123);
  tree.insert(12);
  tree.insert(1);
  tree.insert(2);
  tree.display();

  return 0;
}
