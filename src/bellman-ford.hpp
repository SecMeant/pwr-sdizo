#pragma once
#include "costsourcetable.hpp"

namespace sdizo2
{

class bfSolver
{
private:
  sdizo2::CostSourceTable cst;
  sdizo::List<sdizo::ListNode<MSTListNode>> *edge_list;
  sdizo2::MSTMatrix node_matrix;

  int32_t size;
  int32_t starting_node;

public:
  bfSolver(int32_t size) noexcept;
  bfSolver(bfSolver&& solver) noexcept = default;
  ~bfSolver() noexcept;

  void loadFromFile(const char *filename);
  static bfSolver buildFromFile(const char *filename);
  void resize(int32_t newsize) noexcept;

  void solve();
  void solve_matrix();
  int32_t generate(int32_t node_cnt, double density) noexcept;

  void display() noexcept;

private:
  void set_starting_node(int32_t snode) noexcept;
};

}; // namespace sdizo2


