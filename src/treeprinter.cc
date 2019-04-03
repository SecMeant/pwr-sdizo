// C++ Program to print binary tree in 2D
#include <stdio.h>
#include "tree.hpp"
#include "redblacktree.hpp"
#define COUNT 10

using sdizo::TreeNode;
using sdizo::RedBlackNode;

// Function to print binary tree in 2D
// It does reverse inorder traversal
template<typename T>
void print2DUtil(T *root, int space) noexcept
{
    // Base case
    if (root == NULL)
        return;

    // Increase distance between levels
    space += COUNT;

    // Process right child first
    print2DUtil(root->right, space);

    // Print current node after space
    // count
    printf("\n%*s%i\n", space - COUNT, " ", root->value);

    // Process left child
    print2DUtil(root->left, space);
}

// Wrapper over print2DUtil()
template<typename T>
void print2D(T *root) noexcept
{
    // Pass initial space count as 0
    print2DUtil(root, 0);
}
// This code is contributed by rathbhupendra

template void print2D<TreeNode>(TreeNode *root) noexcept;
template void print2DUtil<TreeNode>(TreeNode *root, int space) noexcept;
template void print2D<RedBlackNode>(RedBlackNode *root) noexcept;
template void print2DUtil<RedBlackNode>(RedBlackNode *root, int space) noexcept;
