#include "test.hpp"
#include "array.hpp"
#include "list.hpp"
#include "heap.hpp"
#include "tree.hpp"
#include "redblacktree.hpp"
#include <random>
#if __cplusplus == 201703L
#define TESTS_CPP_17 true
#include <filesystem>
#include <fstream>
#endif

#define TEST_INVOKE_ASSERT_TRUE(test_func, ...) if(!test_func(__VA_ARGS__)) return false;
#define TEST_INVOKE_ASSERT_FALSE(test_func, ...) if(test_func(__VA_ARGS__)) return false;
#define TEST_ASSERT_TRUE(statement) if(!(statement)) return false;
#define TEST_ASSERT_FALSE(statement) if((statement)) return false;
#define TEST_ASSERT_EQ(a,b) if((a)!=(b)) return false;

// TODO(holz) make betters tests for array and list.
bool sdizo::tests::test_array()
{
  using sdizo::Array;

  Array array;
  try{
    array.append(13);
    TEST_ASSERT_TRUE(array.contains(13))
    array.prepend(123123);
    TEST_ASSERT_TRUE(array.at(0) == 123123)
    TEST_ASSERT_FALSE(array.contains(2))
    array.insert(1, 0);
    TEST_ASSERT_FALSE(array.contains(2))
    array.insert(2, 1);
    TEST_ASSERT_TRUE(array.contains(2))
    array.insert(3, 2);
    array.insert(4, 1);
    TEST_ASSERT_TRUE(array.contains(2))
    array.remove(3);
    TEST_ASSERT_TRUE(array.contains(2))
    TEST_ASSERT_FALSE(array.contains(3))
    array.remove(0);
    TEST_ASSERT_TRUE(array.contains(2))
    array.remove(0);
    array.contains(2);
    array.remove(0);
    array.contains(2);

    array.generate(-20, 74, 20);
    TEST_ASSERT_TRUE(array.get_size() == 20)

    array.update(0, 1337);
    TEST_ASSERT_TRUE(array.at(0) == 1337)
  }catch(...){
    return false;
  }

  return true;
}

bool sdizo::tests::test_array2()
{
  #if TESTS_CPP_17 == true
    const char *filename = "testfile";
    std::ofstream file(filename);
    file << 3 << '\n';
    file << 13 << '\n';
    file << 5 << '\n';
    file << 34 << '\n';
    file.close();

    sdizo::Array array;
    array.loadFromFile(filename);
    TEST_ASSERT_TRUE(array.contains(13))
    TEST_ASSERT_TRUE(array.contains(5))
    TEST_ASSERT_TRUE(array.contains(34))
    TEST_ASSERT_FALSE(array.contains(0))
    TEST_ASSERT_TRUE(array.at(0) == 13)
    std::filesystem::remove(filename);

    return true;
  #else
    return true;
  #endif
}

bool sdizo::tests::test_list()
{
  using sdizo::List;

  try{
    List<sdizo::ListNode<int32_t>> list;
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
    printf("[%s] Exception caught.\n", __func__);
    return false;
  }

  return true;
}

bool sdizo::tests::test_list2()
{
  using sdizo::List;

  try{
    List<sdizo::ListNode<int32_t>> list;
    list.generate(-15, 23, 10);

    if(list.contains(1337))
      return false;

    list.add(1337, 0);

    if(!list.contains(1337))
      return false;
  }catch(...){
    printf("[%s] Exception caught.", __func__);
    return false;
  }
  return true;
}

bool sdizo::tests::test_heap()
{
  using Heap = sdizo::Heap<int32_t>;

  Heap heap;
  heap.generate(0, 20, 7);
  heap.generate(0, 20, 7);

  return heap.verify();
}

bool sdizo::tests::test_bst()
{
  using sdizo::Tree;

  Tree tree;
  TEST_INVOKE_ASSERT_FALSE(tree.search, 2);
  tree.insert(23);
  TEST_INVOKE_ASSERT_TRUE(tree.verify_values);
  tree.insert(123);
  TEST_INVOKE_ASSERT_TRUE(tree.verify_values);
  tree.insert(12);
  TEST_INVOKE_ASSERT_TRUE(tree.verify_values);
  tree.insert(1);
  TEST_INVOKE_ASSERT_TRUE(tree.verify_values);
  tree.insert(2);
  TEST_INVOKE_ASSERT_TRUE(tree.verify_values);
  TEST_INVOKE_ASSERT_FALSE(tree.search, 1337);
  tree.insert(1337);
  TEST_INVOKE_ASSERT_TRUE(tree.search, 1337);

  auto pred = tree.predecessor(tree.search(2));
  TEST_ASSERT_EQ(pred->value, 1);

  pred = tree.predecessor(tree.search(1));
  TEST_ASSERT_EQ(pred, nullptr);

  return true;
}

bool sdizo::tests::test_bst2()
{
  sdizo::Tree tree;
  tree.insert(10);
  tree.insert(7);
  tree.insert(13);
  tree.insert(6);
  tree.insert(1);
  tree.insert(15);
  tree.insert(14);
  tree.dsw();
  TEST_INVOKE_ASSERT_TRUE(tree.verify_connections);

  return true;
}

bool sdizo::tests::test_rbt()
{
  sdizo::RedBlackTree rbt;
  for(int i = 0; i < 10; ++i)
  {
    rbt.insert(i);
  }
  TEST_INVOKE_ASSERT_TRUE(rbt.verify_values);
  TEST_INVOKE_ASSERT_TRUE(rbt.verify_connections);
  for(int i = 0; i < 10; ++i)
  {
    rbt.remove(i);
  }
  TEST_INVOKE_ASSERT_TRUE(rbt.verify_values);
  TEST_INVOKE_ASSERT_TRUE(rbt.verify_connections);
  for(int i = 0; i < 10; ++i)
  {
    rbt.insert(i);
  }
  TEST_INVOKE_ASSERT_TRUE(rbt.verify_values);
  TEST_INVOKE_ASSERT_TRUE(rbt.verify_connections);
  for(int i = 0; i < 10; ++i)
  {
    rbt.remove(i);
  }
  rbt.remove(123);
  rbt.remove(123);
  rbt.remove(123);
  rbt.remove(123);
  rbt.insert(123123);
  TEST_INVOKE_ASSERT_TRUE(rbt.verify_values);
  TEST_INVOKE_ASSERT_TRUE(rbt.verify_connections);
  return true;
}

bool sdizo::tests::run_array_tests()
{
  if(!test_array())
    return false;

  if(!test_array2())
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

  if(!test_bst2())
    return false;

  return true;
}

bool sdizo::tests::run_rbt_tests()
{
  if(!test_rbt())
    return false;

  return true;
}
