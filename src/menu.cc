#include <stdio.h>
#include "array.hpp"
#include "list.hpp"
#include "heap.hpp"
#include "test.hpp"

int main()
{
	TEST("Heap test", run_heap_tests);
  return 0;
}
