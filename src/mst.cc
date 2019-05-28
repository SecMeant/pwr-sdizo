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

