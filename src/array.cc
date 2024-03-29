#include "array.hpp"
#include <algorithm>
#include <random>
#include <fstream>

int32_t sdizo::Array::loadFromFile(const char *filename) noexcept
{
  std::ifstream file(filename);
  int32_t num;
  int32_t count;

  file >> count;

  while(file >> num && count)
  {
    this->append(num);
    --count;
  }

  return 0;
}

int32_t sdizo::Array::at(int32_t index) const
{
  if(index >= this->ssize || index < 0)
    throw std::out_of_range("Referenced element out of range.");

  return this->data[index];
}

void sdizo::Array::insert(int32_t element, int32_t index)
{
  if(!this->isIndexInInsertRange(index))
    throw std::out_of_range("Tried inserting element out of array's span.");

  int32_t new_size = this->ssize+1;
  int32_t *new_data = new int[new_size];

  // Copy left side of array.
  std::copy(this->data, this->data+index, new_data);

  // Insert element at index.
  new_data[index] = element;

  // Copy right side of array.
  std::copy(this->data+index, this->data+this->ssize, new_data+index+1);

  delete [] this->data;
  this->data = new_data;
  this->ssize = new_size;

  #ifdef DEBUG_PRINT_ON
  this->display();
  #endif
}

void sdizo::Array::removeAt(int32_t index)
{
  if(this->ssize == 0)
  {
    #ifdef SOFT_EXCEPTIONS
    return;
    #else
    throw std::length_error("Cannot remove from empty array.");
    #endif
  }

  if(!this->isIndexInRange(index))
    throw std::out_of_range("Tried removing at index that is out of array's span.");

  int32_t new_size = this->ssize-1;
  int32_t *new_data = new int[new_size];

  // Copy left side of array.
  std::copy(this->data, this->data+index, new_data);

  // Copy right side of array.
  std::copy(this->data+index+1, this->data+this->ssize, new_data+index);

  delete [] this->data;
  this->data = new_data;
  this->ssize = new_size;

  #ifdef DEBUG_PRINT_ON
  this->display();
  #endif
}

void sdizo::Array::remove(int32_t element)
{
  int32_t index = this->find(element);

  if(index == -1)
    return;

  this->removeAt(index);
}

void sdizo::Array::clear() noexcept
{
  if(this->ssize > 0)
  {
    delete [] this->data;
    this->data = nullptr;
    this->ssize = 0;
  }
}

void sdizo::Array::update(int32_t index, int32_t element)
{
  if(!this->isIndexInRange(index))
    throw("Tried updating element at index out of array's span.");

  this->data[index] = element;

  #ifdef DEBUG_PRINT_ON
  this->display();
  #endif
}

bool sdizo::Array::contains(int32_t elem) const noexcept
{
  if(this->find(elem) == -1)
    return false;
  return true;
}

int32_t sdizo::Array::find(int32_t elem) const noexcept
{
  for(int32_t i = 0; i < this->ssize; ++i)
  {
    if(this->data[i] == elem)
    {
      #ifdef DEBUG_PRINT_ON
      printf("Found element %i at %i\n", elem, i);
      #endif
      return i;
    }
  }

  #ifdef DEBUG_PRINT_ON
  printf("Could not find element %i\n", elem);
  #endif

  return -1;
}

void sdizo::Array::generate
(int32_t rand_range_begin, int32_t rand_range_end, int32_t size) noexcept
{
  std::random_device generator;
  std::uniform_int_distribution<int32_t>
   distribution(rand_range_begin, rand_range_end);

  this->clear();
  for(int32_t i = 0; i < size; ++i)
  {
    this->insert(distribution(generator), 0);
  }
}

void sdizo::Array::display() const noexcept
{
  printf("{ ");
  for(int32_t i = 0; i < this->ssize; ++i)
  {
    printf("%i ", this->data[i]);
  }
  printf("}\n");
}

