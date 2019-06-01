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
    this->node_matrix.add_single(edge);
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

int32_t
sdizo2::dijkstra::DijkstraSolver::generate(int32_t node_count, double density)
noexcept
{
  constexpr int32_t node_dist_range_begin = 0;
  constexpr int32_t weight_dist_range_begin = 1;
  constexpr int32_t weight_dist_range_end = 5;

  if(density < 0.0 || density > 1.0)
    return -1;

  this->resize(node_count);

  int32_t edges_to_gen = (node_count-1) * node_count / 2 * density;

  std::random_device generator;
  std::uniform_int_distribution<int32_t>
   node_dist(node_dist_range_begin, node_count-1);
  std::uniform_int_distribution<int32_t>
   weight_dist(weight_dist_range_begin, weight_dist_range_end);

  for(auto i = 0; i < node_count-1; ++i)
  {
    auto w = weight_dist(generator);
    node_matrix.add_single({i, i+1, w});
    this->edge_list[i].append({i+1,w});
  }

  edges_to_gen -= node_count;
  if(edges_to_gen <= 0)
    return 0;

  // TODO faster generation -- if density is for example 0.99,
  // there will be a lot of misses until hitting edge that is not
  // already in use.
  for(int32_t curr_node_cnt = 0; curr_node_cnt < edges_to_gen;)
  {
    auto x = node_dist(generator);
    auto y = node_dist(generator);

    if(!!node_matrix.get(x, y) || x == y)
      continue;

    auto weight = weight_dist(generator);

    node_matrix.add_single({x,y,weight});
    this->edge_list[x].append({y,weight});
    ++curr_node_cnt;
  }

  return 0;
}

void sdizo2::dijkstra::DijkstraSolver::resize(int32_t newsize) noexcept
{
  this->cst.resize(newsize);

  delete [] this->edge_list;
  this->edge_list =
    new sdizo::List<sdizo::ListNode<MSTListNode>>[newsize];

  this->node_matrix.resize(newsize);

  this->size = newsize;

  this->cst.reset();
}

void sdizo2::dijkstra::DijkstraSolver::solve() noexcept
{
  constexpr auto INF = sdizo2::CostSourceTable::INF;

  sdizo2::dijkstra::LookupHeap node_heap(this->size);
  this->cst.reset(); // TODO do i need this?
  this->set_starting_node(this->starting_node);

  node_heap.reset();

  this->display();

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

      auto new_cost = current_node_cost == INF ?
                      current_edge_cost :
                      current_node_cost + current_edge_cost;

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
  constexpr auto INF = sdizo2::CostSourceTable::INF;

  sdizo2::dijkstra::LookupHeap node_heap(this->size);
  this->cst.reset(); // TODO do i need this?
  this->set_starting_node(this->starting_node);

  node_heap.reset();

  while(!node_heap.is_empty())
  {
    // Get current cheapest node
    auto node = node_heap.pop();

    // Get adjacent nodes
    auto edge = this->node_matrix.get_next_adjacent(node.node, 0);

    // For all adjacent nodes
    for(;edge != MSTListNode{-1, -1};
        edge = this->node_matrix.get_next_adjacent(node.node, edge.node))
    {
      auto current_node_cost = this->cst.get_cost(node.node);
      auto current_edge_cost = edge.weight;
      auto current_dest_cost = this->cst.get_cost(edge.node);

      auto new_cost = current_node_cost == INF ?
                      current_edge_cost :
                      current_node_cost + current_edge_cost;

      if(new_cost < current_dest_cost)
      {
        auto current_dest_node = edge.node;
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

void sdizo2::dijkstra::DijkstraSolver::
set_starting_node(int32_t snode) noexcept
{
  this->cst.set({-1, snode, 0});
}
