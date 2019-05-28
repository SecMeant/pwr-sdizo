#include "mst.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
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
