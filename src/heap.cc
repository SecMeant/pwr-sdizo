#include "heap.hpp"
#include <stdexcept>
#include <string.h>
#include <algorithm>
#include <random>
#include <cassert>
#include <fstream>

sdizo::Heap::Heap() noexcept
:array{new int32_t[sdizo::Heap::expand_size]},
 ssize{0},
 length{sdizo::Heap::expand_size}
{}

sdizo::Heap::~Heap() noexcept
{
  delete [] this->array;
}

int32_t sdizo::Heap::loadFromFile(const char *filename) noexcept
{
  std::ifstream file(filename);
  int32_t num;

  while(file >> num)
    this->insert(num);

  return 0;
}

int32_t sdizo::Heap::at(int32_t index) const
{
  if(index >= this->ssize)
    throw std::out_of_range("Cannot read from index exceeding span of heap.");

  return this->array[index];
}

void sdizo::Heap::insert(int32_t element)
{
  if(this->ssize == this->length)
    this->expand();

  auto i = this->ssize;
  auto parent = PARENT(i);

  while(i > 0 && this->array[parent] < element)
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

void sdizo::Heap::removeAt(int32_t index)
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

void sdizo::Heap::remove(int32_t element) noexcept
{
  if(this->ssize <= 0)
    return;

  auto index = this->find(element);

  if(index == -1)
    return;

  std::swap(this->array[index], this->array[this->ssize-1]);
  --this->ssize;

  this->heapify(index);

  #ifdef DEBUG_PRINT_ON
  this->display();
  #endif
}

bool sdizo::Heap::contains(int32_t element) const noexcept
{
  for(int32_t i = 0; i < this->ssize; ++i)
  {
    if(this->array[i] == element)
      return true;
  }

  return false;
}

void sdizo::Heap::generate
(int32_t rand_range_begin, int32_t rand_range_end, int32_t size) noexcept
{
  std::random_device generator;
  std::uniform_int_distribution<int32_t>
   distribution(rand_range_begin, rand_range_end);

  for(int32_t i = 0; i < size; ++i)
  {
    this->insert(distribution(generator));
  }
}

void sdizo::Heap::display() const noexcept
{
  printf("{ ");
  for(int32_t i = 0; i < this->ssize; ++i)
  {
    printf("%i ", this->array[i]);
  }
  printf("} SIZE: %i\n", this->ssize);
}

void sdizo::Heap::heapify(int32_t index) noexcept
{
  assert(index < this->ssize);

  if(index >= this->ssize/2)
    return;

  auto left = LEFT(index);
  auto right = RIGHT(index);
  auto largest = index;

  if(left <= this->ssize && this->array[left] > this->array[index])
    largest = left;

  if(right <= this->ssize && this->array[right]  > this->array[largest])
    largest = right;

  if(largest != index)
  {
    std::swap(this->array[largest], this->array[index]);
    this->heapify(largest);
  }

}

void sdizo::Heap::expand() noexcept
{
  int32_t new_size = this->length + sdizo::Heap::expand_size;
  int32_t *new_array = new int32_t[new_size];

  // copy old part of heap
  std::copy(this->array, this->array + this->length, new_array);

  delete [] this->array;
  this->array = new_array;

  this->length = new_size;
}

int32_t sdizo::Heap::find(int32_t elem) const noexcept
{
  for(int32_t i = 0; i < this->ssize; ++i)
  {
    if(this->array[i] == elem)
      return i;
  }

  return -1;
}

bool sdizo::Heap::verify() const noexcept
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
