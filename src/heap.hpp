#pragma once
#include <cstdint>
#include <cstddef>

namespace sdizo{

#ifndef HEAP_MACROS
#define HEAP_MACROS
#define PARENT(i) ((i - 1)>>1)
#define LEFT(i) ((i<<1)+1)
#define RIGHT(i) ((i+1)<<1)
#endif

enum class HeapType
{min, max};

template<typename ElemType, HeapType heap_t = HeapType::max>
class Heap
{
  private:
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
    Heap(Heap&& h) noexcept;
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

    inline ElemType pop()
    {ElemType ret = this->at(0); this->removeAt(0); return ret;}

    void remove(ElemType element) noexcept;

    void update(int32_t index, ElemType element) noexcept;

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

    inline bool is_empty() const noexcept
    {return this->ssize == 0;}

  private:
    void heapify(int32_t index) noexcept;
    void heapify_down(int32_t index) noexcept;
    void heapify_up(int32_t index) noexcept;

    // Reallocs heap to new, bigger space.
    // New space is bigger by Heap::expand_size.
    void expand() noexcept;

    // Finds element in table.
    // If element is in the table, returns it's index, -1 otherwise.
    int32_t find(ElemType elem) const noexcept;
};
} // namespace sdizo

#include "heap.tcc"
