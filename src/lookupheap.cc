#include "dijkstra.hpp"
#include "treeprinter.hpp"
#include "common.hpp"
#include <stdexcept>
#include <string.h>
#include <algorithm>
#include <random>
#include <cassert>
#include <fstream>
#include <fmt/format.h>

#ifndef HEAP_MACROS
#define HEAP_MACROS
#define PARENT(i) ((i - 1)>>1)
#define LEFT(i) ((i<<1)+1)
#define RIGHT(i) ((i+1)<<1)
#endif

sdizo2::dijkstra::LookupHeap::LookupHeap(int32_t size) noexcept
:array{new sdizo2::dijkstra::LookupHeap::ElemType[size]},
 lookup_table{new int32_t[size]},
 ssize{0},
 length{sdizo2::dijkstra::LookupHeap::expand_size}
{}

sdizo2::dijkstra::LookupHeap::LookupHeap(LookupHeap&& h) noexcept
:array(h.array), lookup_table(h.lookup_table), ssize(h.ssize), length(h.length)
{
  h.array = nullptr;
  h.lookup_table = nullptr;
  h.ssize = 0;
  h.length = 0;
}

sdizo2::dijkstra::LookupHeap::~LookupHeap() noexcept
{
  delete [] this->array;
  delete [] this->lookup_table;
}

sdizo2::dijkstra::LookupHeap::ElemType sdizo2::dijkstra::LookupHeap::at(int32_t index) const
{
  if(index >= this->ssize)
    throw std::out_of_range("Cannot read from index exceeding span of heap.");

  return this->array[index];
}

void sdizo2::dijkstra::LookupHeap::insert(ElemType element)
{
  if(this->ssize == this->length)
    this->expand();

  auto i = this->ssize;
  auto parent = PARENT(i);

  while(i > 0)
  {
    bool heap_property_satisfied;

    heap_property_satisfied = sdizo::key(this->array[parent]) <
                              sdizo::key(element);

    if (heap_property_satisfied)
      break;

    this->array[i] = this->array[parent];
    i = parent;
    parent = PARENT(i);
  }

  this->array[i] = element;
  ++this->ssize;
}

void sdizo2::dijkstra::LookupHeap::removeAt(int32_t index)
{
  if(this->ssize <= 0)
    return;

  if(index >= this->ssize)
    throw std::out_of_range("Cannot remove past last element.");

  std::swap(this->array[index], this->array[this->ssize-1]);
  --this->ssize;

  this->heapify(index);
}

void sdizo2::dijkstra::LookupHeap::remove(ElemType element) noexcept
{
  if(this->ssize <= 0)
    return;

  auto index = this->find(element);

  if(index == -1)
    return;

  std::swap(this->array[index], this->array[this->ssize-1]);
  --this->ssize;

  this->heapify(index);

  // Heapify up the tree
  // TODO make it a procedure
  auto parent = PARENT(index);

  bool heap_property_satisfied;

  while(parent > 0)
  {
    heap_property_satisfied = sdizo::key(this->array[index]) >
                             sdizo::key(this->array[parent]);

    if (heap_property_satisfied)
      break;

    std::swap(this->array[parent], this->array[index]);
    index = parent;
    parent = PARENT(parent);
  }
}

void sdizo2::dijkstra::LookupHeap::clear() noexcept
{
  this->ssize = 0;
}

bool sdizo2::dijkstra::LookupHeap::contains(ElemType element)
const noexcept
{
  for(int32_t i = 0; i < this->ssize; ++i)
  {
    if(this->array[i] == element)
      return true;
  }

  return false;
}

void sdizo2::dijkstra::LookupHeap::display() const noexcept
{
  puts("Min heap");

  printf("{ ");
  for(int32_t i = 0; i < this->ssize; ++i)
  {
    printf("%i ", sdizo::key(this->array[i]));
  }
  printf("} SIZE: %i\n", this->ssize);

  puts("===========================");
  print2D(this);
  puts("===========================");
}

void sdizo2::dijkstra::LookupHeap::heapify(int32_t index) noexcept
{
  this->heapify_down(index);
  this->heapify_up(index);
}

void sdizo2::dijkstra::LookupHeap::heapify_down(int32_t index) noexcept
{
  if(index >= this->ssize/2)
    return;

  auto left = LEFT(index);
  auto right = RIGHT(index);

  // Largest of all if heap is max
  // Smallest of all is heap is min
  auto extreme = index;

  if(left < this->ssize &&
     sdizo::key(this->array[left]) < sdizo::key(this->array[index]))
    extreme = left;

  if(right < this->ssize &&
     sdizo::key(this->array[right]) < sdizo::key(this->array[extreme]))
    extreme = right;

  if(extreme != index)
  {
    std::swap(this->array[extreme], this->array[index]);
    this->heapify_down(extreme);
  }

}

void sdizo2::dijkstra::LookupHeap::heapify_up(int32_t index) noexcept
{
  auto parent = PARENT(index);

  bool heap_property_satisfied;

  while(parent > 0)
  {
    heap_property_satisfied = sdizo::key(this->array[index]) >
                             sdizo::key(this->array[parent]);

    if (heap_property_satisfied)
      break;

    std::swap(this->array[parent], this->array[index]);
    index = parent;
    parent = PARENT(parent);
  }
}

void sdizo2::dijkstra::LookupHeap::expand() noexcept
{
  int32_t new_size = this->length + sdizo2::dijkstra::LookupHeap::expand_size;
  ElemType *new_array = new ElemType[new_size];

  // copy old part of heap
  std::copy(this->array, this->array + this->length, new_array);

  delete [] this->array;
  this->array = new_array;

  this->length = new_size;
}

int32_t sdizo2::dijkstra::LookupHeap::find(ElemType elem) const noexcept
{
  for(int32_t i = 0; i < this->ssize; ++i)
  {
    if(this->array[i] == elem)
      return i;
  }

  return -1;
}

bool sdizo2::dijkstra::LookupHeap::verify() const noexcept
{
  for(int32_t i = 0; i < this->ssize/2; ++i)
  {
    auto left = LEFT(i);
    auto right = RIGHT(i);

    if(left < this->ssize && sdizo::key(this->array[i]) >
                             sdizo::key(this->array[left]))
      return false;

    if(right < this->ssize && sdizo::key(this->array[i]) >
                              sdizo::key(this->array[right]))
      return false;
  }

  return true;
}

#undef PARENT
#undef LEFT
#undef RIGHT
