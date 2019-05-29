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

std::ostream&
operator<<(std::ostream& os, const sdizo2::disjoint_set::DisjointNode& node)
{
  os << "[Val: " << node.value << ", Rank: " << node.rank
    << ", Parent: " << node.parent << "] ";

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

  std::fill(this->tree, this->tree+(size*size), 0);
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

  this->set(edge.v1, edge.v2, edge.weight);
  this->set(edge.v2, edge.v1, edge.weight);

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
:ds(node_count), mst_list(node_count), mst_matrix(node_count) {}

sdizo2::KruskalSolver::KruskalSolver(KruskalSolver&& solver) noexcept
:edge_list(std::move(solver.edge_list)), edge_heap(std::move(solver.edge_heap)),
 ds(std::move(solver.ds)), mst_list(std::move(solver.mst_list)),
 mst_matrix(std::move(solver.mst_matrix)), size(solver.size)
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

void sdizo2::KruskalSolver::loadFromFile(const char *filename)
{
  std::ifstream file(filename);
  int edge_cnt;
  int node_cnt;
  Edge edge;

  file >> edge_cnt >> node_cnt;

  if(node_cnt != this->size)
  {
    throw std::runtime_error(
      fmt::format("KruskalSolver created for {} nodes, but file with {} nodes loaded.\n",
                  this->size, node_cnt));
  }

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
  this->ds.reset();
  this->list_solve();

  this->ds.reset();
  this->heap_solve();
}

void sdizo2::KruskalSolver::list_solve() noexcept
{
  this->prepare_heap();

  auto node_cnt = 0;

  while(node_cnt != this->size && !this->edge_heap.is_empty())
  {
    auto edge = this->edge_heap.pop();
    if(ds.findSet(edge.v1) == ds.findSet(edge.v2))
      continue;

    mst_list.add(edge);
    ds.unionSet(ds.get(edge.v1), ds.get(edge.v2));
  }
}

void sdizo2::KruskalSolver::heap_solve() noexcept
{
  this->prepare_heap();

  auto node_cnt = 0;

  while(node_cnt != this->size && !this->edge_heap.is_empty())
  {
    auto edge = this->edge_heap.pop();
    if(ds.findSet(edge.v1) == ds.findSet(edge.v2))
      continue;

    mst_matrix.add(edge);
    ds.unionSet(ds.get(edge.v1), ds.get(edge.v2));
  }
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

sdizo2::disjoint_set::DisjointNode::DisjointNode(int32_t val, DisjointNode *parent)
:value{val}, parent{parent} {}

sdizo2::disjoint_set::DisjointSet::DisjointSet(int32_t size) noexcept
: nodes(new DisjointNode[size]), size(size)
{
  for( auto i = 0; i < size; ++i)
  {
    this->makeSet(i);
  }
}

sdizo2::disjoint_set::DisjointSet::~DisjointSet() noexcept
{
  delete [] this->nodes;
}

void sdizo2::disjoint_set::DisjointSet::makeSet(int32_t i) noexcept
{
  assert(i < this->size);
  assert(i >= 0);

  this->nodes[i].value = i;
  this->nodes[i].parent = &this->nodes[i];
  this->nodes[i].rank = 0;
}

void sdizo2::disjoint_set::DisjointSet::unionSet
(DisjointNode *n1, DisjointNode *n2) noexcept
{
  this->linkSet(this->findSet(n1), this->findSet(n2));
}

void sdizo2::disjoint_set::DisjointSet::linkSet
(DisjointNode *n1, DisjointNode *n2) noexcept
{
  if(n1->rank > n2->rank)
    n2->parent = n1;
  else 
  {
    n1->parent = n2;
    if(n1->rank == n2->rank)
      n2->rank = n2->rank+1;
  }

}

sdizo2::disjoint_set::DisjointNode *sdizo2::disjoint_set::DisjointSet::findSet
(DisjointNode *n) noexcept
{
  if(n->parent != n)
    n->parent = this->findSet(n->parent);
  return n->parent;
}

sdizo2::disjoint_set::DisjointNode *sdizo2::disjoint_set::DisjointSet::findSet
(int32_t idx) noexcept
{
  auto n = this->get(idx);

  if(n->parent != n)
    n->parent = this->findSet(n->parent);
  return n->parent;
}

void sdizo2::disjoint_set::DisjointSet::display() noexcept
{
  for( auto i = 0; i < this->size; ++i)
  {
    auto node = &this->nodes[i];
    fmt::print("{} -> ", *node);

    while(node != node->parent)
    {
      node = node->parent;
      fmt::print("{} -> ", *node);
    }

    fmt::print("self\n\n");
  }

}

void sdizo2::disjoint_set::DisjointSet::reset() noexcept
{
  for(auto i = 0; i < this->size; ++i)
    this->makeSet(i);
}
