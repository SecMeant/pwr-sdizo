#include "costsourcetable.hpp"

sdizo2::CostSourceTable::CostSourceTable(int32_t size) noexcept
:cost_table(new int32_t[size]), source_table(new int32_t[size]), size(size)
{
  this->reset();
}

sdizo2::CostSourceTable::CostSourceTable
(sdizo2::CostSourceTable&& cst) noexcept
{
  this->cost_table = cst.cost_table;
  this->source_table = cst.source_table;
  this->size = cst.size;

  cst.cost_table = nullptr;
  cst.source_table = nullptr;
  cst.size = 0;
}

sdizo2::CostSourceTable::~CostSourceTable() noexcept
{
  delete [] this->cost_table;
  delete [] this->source_table;
}

void sdizo2::CostSourceTable::reset() noexcept
{
  if(this->size <= 0)
    return;

  this->cost_table[0] = 0;
  std::fill(this->cost_table+1, this->cost_table+this->size,
            sdizo2::CostSourceTable::INF);
  std::fill(this->source_table, this->source_table+this->size,
            sdizo2::CostSourceTable::INVALID_SOURCE);
}

void sdizo2::CostSourceTable::set_if_smaller (sdizo2::Edge edge)
{
  if(this->cost_table[edge.v2] <= edge.weight)
    return;

  this->cost_table[edge.v2] = edge.weight;
  this->source_table[edge.v2] = edge.v1;
}

void sdizo2::CostSourceTable::set(sdizo2::Edge edge)
{
  this->cost_table[edge.v2] = edge.weight;
  this->source_table[edge.v2] = edge.v1;
}

int32_t sdizo2::CostSourceTable::get_cost(int32_t index)
{
  if(index >= this->size || index < 0)
    throw std::out_of_range("Tried getting cost if node that is out of rrange.");

  return this->cost_table[index];
}

int32_t sdizo2::CostSourceTable::get_source(int32_t index)
{
  if(index >= this->size || index < 0)
    throw std::out_of_range("Tried getting cost if node that is out of rrange.");

  return this->source_table[index];
}

void sdizo2::CostSourceTable::resize(int32_t newsize) noexcept
{
  delete [] this->cost_table;
  delete [] this->source_table;

  this->cost_table = new int32_t[newsize];
  this->source_table = new int32_t[newsize];

  this->size = newsize;
}

void sdizo2::CostSourceTable::display()
noexcept
{
  constexpr int32_t alignment = 15;

  fmt::print("{:>{}} ", "Nodes ->", alignment);
  for(auto i = 0; i < this->size; ++i)
    fmt::print("{:<10} ", i);

  putchar('\n');

  fmt::print("{:>{}} ", "Cost ->", alignment);
  for(auto i = 0; i < this->size; ++i)
    if(this->cost_table[i] == INF)
      fmt::print("{:<10} ", "INF");
    else
      fmt::print("{:<10} ", this->cost_table[i]);

  putchar('\n');

  fmt::print("{:>{}} ", "Destination ->", alignment);
  for(auto i = 0; i < this->size; ++i)
    fmt::print("{:<10} ", this->source_table[i]);

  putchar('\n');


  // Print path for each node from starting node
  for(auto i = 0; i < this->size; ++i)
  {
    auto current_node = this->source_table[i];
    fmt::print("{}", i);

    while(current_node != -1)
    {
      fmt::print(" <- {}", current_node);
      current_node = this->source_table[current_node];
    }

    putchar('\n');
  }
}

