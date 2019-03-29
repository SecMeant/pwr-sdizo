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
    int32_t ssize;
    int32_t length;

  public:
    Heap() noexcept;
    ~Heap() noexcept;

    int32_t loadFromFile(const char *filename) noexcept;

    // Returns element from given index.
    // Throws std::out_of_range if index exceeds read span of array.
    int32_t at(int32_t index) const;

    // Inserts element at location given by index.
    // Throws std::out_of_range if index exceeds insert span of array,
    void insert(int32_t element);

    // Throws std::out_of_range if index exceeds span of array.
    void removeAt(int32_t index);

    void remove(int32_t element) noexcept;

    // Searches for element in container.
    // Returns true if element is in container.
    // False otherwise.
    bool contains(int32_t element) const noexcept;

    // Randomly generates table.
    void generate(int32_t rand_range_begin, int32_t rand_range_end,
                  int32_t size) noexcept;

    void display() const noexcept;

    // Checks if container is proper heap,
    bool verify() const noexcept;
  
  private:
    void heapify(int32_t index) noexcept;

    // Reallocs heap to new, bigger space.
    // New space is bigger by Heap::expand_size.
    void expand() noexcept;

    // Finds element in table.
    // If element is in the table, returns it's index, -1 otherwise.
    int32_t find(int32_t elem) const noexcept;
};
} // namespace sdizo
