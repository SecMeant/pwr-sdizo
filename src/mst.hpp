#pragma once
#include <cstdint>

namespace sdizo2{
  struct Edge
  {
    int32_t vertex;
    int32_t weight;

    inline Edge() : vertex{0}, weight{0} {}
    inline Edge(int32_t vertex, int32_t weight)
    : vertex{vertex}, weight{weight} {}

    inline int32_t getKey()
    {return weight;}

    inline bool operator==(const Edge& e)
    {
      return this->vertex == e.vertex &&
             this->weight == e.weight;
    }

    inline bool operator!=(const Edge& e)
    {
      return !(*this == e);
    }
  };
};
