#include <stdio.h>
#include <type_traits>
#include "tree.hpp"
#include "redblacktree.hpp"
#include "heap.hpp"
#include "common.hpp"

static constexpr int shift_width = 10;

using sdizo::TreeNode;
using sdizo::RedBlackNode;
using sdizo::NodeColor;

template<typename T>
void printTree(const T *root, int space) noexcept
{
    if (root == nullptr)
        return;

    space += shift_width;

    printTree(root->right, space);

    if constexpr(std::is_same<T, sdizo::RedBlackNode>::value)
    {
      if(root->color == NodeColor::red)
      {
        printf("\u001b[31m");
      }
    }

    if(root->value == sdizo::RedBlackTree::GUARD_VALUE) 
      printf("\n%*s%c\n", space - shift_width, " ", 'N');
    else
      printf("\n%*s%i\n", space - shift_width, " ", root->value);

    if constexpr(std::is_same<T, sdizo::RedBlackNode>::value)
      printf("\u001b[0m");

    printTree(root->left, space);
}

template<typename ElemType, sdizo::HeapType heap_t>
void printHeap
(const sdizo::Heap<ElemType, heap_t> *heap, int index, int space)
noexcept
{
  if (index >= heap->get_ssize())
    return;

  space += shift_width;

  printHeap(heap, RIGHT(index), space);


  printf("\n%*s%i\n", space - shift_width, " ",
         sdizo::key(heap->at(index)));

  printHeap(heap, LEFT(index), space);
}

template<typename T>
void print2D(const T *root) noexcept
{
  printTree(root, 0);
}

template<typename ElemType, sdizo::HeapType heap_t>
void print2D(const sdizo::Heap<ElemType, heap_t> *heap) noexcept
{
  printHeap(heap, 0, 0);
}

template void print2D<const TreeNode>(const TreeNode *root) noexcept;
template void printTree<const TreeNode>(const TreeNode *root, int space) noexcept;
template void print2D<const RedBlackNode>(const RedBlackNode *root) noexcept;
template void printTree<const RedBlackNode>(const RedBlackNode *root, int space) noexcept;
