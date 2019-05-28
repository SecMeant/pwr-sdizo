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

