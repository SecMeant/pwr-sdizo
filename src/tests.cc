#include "test.hpp"
#include "array.hpp"
#include "list.hpp"
#include "heap.hpp"
#include "tree.hpp"
#include <random>

#define TEST_RETURN_ON_FALSE(test_func, ...) if(!test_func(__VA_ARGS__)) return false;
#define TEST_RETURN_ON_TRUE(test_func, ...) if(test_func(__VA_ARGS__)) return false;

// TODO(holz) make betters tests for array and list.
bool sdizo::tests::test_array()
{
  using sdizo::Array;

  Array array;
  array.contains(2);
  array.insert(1, 0);
  array.contains(2);
  array.insert(2, 1);
  array.contains(2);
  array.insert(3, 2);
  array.insert(4, 1);
  array.contains(2);
  array.remove(3);
  array.contains(2);
  array.remove(0);
  array.contains(2);
  array.remove(0);
  array.contains(2);
  array.remove(0);
  array.contains(2);

  array.generate(-20, 74, 20);
  array.add(1337, 0);

  return true;
}

bool sdizo::tests::test_list()
{
  using sdizo::List;

  try{
    List list;
    list.insert(12,0);
    list.insert(2,0);
    list.insert(3,0);
    list.insert(4,1);
    list.insert(5,1);
    list.insert(1337,5);
    list.insert(23232,6);
    list.insert(23232,7);
    list.insert(23232,8);
    list.removeAt(2);
    list.removeAt(0);

    try{
      // should throw
      list.removeAt(3000);
      return false;
    }catch(std::out_of_range &e){}

    list.remove(12312321);
    list.remove(12);
    list.remove(5);
    list.remove(23232);
    list.remove(23232);
    list.removeAt(0);
    list.removeAt(0);
    list.insert(1337, 0);
  }catch(...){
    printf("[%s] Exception caught.\n", __PRETTY_FUNCTION__);
    return false;
  }

  return true;
}

bool sdizo::tests::test_list2()
{
  using sdizo::List;

  try{
    List list;
    list.generate(-15, 23, 10);

    if(list.contains(1337))
      return false;

    list.add(1337, 0);

    if(!list.contains(1337))
      return false;
  }catch(...){
    printf("[%s] Exception caught.", __PRETTY_FUNCTION__);
    return false;
  }
  return true;
}

bool sdizo::tests::test_heap()
{
  using sdizo::Heap;
  
  Heap heap;
  heap.generate(0, 20, 7);
  heap.generate(0, 20, 7);

  return heap.verify();
}

bool sdizo::tests::test_bst()
{
	using sdizo::Tree;

	Tree tree;
	TEST_RETURN_ON_TRUE(tree.search, 2);
	tree.insert(23);
	TEST_RETURN_ON_FALSE(tree.verify);
  tree.insert(123);
	TEST_RETURN_ON_FALSE(tree.verify);
  tree.insert(12);
	TEST_RETURN_ON_FALSE(tree.verify);
  tree.insert(1);
	TEST_RETURN_ON_FALSE(tree.verify);
  tree.insert(2);
	TEST_RETURN_ON_FALSE(tree.verify);
	TEST_RETURN_ON_TRUE(tree.search, 1337);
	tree.insert(1337);
	TEST_RETURN_ON_FALSE(tree.search, 1337);

	return true;
}

bool sdizo::tests::run_array_tests()
{
  if(!test_array())
    return false;
  
  return true;
}

bool sdizo::tests::run_list_tests()
{
  if(!test_list())
    return false;
  
  if(!test_list2())
    return false;

  return true;
}

bool sdizo::tests::run_heap_tests()
{
  if(!test_heap())
    return false;

  return true;
}

bool sdizo::tests::run_bst_tests()
{
	if(!test_bst())
		return false;
	
	return true;
}
