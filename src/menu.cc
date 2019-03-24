#include <stdio.h>
#include "array.hpp"
#include "list.hpp"

void test_array()
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
}

void test_list()
{
  using sdizo::List;

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
  list.removeAt(4);
  list.remove(12312321);
  list.remove(12);
  list.remove(5);
  list.remove(23232);
  list.remove(23232);
  list.removeAt(0);
  list.removeAt(0);
  list.insert(1337, 0);

}

void test_list2()
{
  using sdizo::List;

  List list;
  list.generate(-15, 23, 10);
  puts(list.contains(1337) ? "true" : "false");
  list.add(1337, 0);
  puts(list.contains(1337) ? "true" : "false");
}

int main()
{
  test_list2();
  return 0;
}
