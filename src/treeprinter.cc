#include <stdio.h>
#include <type_traits>
#include "tree.hpp"
#include "redblacktree.hpp"

static constexpr int shift_width = 10;

using sdizo::TreeNode;
using sdizo::RedBlackNode;
using sdizo::NodeColor;

template<typename T>
void print2DUtil(T *root, int space) noexcept
{
    if (root == nullptr)
        return;

    space += shift_width;

    print2DUtil(root->right, space);

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

    print2DUtil(root->left, space);
}

template<typename T>
void print2D(T *root) noexcept
{
    print2DUtil(root, 0);
}

template void print2D<TreeNode>(TreeNode *root) noexcept;
template void print2DUtil<TreeNode>(TreeNode *root, int space) noexcept;
template void print2D<RedBlackNode>(RedBlackNode *root) noexcept;
template void print2DUtil<RedBlackNode>(RedBlackNode *root, int space) noexcept;
