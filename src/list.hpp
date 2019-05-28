#pragma once
#include <cstdint>
#include <stdexcept>
#include <type_traits>
#include "common.hpp"

namespace sdizo{

template<typename ValueType>
struct ListNode
{
  typedef ValueType value_type;

  ListNode<ValueType> *prev;
  ListNode<ValueType> *next;
  ValueType value;

  ListNode() = default;
  explicit inline ListNode(ValueType val)
  :value(val){}
};

// TODO)holz) make enable_if T == ListNode<U>
template<typename NodeType>
class List
{
  public:
    typedef typename NodeType::value_type value_type;

  private:
    NodeType *begin;
    // Making appending easier.
    NodeType *end;

  public:
    List() noexcept;
    List(const List& l) = delete;
    ~List() noexcept;

    int32_t loadFromFile(const char *filename) noexcept;

    // Inserts element at location given by index.
    // Throws std::out_of_range if index exceeds insert span of container,
    void insert(value_type element, int32_t index);

    inline void prepend(value_type element) noexcept
    {this->insert(element, 0);}

    inline void append(value_type element) noexcept
    {this->append(new NodeType(element));}

    inline void append(NodeType node) noexcept
    {this->append(new NodeType(node));}

    // Throws std::out_of_range if index exceeds span of container.
    // Throws std::length_error if container is already empty.
    void removeAt(int32_t index);

    // Finds and removes element from list that is equal to given element.
    // If element is not present, nothing is done.
    void remove(value_type element);

    // Removes all elements.
    void clear() noexcept;

    // Overwrites value at index with given element.
    void add(value_type element, int32_t index);

    // Searches for element in container.
    // Returns true if element is in container.
    // False otherwise.
    bool contains(value_type element) const noexcept;

    // Randomly generates and appends items to container.
    void generate(int32_t rand_range_begin, int32_t rand_range_end,
                  int32_t size) noexcept;

    void display() const noexcept;

    inline bool isEmpty() const noexcept
    {return this->begin == nullptr;}

    inline NodeType* get_begin() noexcept
    {return this->begin;}

    inline const NodeType* get_cbegin() const noexcept
    {return this->begin;}

    inline NodeType* get_end() noexcept
    {return this->end;}

  private:
    // Finds element in container.
    // If element is in the container, returns pointer to it,
    // nullptr otherwise.
    NodeType* find(value_type value) const noexcept;

    // INTERNAL USE ONLY -- DEPENDS ON FACT THAT ARRAY IS NOT EMPTY
    // Returns pointer to node under given index.
    // If index points to place for new element nullptr is returned.
    // If index exceeds span of the container std::out_of_range is thrown.
    // CAUTION: This function cannot be run on empty list!
    NodeType* at(int32_t index) const;

    // Performs actual inserting without searching.
    void insert_(NodeType *node, NodeType *where);

    // Appends element at the end of container.
    void append(NodeType *node) noexcept;

    // Unlinks (removes) element from container.
    void unlink(NodeType *node) noexcept;
};

}; // namespace sdizo

#include "list.tcc"
