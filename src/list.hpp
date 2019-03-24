#pragma once
#include <cstdint>
#include <stdexcept>

namespace sdizo{
struct ListNode
{
  ListNode *prev;
  ListNode *next;
  int32_t value;

  ListNode() = default;
  explicit inline ListNode(int32_t val)
  :value(val){}
};

class List
{
  private:
    ListNode *begin;
    // Making appending easier.
    ListNode *end;
    
  public:
    List() noexcept;
    ~List() noexcept;

    int32_t loadFromFile() noexcept;

    // Inserts element at location given by index.
    // Throws std::out_of_range if index exceeds insert span of container,
    void insert(int32_t element, int32_t index);

    // Throws std::out_of_range if index exceeds span of container.
    // Throws std::length_error if container is already empty.
    void removeAt(int32_t index);

    // Finds and removes element from list that is equal to given element.
    // If element is not present, nothing is done.
    void remove(int32_t element);

    // Overwrites value at index with given element.
    void add(int32_t element, int32_t index);
  
    // Searches for element in container.
    // Returns true if element is in container.
    // False otherwise.
    bool contains(int32_t element) const noexcept;

    // Randomly generates and appends items to container.
    void generate(int32_t rand_range_begin, int32_t rand_range_end,
                  int32_t size) noexcept;

    void display() const noexcept;

    inline bool isEmpty() const noexcept
    {return this->begin == nullptr;}

  private:
    // Finds element in container.
    // If element is in the container, returns pointer to it,
    // nullptr otherwise.
    ListNode* find(int32_t value) const noexcept;

    // INTERNAL USE ONLY -- DEPENDS ON FACT THAT ARRAY IS NOT EMPTY
    // Returns pointer to node under given index.
    // If index points to place for new element nullptr is returned.
    // If index exceeds span of the container std::out_of_range is thrown.
    // CAUTION: This function cannot be run on empty list!
    ListNode* at(int32_t index) const;

    // Performs actual inserting without searching.
    void insert_(ListNode *node, ListNode *where);
  
    // Appends element at the end of container.
    void append(ListNode *node) noexcept;

    // Unlinks (removes) element from container.
    void unlink(ListNode *node) noexcept;
};
} // namespace sdizo
