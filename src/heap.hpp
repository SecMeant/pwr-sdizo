#pragma once
#include <cstdint>

namespace sdizo{

#define PARENT(i) ((i - 1)>>1)
#define LEFT(i) ((i<<1)+1)
#define RIGHT(i) ((i+1)<<1)

class Heap
{
	public:
		// Each time heap expands in memory (need to be relocated)
		// it allocates memory of size given in expand_size * sizeof(remove_ptr(array_type)).
		constexpr static std::size_t expand_size = 4;
	private:
		int32_t *array;
		// TODO(holz) change size name to ssize.
		int32_t size;
		int32_t length;

	public:
		Heap() noexcept;
		~Heap() noexcept;

    int32_t loadFromFile() noexcept;

    // Inserts element at location given by index.
    // Throws std::out_of_range if index exceeds insert span of array,
    void insert(int32_t element);

    // Throws std::out_of_range if index exceeds span of array.
    // Throws std::length_error if array is already empty.
    void removeAt(int32_t index);

    // Throws std::out_of_range if index exceeds span of array.
    // Throws std::length_error if array is already empty.
    void remove(int32_t element);

    // Overwrites value at index with given element.
    void add(int32_t element, int32_t index);
  
    // Searches for element in container.
    // Returns true if element is in container.
    // False otherwise.
    bool contains(int32_t element) const noexcept;

    // Randomly generates table.
    void generate(int32_t rand_range_begin, int32_t rand_range_end,
                  int32_t size) noexcept;

    void display() const noexcept;
	
	private:
		void heapify(int32_t index) noexcept;

		// Reallocs heap to new, bigger space.
		// New space is bigger by Heap::expand_size.
		void expand() noexcept;
};
} // namespace sdizo
