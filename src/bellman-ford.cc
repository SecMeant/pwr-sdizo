#include "bellman-ford.hpp"
#include "fmt_custom.hpp"

sdizo2::bfSolver::bfSolver(int32_t size) noexcept
:cst(size),
 edge_list(new sdizo::List<sdizo::ListNode<MSTListNode>>[size]),
 node_matrix(size), size(size), starting_node(0) {}

sdizo2::bfSolver::~bfSolver() noexcept
{
  delete [] this->edge_list;
}

void sdizo2::bfSolver::loadFromFile
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
    this->node_matrix.add_single(edge);
    --edge_cnt;
  }
}

sdizo2::bfSolver
sdizo2::bfSolver::buildFromFile(const char *filename)
{
  std::ifstream file(filename);

  if(!file.is_open())
    throw std::runtime_error(fmt::format("Cant open file {}", filename));

  int32_t edge_cnt;
  int32_t node_cnt;

  file >> edge_cnt;
  file >> node_cnt;

  sdizo2::bfSolver dsolver(node_cnt);
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
    dsolver.node_matrix.add_single(edge);
    --edge_cnt;
  }

  return dsolver;
}

void sdizo2::bfSolver::resize(int32_t newsize) noexcept
{
  this->cst.resize(newsize);

  delete [] this->edge_list;
  this->edge_list =
    new sdizo::List<sdizo::ListNode<MSTListNode>>[newsize];

  this->node_matrix.resize(newsize);

  this->cst.reset();

  this->size = newsize;
}

void sdizo2::bfSolver::solve() noexcept
{
  this->cst.reset(); // TODO do i need this?
  this->set_starting_node(this->starting_node);

  for(auto i = 0 ; i < this->size; ++i)
  {
    for(auto current_node = 0;
        current_node < this->size;
        ++current_node)
    {
      // Get adjacent nodes
      auto edge = this->edge_list[current_node].get_cbegin();

      // For all adjacent nodes
      for(;edge != nullptr; edge = edge->next)
      {
        auto current_node_cost = this->cst.get_cost(current_node);

        if(current_node_cost == sdizo2::CostSourceTable::INF)
          continue;

        auto current_edge_cost = edge->value.weight;
        auto current_dest_cost = this->cst.get_cost(edge->value.node);

        auto new_cost = current_node_cost + current_edge_cost;
        if(new_cost < current_dest_cost)
        {
          auto current_dest_node = edge->value.node;

          cst.set({current_node, current_dest_node, new_cost});
        }
      }
    }
  }
}

void sdizo2::bfSolver::solve_matrix() noexcept
{
  this->cst.reset(); // TODO do i need this?
  this->set_starting_node(this->starting_node);

  for(auto i = 0 ; i < this->size; ++i)
  {
    for(auto current_node = 0;
        current_node < this->size;
        ++current_node)
    {
      // Get adjacent nodes
      auto edge = this->node_matrix.get_next_adjacent(current_node, 0);

      // For all adjacent nodes
      for(;edge != MSTListNode{-1, -1};
          edge = this->node_matrix.get_next_adjacent(current_node, edge.node))
      {
        auto current_node_cost = this->cst.get_cost(current_node);

        if(current_node_cost == sdizo2::CostSourceTable::INF)
          continue;

        auto current_edge_cost = edge.weight;
        auto current_dest_cost = this->cst.get_cost(edge.node);

        auto new_cost = current_node_cost + current_edge_cost;
        if(new_cost < current_dest_cost)
        {
          auto current_dest_node = edge.node;

          cst.set({current_node, current_dest_node, new_cost});
        }
      }
    }
  }
}

void sdizo2::bfSolver::display() noexcept
{
  this->cst.display();
  this->node_matrix.display();

  for(auto i = 0; i < this->size; ++i)
    fmt::print("[{}] {}\n", i, this->edge_list[i]);
}

void sdizo2::bfSolver::
set_starting_node(int32_t snode) noexcept
{
  this->cst.set({-1, snode, 0});
}



