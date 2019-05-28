#pragma once
#include <cstdint>

namespace sdizo2{
struct Edge
{
  int32_t v1;
  int32_t v2;
  int32_t weight;

  inline Edge() : v1{0}, v2{0}, weight{0} {}
  inline Edge(int32_t v1, int32_t v2, int32_t weight)
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
};
}; // namespace sdizo2

