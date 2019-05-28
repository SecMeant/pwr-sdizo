#include "mst.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <fmt/format.h>

void sdizo2::KruskalSolver::loadFromFile(const char *filename) noexcept
{
  std::ifstream file(filename);
  int edge_cnt;
  int node_cnt;
  Edge edge;

  file >> edge_cnt >> node_cnt;

  while(file >> edge && edge_cnt)
  {
    this->edge_list.append(edge);
    --edge_cnt;
  }
};

void sdizo2::KruskalSolver::display() noexcept
{
  auto edge_begin = this->edge_list.get_begin();

  while(edge_begin != nullptr)
  {
    fmt::print("{}\n", edge_begin->value);
    edge_begin = edge_begin->next;
  }

}

void sdizo2::KruskalSolver::solve() noexcept
{
  this->prepare_heap();

  while(!this->edge_heap.is_empty())
    fmt::print("{}\n", this->edge_heap.pop());
}

void sdizo2::KruskalSolver::prepare_heap() noexcept
{
  this->edge_heap.clear();

  auto edge_node = this->edge_list.get_begin();

  while(edge_node != nullptr)
  {
    this->edge_heap.insert(edge_node->value);
    edge_node = edge_node->next;
  }
}
