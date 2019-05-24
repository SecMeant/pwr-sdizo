#pragma once
#include "treeprinter.hpp"
#include "heap.hpp"
#include <stdexcept>
#include <string.h>
#include <algorithm>
#include <random>
#include <cassert>
#include <fstream>
#include <fmt/format.h>

template<typename ElemType, sdizo::HeapType heap_t>
sdizo::Heap<ElemType, heap_t>::Heap() noexcept
:array{new ElemType[sdizo::Heap<ElemType, heap_t>::expand_size]},
 ssize{0},
 length{sdizo::Heap<ElemType, heap_t>::expand_size}
{}

template<typename ElemType, sdizo::HeapType heap_t>
sdizo::Heap<ElemType, heap_t>::~Heap() noexcept
{
  delete [] this->array;
}

template<typename ElemType, sdizo::HeapType heap_t>
int32_t sdizo::Heap<ElemType, heap_t>::loadFromFile(const char *filename) noexcept
{
  std::ifstream file(filename);
  int32_t num;
  int32_t count;

  file >> count;

  while(file >> num && count)
  {
    this->insert(num);
    --count;
  }

  return 0;
}

template<typename ElemType, sdizo::HeapType heap_t>
ElemType sdizo::Heap<ElemType, heap_t>::at(int32_t index) const
{
  if(index >= this->ssize)
    throw std::out_of_range("Cannot read from index exceeding span of heap.");

  return this->array[index];
}

template<typename ElemType, sdizo::HeapType heap_t>
void sdizo::Heap<ElemType, heap_t>::insert(ElemType element)
{
  if(this->ssize == this->length)
    this->expand();

  auto i = this->ssize;
  auto parent = PARENT(i);

  while(i > 0 && sdizo::key(this->array[parent]) < sdizo::key(element))
  {
    this->array[i] = this->array[parent];
    i = parent;
    parent = PARENT(i);
  }

  this->array[i] = element;
  ++this->ssize;

  #ifdef DEBUG_PRINT_ON
  this->display();
  #endif
}

template<typename ElemType, sdizo::HeapType heap_t>
void sdizo::Heap<ElemType, heap_t>::removeAt(int32_t index)
{
  if(this->ssize <= 0)
    return;

  if(index >= this->ssize)
    throw std::out_of_range("Cannot remove past last element.");

  std::swap(this->array[index], this->array[this->ssize-1]);
  --this->ssize;

  this->heapify(index);

  #ifdef DEBUG_PRINT_ON
  this->display();
  #endif
}

template<typename ElemType, sdizo::HeapType heap_t>
void sdizo::Heap<ElemType, heap_t>::remove(ElemType element) noexcept
{
  if(this->ssize <= 0)
    return;

  auto index = this->find(element);

  if(index == -1)
    return;

  std::swap(this->array[index], this->array[this->ssize-1]);
  --this->ssize;

  this->heapify(index);
  auto parent = PARENT(index);
  fmt::print("Parent: {}, Index {}\n",parent, index);
  while(parent > 0 && this->array[index] > this->array[parent])
  {
    fmt::print("Parent: {}, Index {}\n",parent, index);
    std::swap(this->array[parent], this->array[index]);
    index = parent;
    parent = PARENT(parent);
  }

  #ifdef DEBUG_PRINT_ON
  this->display();
  #endif
}

template<typename ElemType, sdizo::HeapType heap_t>
void sdizo::Heap<ElemType, heap_t>::clear() noexcept
{
  this->ssize = 0;
}

template<typename ElemType, sdizo::HeapType heap_t>
bool sdizo::Heap<ElemType, heap_t>::contains(ElemType element) const noexcept
{
  for(int32_t i = 0; i < this->ssize; ++i)
  {
    if(this->array[i] == element)
      return true;
  }

  return false;
}

template<typename ElemType, sdizo::HeapType heap_t>
void sdizo::Heap<ElemType, heap_t>::generate
(int32_t rand_range_begin, int32_t rand_range_end, int32_t size) noexcept
{
  std::random_device generator;
  std::uniform_int_distribution<int32_t>
   distribution(rand_range_begin, rand_range_end);

  this->clear();
  for(int32_t i = 0; i < size; ++i)
  {
    this->insert(distribution(generator));
  }
}

template<typename ElemType, sdizo::HeapType heap_t>
void sdizo::Heap<ElemType, heap_t>::display() const noexcept
{
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

template<typename ElemType, sdizo::HeapType heap_t>
void sdizo::Heap<ElemType, heap_t>::heapify(int32_t index) noexcept
{
  if(index >= this->ssize/2)
    return;

  auto left = LEFT(index);
  auto right = RIGHT(index);
  auto largest = index;

  if(left < this->ssize && this->array[left] > this->array[index])
    largest = left;

  if(right < this->ssize && this->array[right]  > this->array[largest])
    largest = right;

  if(largest != index)
  {
    std::swap(this->array[largest], this->array[index]);
    this->heapify(largest);
  }

}

template<typename ElemType, sdizo::HeapType heap_t>
void sdizo::Heap<ElemType, heap_t>::expand() noexcept
{
  int32_t new_size = this->length + sdizo::Heap<ElemType, heap_t>::expand_size;
  ElemType *new_array = new ElemType[new_size];

  // copy old part of heap
  std::copy(this->array, this->array + this->length, new_array);

  delete [] this->array;
  this->array = new_array;

  this->length = new_size;
}

template<typename ElemType, sdizo::HeapType heap_t>
int32_t sdizo::Heap<ElemType, heap_t>::find(ElemType elem) const noexcept
{
  for(int32_t i = 0; i < this->ssize; ++i)
  {
    if(this->array[i] == elem)
      return i;
  }

  return -1;
}

template<typename ElemType, sdizo::HeapType heap_t>
bool sdizo::Heap<ElemType, heap_t>::verify() const noexcept
{
  for(int32_t i = 0; i < this->ssize/2; ++i)
  {
    #ifdef DEBUG_PRINT_ON
    printf("Veryfing index %i. ", i);
    #endif

    auto left = LEFT(i);
    if(left < this->ssize && this->array[i] < this->array[left])
      return false;

    auto right = RIGHT(i);
    if(right < this->ssize && this->array[i] < this->array[right])
      return false;

    #ifdef DEBUG_PRINT_ON
    puts("OK");
    #endif
  }

  return true;
}
