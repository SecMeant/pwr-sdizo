#include <exception>
#include <fmt/ostream.h>
#include "dijkstra.hpp"
#include "fmt_custom.hpp"

sdizo2::dijkstra::CostSourceTable::CostSourceTable(int32_t size) noexcept
:cost_table(new int32_t[size]), source_table(new int32_t[size]), size(size)
{
  this->reset();
}

sdizo2::dijkstra::CostSourceTable::~CostSourceTable() noexcept
{
  delete [] this->cost_table;
  delete [] this->source_table;
}

void sdizo2::dijkstra::CostSourceTable::reset() noexcept
{
  if(this->size <= 0)
    return;

  this->cost_table[0] = 0;
  std::fill(this->cost_table+1, this->cost_table+this->size,
            sdizo2::dijkstra::CostSourceTable::INF);
  std::fill(this->source_table, this->source_table+this->size,
            sdizo2::dijkstra::CostSourceTable::INVALID_SOURCE);
}

void sdizo2::dijkstra::CostSourceTable::set_if(sdizo2::Edge edge)
{
  if(this->cost_table[edge.v2] <= edge.weight)
    return;

  this->cost_table[edge.v2] = edge.weight;
  this->source_table[edge.v2] = edge.v1;
}

void sdizo2::dijkstra::CostSourceTable::set(sdizo2::Edge edge)
{
  this->cost_table[edge.v2] = edge.weight;
  this->source_table[edge.v2] = edge.v1;
}

void sdizo2::dijkstra::CostSourceTable::resize(int32_t newsize) noexcept
{
  delete [] this->cost_table;
  delete [] this->source_table;

  this->cost_table = new int32_t[newsize];
  this->source_table = new int32_t[newsize];

  this->size = newsize;
}

void sdizo2::dijkstra::CostSourceTable::display() noexcept
{
  constexpr int32_t alignment = 15;

  fmt::print("{:>{}} ", "Nodes ->", alignment);
  for(auto i = 0; i < this->size; ++i)
    fmt::print("{:<10} ", i);

  putchar('\n');

  fmt::print("{:>{}} ", "Cost ->", alignment);
  for(auto i = 0; i < this->size; ++i)
    fmt::print("{:<10} ", this->cost_table[i]);

  putchar('\n');

  fmt::print("{:>{}} ", "Destination ->", alignment);
  for(auto i = 0; i < this->size; ++i)
    fmt::print("{:<10} ", this->source_table[i]);

  putchar('\n');
}

sdizo2::dijkstra::DijkstraSolver::DijkstraSolver(int32_t size) noexcept
:cst(size), node_list(new sdizo::List<sdizo::ListNode<MSTListNode>>[size]),
 node_matrix(size), size(size) {}

sdizo2::dijkstra::DijkstraSolver::~DijkstraSolver() noexcept
{
  delete [] this->node_list;
}

void sdizo2::dijkstra::DijkstraSolver::resize(int32_t newsize) noexcept
{
  this->cst.resize(newsize);

  delete [] this->node_list;
  this->node_list =
    new sdizo::List<sdizo::ListNode<MSTListNode>>[newsize];

  this->node_matrix.resize(newsize);

  this->size = newsize;
}

void sdizo2::dijkstra::DijkstraSolver::display() noexcept
{
  this->cst.display();

  for(auto i = 0; i < this->size; ++i)
    fmt::print("[{}] {}\n", i, this->node_list[i]);
}
