#pragma once
#include <cstdint>
#include <limits>

#include "mst.hpp"
#include "list.hpp"
#include "heap.hpp"
#include "mst.hpp"

namespace sdizo2::dijkstra{

struct DijkstraNode
{
  typedef int32_t base_type;

  int32_t node;
  int32_t cost;

  inline DijkstraNode() : node(0), cost(0) {}
  inline DijkstraNode(base_type node, base_type cost)
  : node{node}, cost{cost} {}

  inline int32_t getKey()
  {return cost;}

  inline bool operator==(DijkstraNode n)
  {
    return this->node == n.node && this->cost == n.cost;
  }

  inline bool operator!=(DijkstraNode n)
  {
    return !(*this == n);
  }

  friend std::ostream&
  operator<<(std::ostream& os, const DijkstraNode& n)
  {
    return os << n.node << ' ' << n.cost;
  }

  friend std::ifstream&
  operator>>(std::ifstream& is, DijkstraNode& n)
  {
    is >> n.node >> n.cost;
    return is;
  }
};

// Dijkstra table for holding direction and cost of given path
// Example:
//        nodes ->  0  1  2  3  4  5
//  cost_tabele ->  0  3  4  6  3  5
// source_table -> -1  0  1  5  0  4
// Table shows for example that to node 4 we can come from node 0
// with total summary cost of 3.
// It initializes table with infinity values for costs and -1 for source.
class CostSourceTable
{
public:
  static constexpr int32_t INF = std::numeric_limits<int32_t>::max();
  static constexpr int32_t INVALID_SOURCE = -1;

private:
  int32_t *cost_table;
  int32_t *source_table;
  int32_t size;

public:
  CostSourceTable(int32_t size) noexcept;
  CostSourceTable(CostSourceTable&& cst) noexcept;
  ~CostSourceTable() noexcept;

  void reset() noexcept;
  void set_if_smaller(sdizo2::Edge edge);
  void set(sdizo2::Edge edge);

  int32_t get_cost(int32_t index);
  int32_t get_source(int32_t index);

  void resize(int32_t newsize) noexcept;

  void display() noexcept;
};


class DijkstraSolver
{
private:
  CostSourceTable cst;
  sdizo::List<sdizo::ListNode<MSTListNode>> *node_list;
  sdizo2::MSTMatrix node_matrix;

  int32_t size;
  int32_t starting_node;

public:
  DijkstraSolver(int32_t size) noexcept;
  DijkstraSolver(DijkstraSolver&& solver) noexcept = default;
  ~DijkstraSolver() noexcept;

  void loadFromFile(const char *filename);
  static DijkstraSolver buildFromFile(const char *filename);
  void resize(int32_t newsize) noexcept;

  void solve() noexcept;

  void display() noexcept;
};

#ifndef HEAP_MACROS
#define HEAP_MACROS
#define PARENT(i) ((i - 1)>>1)
#define LEFT(i) ((i<<1)+1)
#define RIGHT(i) ((i+1)<<1)
#endif

class LookupHeap
{
  public:
    using ElemType = sdizo2::dijkstra::DijkstraNode;

  private:
    // Each time heap expands in memory (need to be relocated)
    // it allocates memory of size given in expand_size * sizeof(remove_ptr(array_type)).
    constexpr static size_t expand_size = 4;

  private:
    ElemType *array;
    int32_t *lookup_table;
    int32_t ssize;
    int32_t length;

  public:
    LookupHeap(int32_t size) noexcept;
    LookupHeap(LookupHeap&& h) noexcept;
    ~LookupHeap() noexcept;

    // Returns element from given index.
    // Throws std::out_of_range if index exceeds read span of array.
    ElemType at(int32_t index) const;

    // Inserts element at location given by index.
    // Throws std::out_of_range if index exceeds insert span of array,
    void insert(ElemType element);

    // Throws std::out_of_range if index exceeds span of array.
    void removeAt(int32_t index);

    inline ElemType pop()
    {ElemType ret = this->at(0); this->removeAt(0); return ret;}

    void remove(ElemType element) noexcept;

    void update(int32_t index, ElemType element) noexcept;

    // Removes all elements
    void clear() noexcept;

    // Searches for element in container.
    // Returns true if element is in container.
    // False otherwise.
    bool contains(ElemType element) const noexcept;

    void display() const noexcept;

    // Checks if container is proper heap,
    bool verify() const noexcept;

    inline decltype(auto) get_ssize() const noexcept
    {return this->ssize;}

    inline bool is_empty() const noexcept
    {return this->ssize == 0;}

  private:
    void heapify(int32_t index) noexcept;

    // Reallocs heap to new, bigger space.
    // New space is bigger by Heap::expand_size.
    void expand() noexcept;

    // Finds element in table.
    // If element is in the table, returns it's index, -1 otherwise.
    int32_t find(ElemType elem) const noexcept;
};

}; // namespace sdizo2

inline void printHeap
(const sdizo2::dijkstra::LookupHeap *heap, int index, int space) noexcept
{
  if (index >= heap->get_ssize())
    return;

  space += shift_width;

  printHeap(heap, RIGHT(index), space);


  printf("\n%*s%i\n", space - shift_width, " ",
         sdizo::key(heap->at(index)));

  printHeap(heap, LEFT(index), space);
}

inline void print2D(const sdizo2::dijkstra::LookupHeap *heap) noexcept
{
  printHeap(heap, 0, 0);
}
