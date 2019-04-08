#include "list.hpp"
#include <cstdio>
#include <cassert>
#include <random>
#include <fstream>

sdizo::List::List() noexcept
:begin{nullptr}, end(nullptr)
{}

sdizo::List::~List() noexcept
{
  while(this->begin != nullptr)
  {
    ListNode *next = this->begin->next;
    delete this->begin;
    this->begin = next;
  }
}

int32_t sdizo::List::loadFromFile(const char *filename) noexcept
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

void sdizo::List::insert(int32_t element, int32_t index)
{
  ListNode *insert_node = this->at(index);
  ListNode *new_node = new ListNode;
  new_node->value = element;

  if(insert_node == nullptr)
    this->append(new_node);
  else
    this->insert_(new_node, insert_node);

  #ifdef DEBUG_PRINT_ON
  this->display();
  #endif
}

void sdizo::List::removeAt(int32_t index)
{
  ListNode *to_delete = this->at(index);

  if(to_delete == nullptr)
    throw std::out_of_range("Tried to delete element of out range.");

  this->unlink(to_delete);

  #ifdef DEBUG_PRINT_ON
  this->display();
  #endif
}

void sdizo::List::remove(int32_t element)
{
  ListNode *to_delete = this->find(element);

  if(to_delete == nullptr)
    return;

  this->unlink(to_delete);

  #ifdef DEBUG_PRINT_ON
  this->display();
  #endif
}

void sdizo::List::add(int32_t element, int32_t index)
{
  if(this->isEmpty())
    throw std::out_of_range("Treid to update (add) element at index out of range.");

  ListNode *to_update = this->at(index);

  if(to_update == nullptr)
    throw std::out_of_range("Treid to update (add) element at index out of range.");

  to_update->value = element;

  #ifdef DEBUG_PRINT_ON
  this->display();
  #endif
}

bool sdizo::List::contains(int32_t element) const noexcept
{
  return static_cast<bool>(this->find(element));
}

void sdizo::List::generate
(int32_t rand_range_begin, int32_t rand_range_end, int32_t size) noexcept
{
  std::random_device generator;
  std::uniform_int_distribution<int32_t>
   distribution(rand_range_begin, rand_range_end);

  for(int32_t i = 0; i < size; ++i)
  {
    this->append(new ListNode(distribution(generator)));
  }

  #ifdef DEBUG_PRINT_ON
  this->display();
  #endif
}

void sdizo::List::display() const noexcept
{
  if(this->isEmpty())
  {
    puts("List is empty.");
    return;
  }

  ListNode *current = this->begin;
  while(current != nullptr)
  {
    printf("%i ", current->value);
    current = current->next;
  }
  puts("");
}

sdizo::ListNode *sdizo::List::find(int32_t elem) const noexcept
{
  ListNode *current = this->begin;
  while(current != nullptr && current->value != elem)
  {
    current = current->next;
  }

  return current;
}

sdizo::ListNode* sdizo::List::at(int32_t index) const
{
  if(index < 0)
    throw std::out_of_range("List index out of range. (Negative)");

  ListNode *current = this->begin;
  for(int i = 0; i < index; ++i)
  {
    if(current == nullptr)
      throw std::out_of_range("List index out of range.");

    current = current->next;
  }

  return current;
}


void sdizo::List::insert_(ListNode *node, ListNode *where)
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

void sdizo::List::append(ListNode *node) noexcept
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

void sdizo::List::unlink(ListNode *to_unlink) noexcept
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

