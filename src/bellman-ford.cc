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

void sdizo2::bfSolver::solve()
{
  constexpr auto INF = sdizo2::CostSourceTable::INF;

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

        auto new_cost = current_node_cost == INF ?
                        current_edge_cost :
                        current_node_cost + current_edge_cost;

        if(new_cost < current_dest_cost)
        {
          auto current_dest_node = edge->value.node;

          cst.set({current_node, current_dest_node, new_cost});
        }
      }
    }
  }

  // Check neg cycle
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

      auto current_edge_cost = edge->value.weight;
      auto current_dest_cost = this->cst.get_cost(edge->value.node);

      auto new_cost = current_node_cost + current_edge_cost;
      if(new_cost < current_dest_cost)
      {
        throw std::logic_error("Bellman-ford algorithm found negative cycle. "
                               "RESULT IS INVALID!");
      }
    }
  }
}

void sdizo2::bfSolver::solve_matrix()
{
  constexpr auto INF = sdizo2::CostSourceTable::INF;

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

        auto new_cost = current_node_cost == INF ?
                        current_edge_cost :
                        current_node_cost + current_edge_cost;

        if(new_cost < current_dest_cost)
        {
          auto current_dest_node = edge.node;

          cst.set({current_node, current_dest_node, new_cost});
        }
      }
    }
  }

  // Check neg cycle
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

      auto current_edge_cost = edge.weight;
      auto current_dest_cost = this->cst.get_cost(edge.node);

      auto new_cost = current_node_cost + current_edge_cost;
      if(new_cost < current_dest_cost)
      {
        throw std::logic_error("Bellman-ford algorithm found negative cycle. "
                               "RESULT IS INVALID!");
      }
    }
  }
}

int32_t
sdizo2::bfSolver::generate(int32_t node_count, double density)
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

void sdizo2::bfSolver::display() noexcept
{
  this->display_list();
  this->display_matrix();
}

void sdizo2::bfSolver::display_list() noexcept
{
  this->cst.display();
  for(auto i = 0; i < this->size; ++i)
    fmt::print("[{}] {}\n", i, this->edge_list[i]);
}

void sdizo2::bfSolver::display_matrix() noexcept
{
  this->cst.display();
  this->node_matrix.display();
}

void sdizo2::bfSolver::
set_starting_node(int32_t snode) noexcept
{
  this->cst.set({-1, snode, 0});
}



