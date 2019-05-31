#include <exception>
#include <fmt/ostream.h>
#include "dijkstra.hpp"
#include "fmt_custom.hpp"

sdizo2::dijkstra::CostSourceTable::CostSourceTable(int32_t size) noexcept
:cost_table(new int32_t[size]), source_table(new int32_t[size]), size(size)
{
  this->reset();
}

sdizo2::dijkstra::CostSourceTable::CostSourceTable
(sdizo2::dijkstra::CostSourceTable&& cst) noexcept
{
  this->cost_table = cst.cost_table;
  this->source_table = cst.source_table;
  this->size = cst.size;

  cst.cost_table = nullptr;
  cst.source_table = nullptr;
  cst.size = 0;
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

void sdizo2::dijkstra::CostSourceTable::set_if_smaller (sdizo2::Edge edge)
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

int32_t sdizo2::dijkstra::CostSourceTable::get_cost(int32_t index)
{
  if(index >= this->size || index < 0)
    throw std::out_of_range("Tried getting cost if node that is out of rrange.");

  return this->cost_table[index];
}

int32_t sdizo2::dijkstra::CostSourceTable::get_source(int32_t index)
{
  if(index >= this->size || index < 0)
    throw std::out_of_range("Tried getting cost if node that is out of rrange.");

  return this->source_table[index];
}

void sdizo2::dijkstra::CostSourceTable::resize(int32_t newsize) noexcept
{
  delete [] this->cost_table;
  delete [] this->source_table;

  this->cost_table = new int32_t[newsize];
  this->source_table = new int32_t[newsize];

  this->size = newsize;
}

void sdizo2::dijkstra::CostSourceTable::display()
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

sdizo2::dijkstra::DijkstraSolver::DijkstraSolver(int32_t size) noexcept
:cst(size),
 edge_list(new sdizo::List<sdizo::ListNode<MSTListNode>>[size]),
 node_matrix(size), size(size), starting_node(0) {}

sdizo2::dijkstra::DijkstraSolver::~DijkstraSolver() noexcept
{
  delete [] this->edge_list;
}

void sdizo2::dijkstra::DijkstraSolver::loadFromFile
(const char *filename)
{
  std::ifstream file(filename);

  if(!file.is_open())
    throw std::runtime_error(fmt::format("Cant open file {}", filename));

  int32_t edge_cnt;
  int32_t node_cnt;
  int32_t starting_node;

  file >> edge_cnt;
  file >> node_cnt;
  file >> starting_node;

  this->resize(node_cnt);

  Edge edge;

  while(file >> edge && edge_cnt)
  {
    if(edge.v1 >= this->size || edge.v1 < 0)
    {
      throw std::runtime_error(fmt::format(
        "Parse error! Node number out of range."
        " Excepted from [0,{}), got {}", this->size, edge.v1));
    }

    this->edge_list[edge.v1].append({edge.v2, edge.weight});
    --edge_cnt;
  }
}

sdizo2::dijkstra::DijkstraSolver
sdizo2::dijkstra::DijkstraSolver::buildFromFile(const char *filename)
{
  std::ifstream file(filename);

  if(!file.is_open())
    throw std::runtime_error(fmt::format("Cant open file {}", filename));

  int32_t edge_cnt;
  int32_t node_cnt;

  file >> edge_cnt;
  file >> node_cnt;

  sdizo2::dijkstra::DijkstraSolver dsolver(node_cnt);
  file >> dsolver.starting_node;

  Edge edge;

  while(file >> edge && edge_cnt)
  {
    if(edge.v1 >= dsolver.size || edge.v1 < 0)
    {
      throw std::runtime_error(fmt::format(
        "Parse error! Node number out of range."
        " Excepted from [0,{}), got {}", dsolver.size, edge.v1));
    }

    dsolver.edge_list[edge.v1].append({edge.v2, edge.weight});
    --edge_cnt;
  }

  return dsolver;
}

void sdizo2::dijkstra::DijkstraSolver::resize(int32_t newsize) noexcept
{
  this->cst.resize(newsize);

  delete [] this->edge_list;
  this->edge_list =
    new sdizo::List<sdizo::ListNode<MSTListNode>>[newsize];

  this->node_matrix.resize(newsize);

  this->size = newsize;
}

void sdizo2::dijkstra::DijkstraSolver::solve() noexcept
{
  sdizo2::dijkstra::LookupHeap node_heap(this->size);
  this->cst.reset(); // TODO do i need this?

  node_heap.reset();

  while(!node_heap.is_empty())
  {
    // Get current cheapest node
    auto node = node_heap.pop();

    // Get adjacent nodes
    auto edge = this->edge_list[node.node].get_cbegin();

    // For all adjacent nodes
    for(;edge != nullptr; edge = edge->next)
    {
      auto current_node_cost = this->cst.get_cost(node.node);
      auto current_edge_cost = edge->value.weight;
      auto current_dest_cost = this->cst.get_cost(edge->value.node);

      auto new_cost = current_node_cost + current_edge_cost;
      if(new_cost < current_dest_cost)
      {
        auto current_dest_node = edge->value.node;
        auto current_node = node.node;

        node_heap.update(current_dest_node, new_cost);
        cst.set({current_node, current_dest_node, new_cost});
      }
    }
  }
}

void sdizo2::dijkstra::DijkstraSolver::display() noexcept
{
  this->cst.display();

  for(auto i = 0; i < this->size; ++i)
    fmt::print("[{}] {}\n", i, this->edge_list[i]);
}
