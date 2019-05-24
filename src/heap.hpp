#pragma once
#include <cstdint>
#include <cstddef>

namespace sdizo{

#define PARENT(i) ((i - 1)>>1)
#define LEFT(i) ((i<<1)+1)
#define RIGHT(i) ((i+1)<<1)

using heap_key_t = int32_t;

template<typename ElemType>
heap_key_t key(ElemType element)
{return element.getKey();}

template<>
heap_key_t key(int32_t element);

enum class HeapType
{min, max};

template<typename ElemType, HeapType heap_t = HeapType::max>
class Heap
{
  public:
    // Each time heap expands in memory (need to be relocated)
    // it allocates memory of size given in expand_size * sizeof(remove_ptr(array_type)).
    constexpr static size_t expand_size = 4;
    constexpr static HeapType heap_type = heap_t;
  private:
    ElemType *array;
    int32_t ssize;
    int32_t length;

  public:
    Heap() noexcept;
    ~Heap() noexcept;

    int32_t loadFromFile(const char *filename) noexcept;

    // Returns element from given index.
    // Throws std::out_of_range if index exceeds read span of array.
    ElemType at(int32_t index) const;

    // Inserts element at location given by index.
    // Throws std::out_of_range if index exceeds insert span of array,
    void insert(ElemType element);

    // Throws std::out_of_range if index exceeds span of array.
    void removeAt(int32_t index);

    inline void pop()
    {this->removeAt(0);}

    void remove(ElemType element) noexcept;

    // Removes all elements
    void clear() noexcept;

    // Searches for element in container.
    // Returns true if element is in container.
    // False otherwise.
    bool contains(ElemType element) const noexcept;

    // Randomly generates table.
    void generate(int32_t rand_range_begin, int32_t rand_range_end,
                  int32_t size) noexcept;

    void display() const noexcept;

    // Checks if container is proper heap,
    bool verify() const noexcept;

    inline decltype(auto) get_ssize() const noexcept
    {return this->ssize;}

  private:
    void heapify(int32_t index) noexcept;

    // Reallocs heap to new, bigger space.
    // New space is bigger by Heap::expand_size.
    void expand() noexcept;

    // Finds element in table.
    // If element is in the table, returns it's index, -1 otherwise.
    int32_t find(ElemType elem) const noexcept;
};
} // namespace sdizo

#include "heap.tcc"
