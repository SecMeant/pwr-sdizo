#include "list.hpp"
#include <cstdio>
#include <cassert>
#include <random>
#include <fstream>

template<typename NodeType>
sdizo::List<NodeType>::List() noexcept
:begin{nullptr}, end(nullptr)
{}

template<typename NodeType>
sdizo::List<NodeType>::~List() noexcept
{
  while(this->begin != nullptr)
  {
    NodeType *next = this->begin->next;
    delete this->begin;
    this->begin = next;
  }
}

template<typename NodeType>
int32_t sdizo::List<NodeType>::loadFromFile(const char *filename) noexcept
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

template<typename NodeType>
void sdizo::List<NodeType>::insert(int32_t element, int32_t index)
{
  NodeType *insert_node = this->at(index);
  NodeType *new_node = new NodeType;
  new_node->value = element;

  if(insert_node == nullptr)
    this->append(new_node);
  else
    this->insert_(new_node, insert_node);

  #ifdef DEBUG_PRINT_ON
  this->display();
  #endif
}

template<typename NodeType>
void sdizo::List<NodeType>::removeAt(int32_t index)
{
  NodeType *to_delete = this->at(index);

  if(to_delete == nullptr)
    throw std::out_of_range("Tried to delete element of out range.");

  this->unlink(to_delete);

  #ifdef DEBUG_PRINT_ON
  this->display();
  #endif
}

template<typename NodeType>
void sdizo::List<NodeType>::remove(int32_t element)
{
  NodeType *to_delete = this->find(element);

  if(to_delete == nullptr)
    return;

  this->unlink(to_delete);

  #ifdef DEBUG_PRINT_ON
  this->display();
  #endif
}

template<typename NodeType>
void sdizo::List<NodeType>::clear() noexcept
{
  if(this->isEmpty())
    return;

  auto current = this->begin;
  while(current != nullptr)
  {
    auto next = current->next;
    delete current;
    current = next;
  }

  this->begin = nullptr;
  this->end = nullptr;
}

template<typename NodeType>
void sdizo::List<NodeType>::add(int32_t element, int32_t index)
{
  if(this->isEmpty())
    throw std::out_of_range("Treid to update (add) element at index out of range.");

  NodeType *to_update = this->at(index);

  if(to_update == nullptr)
    throw std::out_of_range("Treid to update (add) element at index out of range.");

  to_update->value = element;

  #ifdef DEBUG_PRINT_ON
  this->display();
  #endif
}

template<typename NodeType>
bool sdizo::List<NodeType>::contains(int32_t element) const noexcept
{
  return static_cast<bool>(this->find(element));
}

template<typename NodeType>
void sdizo::List<NodeType>::generate
(int32_t rand_range_begin, int32_t rand_range_end, int32_t size) noexcept
{
  std::random_device generator;
  std::uniform_int_distribution<int32_t>
   distribution(rand_range_begin, rand_range_end);

  this->clear();
  for(int32_t i = 0; i < size; ++i)
  {
    this->append(new NodeType(distribution(generator)));
  }

  #ifdef DEBUG_PRINT_ON
  this->display();
  #endif
}
template<typename NodeType>
void sdizo::List<NodeType>::display() const noexcept
{
  if(this->isEmpty())
  {
    puts("List is empty.");
    return;
  }

  NodeType *current = this->begin;
  while(1)
  {
    printf("%i -> ", current->value);
    if(current->next == nullptr)
      break;
    current = current->next;
  }
  
  puts("\nReversed:");
  while(1)
  {
    printf("%i <- ", current->value);
    if(current->prev == nullptr)
      break;
    current = current->prev;
  }
  puts("");
}

template<typename NodeType>
NodeType *sdizo::List<NodeType>::find(int32_t elem) const noexcept
{
  NodeType *current = this->begin;
  while(current != nullptr && current->value != elem)
  {
    current = current->next;
  }

  return current;
}

template<typename NodeType>
NodeType* sdizo::List<NodeType>::at(int32_t index) const
{
  if(index < 0)
    throw std::out_of_range("List index out of range. (Negative)");

  NodeType *current = this->begin;
  for(int i = 0; i < index; ++i)
  {
    if(current == nullptr)
      throw std::out_of_range("List index out of range.");

    current = current->next;
  }

  return current;
}

template<typename NodeType>
void sdizo::List<NodeType>::insert_(NodeType *node, NodeType *where)
{
  assert(node);

  if(where == nullptr)
  {
    this->append(node);
    return;
  }

  node->next = where;
  node->prev = where->prev;

  if(node->prev != nullptr)
    node->prev->next = node;
  // If there is no previous node, it means its begin
  else
    this->begin = node;

  if(where != nullptr)
    where->prev = node;
}

template<typename NodeType>
void sdizo::List<NodeType>::append(NodeType *node) noexcept
{
  if(this->isEmpty())
  {
    this->begin = node;
    this->begin->next = nullptr;
    this->begin->prev = nullptr;
    this->end = this->begin;
  }
  else
  {
    this->end->next = node;
    node->prev = this->end;
    node->next = nullptr;
    this->end = node;
  }
}

template<typename NodeType>
void sdizo::List<NodeType>::unlink(NodeType *to_unlink) noexcept
{
  if(to_unlink->prev != nullptr)
    to_unlink->prev->next = to_unlink->next;
  else
    this->begin = to_unlink->next;

  if(to_unlink->next != nullptr)
    to_unlink->next->prev = to_unlink->prev;
  else
    this->end = to_unlink->prev;

  delete to_unlink;
}

