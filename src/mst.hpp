#pragma once
#include <cstdint>
#include <fmt/ostream.h>

#include "list.hpp"
#include "heap.hpp"

namespace sdizo2{
struct  Edge
{
  typedef int32_t base_type;

  base_type v1;
  base_type v2;
  base_type weight;

  inline Edge() : v1{0}, v2{0}, weight{0} {}
  inline Edge(base_type v1, base_type v2, base_type weight)
  : v1{v1}, v2{v2}, weight{weight} {}

  inline int32_t getKey()
  {return weight;}

  inline bool operator==(Edge e)
  {
    return ((this->v1 == e.v1 && this->v2 == e.v2)  ||
            (this->v1 == e.v2 && this->v2 == e.v1)) &&
             this->weight == e.weight;
  }

  inline bool operator!=(Edge e)
  {
    return !(*this == e);
  }

  friend std::ostream& operator<<(std::ostream& os, const Edge& e)
  {
    return os << e.v1 << ' ' << e.v2 << ' ' << e.weight;
  }

  friend std::ifstream& operator>>(std::ifstream& is, sdizo2::Edge& e)
  {
    is >> e.v1 >> e.v2 >> e.weight;
    return is;
  }

};

namespace disjoint_set{
class DisjointSet;

struct DisjointNode
{
  int32_t value;
  DisjointNode *next = nullptr;
  DisjointSet *set = nullptr;

  DisjointNode(int32_t val, DisjointSet *parent_set);
  inline DisjointNode(DisjointNode node, DisjointSet *parent_set)
  :DisjointNode(node.value, parent_set) {}
};

class DisjointSet
{
private:
  DisjointNode *head;
  DisjointNode *tail;

public:
  DisjointSet(DisjointNode head)
  : head{new DisjointNode(head, this)}, tail{this->head} {}
  DisjointSet(int32_t value)
  : head{new DisjointNode(value, this)}, tail{this->head} {}
  ~DisjointSet()
  {
    while(this->head != nullptr)
    {
      auto next = this->head->next;
      delete this->head;
      this->head = next;
    }
  }

  void union_to(DisjointSet& set) noexcept
  {
    if(!this->head)
      return;

    if(!set.head)
      set.tail = this->tail;

    this->tail->next = set.head;

    set.head = this->head;

    while(this->head != nullptr)
    {
      this->head->set = &set;
      this->head = this->head->next;
    }

    this->tail = nullptr;
  }

  void display() noexcept;

};

inline DisjointSet make_set(DisjointNode head) noexcept
{ return DisjointSet(head); }

};

struct MSTListNode
{
  typedef int32_t value_type;

  value_type node;
  value_type weight;
};

class MSTList
{
public:
  sdizo::List<sdizo::ListNode<MSTListNode>> *tree;
  int32_t weight;
  int32_t size;

public:
  MSTList(int32_t size) noexcept;
  ~MSTList();
  void add(Edge edge) noexcept;
  void display() noexcept;
};

class KruskalSolver 
{
private:
  sdizo::List<sdizo::ListNode<Edge>> edge_list;
  sdizo::Heap<Edge, sdizo::HeapType::min> edge_heap;

public:
  void loadFromFile(const char *filename) noexcept;
  void display() noexcept;
  void solve() noexcept;

private:
  void prepare_heap() noexcept;

};

}; // namespace sdizo2
