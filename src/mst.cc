#include "mst.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <exception>
#include <fmt/format.h>

std::ostream& 
operator<<(std::ostream& os, const sdizo::List<sdizo::ListNode<sdizo2::MSTListNode>>& list)
{
  auto begin = list.get_cbegin();
  while(begin != nullptr)
  {
    os << "[" << begin->value.node << ", " << begin->value.weight << "], ";
    begin = begin->next;
  }

  return os;
}

sdizo2::MSTList::MSTList(int32_t size) noexcept
{
  this->tree = new sdizo::List<sdizo::ListNode<sdizo2::MSTListNode>>[size];
  this->weight = 0;
  this->size = size;
}

sdizo2::MSTList::MSTList(MSTList &&ml) noexcept
:tree(ml.tree), weight(ml.weight), size(ml.size)
{
  ml.tree = nullptr;
  ml.weight = 0;
  ml.size = 0;
}

sdizo2::MSTList::~MSTList()
{
  delete [] this->tree;
}

void sdizo2::MSTList::add(sdizo2::Edge edge) noexcept
{
  assert(edge.v1 < this->size);
  assert(edge.v2 < this->size);

  this->tree[edge.v1].append({edge.v2, edge.weight});
  this->tree[edge.v2].append({edge.v1, edge.weight});

  this->weight += edge.weight;
}

void sdizo2::MSTList::display() noexcept
{
  for(auto i = 0; i < this->size; ++i)
  {
    fmt::print("N[{}] -> {}\n", i, this->tree[i]);
  }
}

sdizo2::MSTMatrix::MSTMatrix(int32_t size) noexcept
{
  this->tree = new std::remove_pointer<decltype(this->tree)>::type[size*size];
  this->weight = 0;
  this->size = size;
}

sdizo2::MSTMatrix::MSTMatrix(MSTMatrix &&mm) noexcept
:tree(mm.tree), weight(mm.weight), size(mm.size)
{
  mm.tree = nullptr;
  mm.weight = 0;
  mm.size = 0;
}

sdizo2::MSTMatrix::~MSTMatrix()
{
  delete [] this->tree;
}

void sdizo2::MSTMatrix::add(sdizo2::Edge edge) noexcept
{
  assert(edge.v1 < this->size);
  assert(edge.v2 < this->size);

  // TODO add adding
  assert(false);

  this->weight += edge.weight;
}

int32_t sdizo2::MSTMatrix::get(int32_t x, int32_t y)
{
  assert(x < this->size);
  assert(x >= 0);
  assert(y < this->size);
  assert(y >= 0);

  return this->tree[x + y * this->size];
}

void sdizo2::MSTMatrix::set(int32_t x, int32_t y, int32_t val)
{
  assert(x < this->size);
  assert(x >= 0);
  assert(y < this->size);
  assert(y >= 0);

  this->tree[x + y * this->size] = val;
}

void sdizo2::MSTMatrix::display() noexcept
{
  for(auto y = 0; y < this->size; ++y)
  {
    for(auto x = 0; x < this->size; ++x)
    {
      fmt::print("{} ", this->get(x,y));
    }
    putchar('\n');
  }
}

sdizo2::KruskalSolver::KruskalSolver(int32_t node_count)
:mst_list(node_count), mst_matrix(node_count) {}

sdizo2::KruskalSolver::KruskalSolver(KruskalSolver&& solver) noexcept
:edge_list(std::move(solver.edge_list)), edge_heap(std::move(solver.edge_heap)),
 mst_list(std::move(solver.mst_list)), mst_matrix(std::move(solver.mst_matrix))
{}

sdizo2::KruskalSolver
sdizo2::KruskalSolver::buildFromFile(const char *filename)
{
  std::ifstream file(filename);

  if(!file.is_open())
    throw std::runtime_error(fmt::format("Cant open file {}", filename));

  int32_t edge_cnt;
  int32_t node_cnt;

  file >> edge_cnt;
  file >> node_cnt;

  KruskalSolver solver(node_cnt);

  Edge edge;

  while(file >> edge && edge_cnt)
  {
    solver.edge_list.append(edge);
    --edge_cnt;
  }

  return solver;
}

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
  this->mst_list.display();
  this->mst_matrix.display();
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

sdizo2::disjoint_set::DisjointNode::DisjointNode(int32_t val, DisjointSet *parent_set)
:value{val}, set{parent_set} {}

void sdizo2::disjoint_set::DisjointSet::display() noexcept
{
  auto head_val = this->head ? this->head->value : 0;
  auto tail_val = this->tail ? this->tail->value : 0;

  fmt::print("Head at: {}, with value: {}\n"
             "Tail at: {}, with value: {}\n",
             (void*)this->head, head_val,
             (void*)this->tail, tail_val);

  auto node = this->head;
  while(node != nullptr)
  {
    fmt::print("{} -> ", node->value);
    node = node->next;
  }
  putchar('\n');
}
