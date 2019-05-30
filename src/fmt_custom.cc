#include "fmt_custom.hpp"

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
