#pragma once
#include <cstdint>
#include <limits>

#include "mst.hpp"
#include "list.hpp"
#include "heap.hpp"
#include "mst.hpp"

namespace sdizo2::dijkstra{

// Dijkstra table for holding direction and cost of given path
// Example:
//        nodes ->  0  1  2  3  4  5
//  cost_tabele ->  0  3  4  6  3  5
// source_table -> -1  0  1  5  0  4
// Table shows for example that to node 4 we can come from node 0
// with total summary cost of 3.
// It initializes table with infinity values for costs and -1 for source.
class CostSourceTable
{
public:
  static constexpr int32_t INF = std::numeric_limits<int32_t>::max();
  static constexpr int32_t INVALID_SOURCE = -1;

private:
  int32_t *cost_table;
  int32_t *source_table;
  int32_t size;

public:
  CostSourceTable(int32_t size) noexcept;
  CostSourceTable(CostSourceTable&& cst) noexcept;
  ~CostSourceTable() noexcept;

  void reset() noexcept;
  void set_if(sdizo2::Edge edge);
  void set(sdizo2::Edge edge);

  void resize(int32_t newsize) noexcept;

  void display() noexcept;
};


class DijkstraSolver
{
public:
  CostSourceTable cst;
  sdizo::List<sdizo::ListNode<MSTListNode>> *node_list;
  sdizo2::MSTMatrix node_matrix;

  int32_t size;

public:
  DijkstraSolver(int32_t size) noexcept;
  DijkstraSolver(DijkstraSolver&& solver) noexcept = default;
  ~DijkstraSolver() noexcept;

  void loadFromFile(const char *filename);
  static DijkstraSolver buildFromFile(const char *filename);
  void resize(int32_t newsize) noexcept;

  void display() noexcept;
};

}; // namespace sdizo2
