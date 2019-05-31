#include "dijkstra.hpp"
#include "costsourcetable.hpp"
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

sdizo2::dijkstra::LookupHeap::LookupHeap
(int32_t size) noexcept
:array{new sdizo2::dijkstra::LookupHeap::ElemType[size]},
 lookup_table{new int32_t[size]},
 ssize{0},
 length{sdizo2::dijkstra::LookupHeap::expand_size}
{
  // prepare lookup table
  for(auto i = 0; i < this->ssize; ++i)
    this->lookup_table[i] = i;

  // prepare heap
  this->array[0] = {0,0};
  for(auto i = 1; i < this->ssize; ++i)
    this->array[i] = {i, sdizo2::CostSourceTable::INF};

  this->ssize = size;
  this->length = size;
}

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

void sdizo2::dijkstra::LookupHeap::removeAt(int32_t index)
{
  if(this->ssize <= 0)
    return;

  if(index >= this->ssize)
    throw std::out_of_range("Cannot remove past last element.");

  // fix lookup table
  // find nodes that will be swaped
  auto n1 = this->array[index].node;
  auto n2 = this->array[this->ssize-1].node;

  // Second will go in place of first
  this->lookup_table[n2] = this->lookup_table[n1];
  // First one is removed
  this->lookup_table[n1] = -1;

  std::swap(this->array[index], this->array[this->ssize-1]);

  --this->ssize;

  this->heapify(index);
}

void sdizo2::dijkstra::LookupHeap::clear() noexcept
{
  this->ssize = 0;
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
    // Fix lookup_table, swap positions
    auto n1 = this->array[extreme].node;
    auto n2 = this->array[index].node;
    std::swap(this->lookup_table[n1], this->lookup_table[n2]);

    // Swap inside heap
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

    // Fix lookup_table, swap positions
    auto n1 = this->array[parent].node;
    auto n2 = this->array[index].node;
    std::swap(this->lookup_table[n1], this->lookup_table[n2]);

    // Swap inside heap
    std::swap(this->array[parent], this->array[index]);

    index = parent;
    parent = PARENT(parent);
  }
}

void sdizo2::dijkstra::LookupHeap::insert(ElemType element) noexcept
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

    // Make place for current inserting element
    this->array[i] = this->array[parent];

    // Update position of moved node
    auto moved_node = this->array[parent].node;
    this->lookup_table[moved_node] = i;

    i = parent;
    parent = PARENT(i);
  }

  // Insert element
  this->array[i] = element;

  // Update inserted element position
  this->lookup_table[element.node] = i;

  ++this->ssize;
}

void sdizo2::dijkstra::LookupHeap::expand() noexcept
{
  // Realloc heap
  int32_t new_size = this->length + sdizo2::dijkstra::LookupHeap::expand_size;
  ElemType *new_array = new ElemType[new_size];

  // copy old part of heap
  std::copy(this->array, this->array + this->length, new_array);

  delete [] this->array;
  this->array = new_array;


  // Realloc lookup_table
  int32_t *new_lookup = new int32_t[new_size];

  // copy old part of heap
  std::copy(this->lookup_table, 
            this->lookup_table + this->length,
            new_lookup);

  delete [] this->lookup_table;
  this->lookup_table = new_lookup;

  this->length = new_size;
}

int32_t sdizo2::dijkstra::LookupHeap::find(ElemType elem) const 
{
  if(elem.node >= this->length|| elem.node < 0)
    throw std::out_of_range("Tried finding node out of range.");

  return this->lookup_table[elem.node];
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

void sdizo2::dijkstra::LookupHeap::reset(int32_t starting_node)
{
  if(starting_node >= this->ssize || starting_node < 0)
    throw std::out_of_range(fmt::format("Starting node out of range, "
      "expected in range [0,{}), got {}", this->ssize, starting_node));

  // prepare lookup table
  for(auto i = 0; i < this->ssize; ++i)
    this->lookup_table[i] = i;

  // prepare heap
  for(auto i = 0; i < this->ssize; ++i)
    this->array[i] = {i, sdizo2::CostSourceTable::INF};

  this->array[starting_node] = {starting_node,0};
}

void sdizo2::dijkstra::LookupHeap::update(int32_t node, int32_t cost)
{
  auto node_index = this->lookup_table[node];

  if(node >= this->length || node < 0)
    throw std::out_of_range(
      fmt::format("Tried updating node ({}) that is out of range in lookup,"
      " expected in range [0,{}), got {}", node, this->length, node_index));

  // if node no longer in heap, but updated it must be
  // reinserted into heap, all paths comming from it must
  // be analyzed once more
  if(node_index >= this->ssize || node_index < 0)
  {
    this->insert({node, cost});

    // Also update, node_index with new position of node
    node_index = this->lookup_table[node];
  }

  this->array[node_index].cost = cost;
  this->heapify(node_index);
}

#undef PARENT
#undef LEFT
#undef RIGHT
