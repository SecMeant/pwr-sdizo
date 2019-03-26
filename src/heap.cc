#include "heap.hpp"
#include <stdexcept>
#include <string.h>
#include <algorithm>

#define DEBUG_PRINT_ON

sdizo::Heap::Heap() noexcept
:array{new int32_t[sdizo::Heap::expand_size]},
 size{0},
 length{sdizo::Heap::expand_size}
{}

sdizo::Heap::~Heap() noexcept
{
	delete [] this->array;
}

void sdizo::Heap::insert(int32_t element)
{
	if(this->size == this->length)
		this->expand();
	
	auto i = this->size;
	auto parent = PARENT(i);

	while(i > 0 && this->array[parent] < element)
	{
		this->array[i] = this->array[parent];
		i = parent;
		parent = PARENT(i);
	}

	this->array[i] = element;
	++this->size;

	#ifdef DEBUG_PRINT_ON
	this->display();
	#endif
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

void sdizo::Heap::display() const noexcept
{
  printf("{ ");
  for(int32_t i = 0; i < this->size; ++i)
  {
    printf("%i ", this->array[i]);
  }
  printf("}\n");
}
