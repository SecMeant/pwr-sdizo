#include <exception>
#include <fmt/ostream.h>
#include "dijkstra.hpp"
#include "fmt_custom.hpp"

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
    this->node_matrix.add(edge);
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
    dsolver.node_matrix.add_single(edge);
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

void sdizo2::dijkstra::DijkstraSolver::solve_matrix() noexcept
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

  this->node_matrix.display();
}
