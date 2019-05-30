#pragma once
#include <fmt/format.h>
#include <fmt/ostream.h>
#include "mst.hpp"
#include "dijkstra.hpp"

std::ostream& operator<<
  (std::ostream& os, const sdizo::List<sdizo::ListNode<sdizo2::MSTListNode>>& list);

std::ostream& operator<<
  (std::ostream& os, const sdizo2::disjoint_set::DisjointNode& node);
