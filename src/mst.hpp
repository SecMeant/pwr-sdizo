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

  void reset(int32_t current_size) noexcept;
};

}; // namespace disjoint_set

struct MSTListNode
{
  typedef int32_t value_type;

  value_type node;
  value_type weight;

  inline bool operator==(MSTListNode n)
  {
    return this->node == n.node;
  }

  inline bool operator!=(MSTListNode n)
  {
    return !(*this == n);
  }
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

  void resize(int32_t newsize) noexcept;

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

  void add_single(Edge edge) noexcept;
  void add(Edge edge) noexcept;

  int32_t get(int32_t x, int32_t y);
  void set(int32_t x, int32_t y, int32_t val);

  // Returns next adjacent node to node passed as argument
  // after the one passed as second argument
  // returns {-1, -1} if no more adjacent nodes
  MSTListNode get_next_adjacent(int32_t node, int32_t last_adj);

  // Clears and resizes matrix
  void resize(int32_t newsize) noexcept;

  void display() noexcept;
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
  void loadFromFile(const char *filename);

  int32_t generate(int32_t node_count, double density) noexcept;

  // Displaying solved results
  void display() noexcept;

  inline void display_list() noexcept
  {this->mst_list.display();}

  inline void display_matrix() noexcept
  {this->mst_matrix.display();}


  // Displaying unsolved, fetched data
  void display_buffer_matrix() noexcept;
  void display_buffer_list() noexcept;


  // Solving
  virtual void solve() noexcept;

protected:
  void prepareHeap() noexcept;
  void resize(int32_t) noexcept;
};

class KruskalSolver :public MSTSolver
{
protected:
  disjoint_set::DisjointSet ds;

public:
  KruskalSolver() noexcept;
  KruskalSolver(int32_t node_count) noexcept;
  KruskalSolver(KruskalSolver&& solver) noexcept;
  KruskalSolver(MSTSolver&& base_solver) noexcept;
  virtual void solve() noexcept override;

private:
  void list_solve() noexcept;
  void matrix_solve() noexcept;

};

class PrimSolver :public MSTSolver
{
public:
  PrimSolver() noexcept;
  PrimSolver(int32_t node_count) noexcept;
  PrimSolver(PrimSolver&& solver) noexcept;
  PrimSolver(MSTSolver&& base_solver) noexcept;
  virtual void solve() noexcept override;

private:
  void list_solve() noexcept;
  void matrix_solve() noexcept;
};

}; // namespace sdizo2
