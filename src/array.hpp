#pragma once
#include <cstdint>
#include <stdexcept>

namespace sdizo{
class Array
{
  private:
    int32_t *data;
    int32_t ssize;

  public:
    Array() noexcept;

    ~Array() noexcept;

    int32_t loadFromFile() noexcept;

    // Inserts element at location given by index.
    // Throws std::out_of_range if index exceeds insert span of array,
    // TODO(holz) make it use malloc and realloc instead of new.
    void insert(int32_t element, int32_t index);

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
    // Checks if index is in range of array.
    inline bool isIndexInRange(int32_t index) const noexcept
    {
      if(index < 0)
        return false;

      if(index >= this->ssize)
        return false;

      return true;
    }

    // Checks if element could be inserted at given index.
    inline bool isIndexInInsertRange(int32_t index) const noexcept
    {
      if(index < 0)
        return false;

      if(index > this->ssize)
        return false;

      return true;
    }

    // Finds element in table.
    // If element is in the table, returns it's index, -1 otherwise.
    int32_t find(int32_t elem) const noexcept;
};
} // namespace sdizo
