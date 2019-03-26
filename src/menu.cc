#include <stdio.h>
#include "array.hpp"
#include "list.hpp"
#include "heap.hpp"
#include "test.hpp"

int main()
{
	using namespace sdizo::tests;

  TEST("Array test", run_array_tests);
  TEST("List test", run_list_tests);
  TEST("Heap test", run_heap_tests);
  return 0;
}
