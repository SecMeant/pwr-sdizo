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

class DisjointNode
{
public:
  int32_t value;
  int32_t rank;
  DisjointNode *parent;

  DisjointNode(int32_t val, DisjointNode *parent);

private:
  DisjointNode() = default;

  friend class DisjointSet;
};

class DisjointSet
{
private:
  DisjointNode *nodes;
  int32_t size;

public:
  DisjointSet(int32_t size) noexcept;
  ~DisjointSet() noexcept;

  void makeSet(int32_t index) noexcept;
  void unionSet(DisjointNode *n1, DisjointNode *n2) noexcept;
  void linkSet(DisjointNode *n1, DisjointNode *n2) noexcept;
  DisjointNode *findSet(DisjointNode *n) noexcept;
  DisjointNode *findSet(int32_t index) noexcept;

  inline DisjointNode *get(int32_t index)
  {assert(index < this->size); assert(index >= 0); return &this->nodes[index];}

  void display() noexcept;

  void reset() noexcept;
};

}; // namespace disjoint_set

struct MSTListNode
{
  typedef int32_t value_type;

  value_type node;
  value_type weight;
};

class MSTList
{
private:
  sdizo::List<sdizo::ListNode<MSTListNode>> *tree;
  int32_t weight;
  int32_t size;

public:
  MSTList(int32_t size) noexcept;
  MSTList(MSTList &&ml) noexcept;
  ~MSTList();
  void add(Edge edge) noexcept;
  void display() noexcept;
};

class MSTMatrix
{
private:
  int32_t *tree;
  int32_t weight;
  int32_t size;

public:
  MSTMatrix(int32_t size) noexcept;
  MSTMatrix(MSTMatrix &&mm) noexcept;
  ~MSTMatrix();
  void add(Edge edge) noexcept;
  void display() noexcept;
  int32_t get(int32_t x, int32_t y);
  void set(int32_t x, int32_t y, int32_t val);
};

class MSTSolver
{
protected:
  sdizo::List<sdizo::ListNode<Edge>> edge_list;
  sdizo::Heap<Edge, sdizo::HeapType::min> edge_heap;
  MSTList mst_list;
  MSTMatrix mst_matrix;

  int32_t size;

public:
  MSTSolver(int32_t node_count);
  MSTSolver(MSTSolver&& solver) noexcept;
  static MSTSolver buildFromFile(const char *filename);
  void display() noexcept;
  virtual void solve() noexcept;

protected:
  void prepareHeap() noexcept;
};

class KruskalSolver :public MSTSolver
{
protected:
  disjoint_set::DisjointSet ds;

public:
  KruskalSolver(int32_t node_count);
  KruskalSolver(KruskalSolver&& solver) noexcept;
  KruskalSolver(MSTSolver&& base_solver) noexcept;
  virtual void solve() noexcept override;

private:
  void list_solve() noexcept;
  void heap_solve() noexcept;

};

class PrimSolver :public MSTSolver
{
public:
  PrimSolver(int32_t node_count);
  PrimSolver(PrimSolver&& solver) noexcept;
  PrimSolver(MSTSolver&& base_solver) noexcept;
  virtual void solve() noexcept override;

private:
  void list_solve() noexcept;
  void heap_solve() noexcept;
};

}; // namespace sdizo2
