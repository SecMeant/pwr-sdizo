#include "tree.hpp"
#include "treeprinter.hpp"
#include <stdio.h>
#include <cassert>

using sdizo::TreeNode;

sdizo::Tree::Tree() noexcept
:root{nullptr} {}

sdizo::Tree::~Tree() noexcept
{
  this->free(this->root);
}

void sdizo::Tree::insert(int32_t element) noexcept
{
  TreeNode *new_node = new TreeNode(element);
  this->insert(new_node);
}

void sdizo::Tree::insert(TreeNode *node) noexcept
{
  TreeNode *current_parent = nullptr;
  TreeNode *current_node = this->root;
  while(current_node != nullptr)
  {
    current_parent = current_node;

    if(node->value < current_node->value)
      current_node = current_node->left;
    else
      current_node = current_node->right;
  }

  node->parent = current_parent;
  if(current_parent == nullptr)
    this->root = node;
  else if(node->value < current_parent->value)
    current_parent->left = node;
  else
    current_parent->right = node;
}

TreeNode* sdizo::Tree::successor(TreeNode *root) noexcept
{
  assert(root);

  if(root->right != nullptr)
    return min(root);

  TreeNode *current_parent = root->parent;

  while
  (current_parent != nullptr &&
   root == current_parent->right)
  {
    root = current_parent;
    current_parent = root->parent; 
  }

  return current_parent;
}

TreeNode* sdizo::Tree::min(TreeNode *root) noexcept
{
  assert(root);
  while(root->left != nullptr)
    root = root->left;
  return root;
}

TreeNode* sdizo::Tree::max(TreeNode *root) noexcept
{
  assert(root);
  while(root->right != nullptr)
    root = root->right;
  return root;
}

void sdizo::Tree::display() const noexcept
{
  print2DUtil(this->root, 0);
}

void sdizo::Tree::free(TreeNode *to_delete) noexcept
{
  if(to_delete == nullptr)
    return;
  
  sdizo::Tree::free(to_delete->left);
  sdizo::Tree::free(to_delete->right);
  delete to_delete;
}
