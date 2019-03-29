#include "tree.hpp"
#include "treeprinter.hpp"
#include <stdio.h>
#include <cassert>
#include <stdexcept>
#include <cmath>

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

void sdizo::Tree::remove(TreeNode *node)
{
  assert(node);

  if(this->root == nullptr)
    throw std::length_error("Tried to remove from already empty tree.");

  TreeNode *to_delete;
  TreeNode *x;

  if(node->left == nullptr || node->right == nullptr)
    to_delete = node;
  else
    to_delete = sdizo::Tree::successor(node);
  
  if(to_delete->left != nullptr)
    x = to_delete->left;
  else
    x = to_delete->right;
  
  if(x != nullptr)
    x->parent = to_delete->parent;
  
  if(to_delete->parent == nullptr)
    this->root = x;
  else if(to_delete == to_delete->parent->left)
    to_delete->parent->left = x;
  else
    to_delete->parent->right = x;
  
  if(to_delete != node)
    node->value = to_delete->value;
  
  delete to_delete;
}

TreeNode *sdizo::Tree::search(int32_t element) const noexcept
{
  TreeNode *current = this->root;
  while(current != nullptr && current->value != element)
  {
    if(current->value < element)
      current = current->right;
    else
      current = current->left;
  }

  return current;
}

TreeNode* sdizo::Tree::successor(TreeNode *root) noexcept
{
  assert(root);

  if(root->right != nullptr)
    return min(root->right);

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

TreeNode* sdizo::Tree::predecessor(TreeNode *node) noexcept
{
  assert(node);

  if(node->left != nullptr)
    return max(node->right);

  TreeNode *current_parent = node->parent;

  while
  (current_parent != nullptr &&
   node == current_parent->left)
  {
    node = current_parent;
    current_parent = node->parent; 
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

void sdizo::Tree::rot_left(TreeNode *node) noexcept
{
  TreeNode *B = node->right;
  TreeNode *p = node->parent;

  if(B)
  {
    node->right = B->left;
    if(node->right) node->right->parent = node;

    B->left = node;
    B->parent = p;
    node->parent = B;

    if(p)
    {
      if(p->left == node) p->left = B; else p->right = B;
    }
    else root = B;
  }
}

void sdizo::Tree::rot_right(TreeNode *node) noexcept
{
  TreeNode *B = node->left;
  TreeNode *p = node->parent;

  if(B)
  {
    node->left = B->right;
    if(node->left) node->left->parent = node;

    B->right = node;
    B->parent = p;
    node->parent = B;

    if(p)
    {
      if(p->left == node) p->left = B; else p->right = B;
    }
    else root = B;
  }
}

unsigned sdizo::Tree::log2(unsigned x) noexcept
{
  unsigned y = 1;

  while((x >>= 1) > 0) y <<= 1;

  return y;
}

void sdizo::Tree::dsw() noexcept
{
  unsigned n,i,s;
  TreeNode* p;

  n = 0;
  p = this->root;
  while(p)
    if(p->left)
      {
        rot_right(p);
        p = p->parent;
      }
    else
    {
      n++;
      p = p->right;
    }

  s = n + 1 - sdizo::Tree::log2(n + 1);

  p = root;
  for(i = 0; i < s; i++)
  {
    rot_left(p);
    p = p->parent->right;
  }

  n = n - s;


  while(n > 1)
  {
    n >>= 1;
    p = root;
    for(i = 0; i < n; i++)
    {
      rot_left(p);
      p = p->parent->right;
    }
  }
}
void sdizo::Tree::display() const noexcept
{
  puts("===========================");
  print2DUtil(this->root, 0);
  puts("===========================");
}

bool sdizo::Tree::verify_values() const noexcept
{
  return sdizo::Tree::verify_(this->root);
}

bool sdizo::Tree::verify_(TreeNode *root) noexcept
{
  if(root == nullptr)
    return true;
  
  if(!(verify_(root->left) & verify_(root->right)))
    return false;

  if(root->right != nullptr && root->value > root->right->value)
    return false;

  if(root->left != nullptr && root->value <= root->left->value)
    return false;

  return true;
}

bool sdizo::Tree::verify_connections() const noexcept
{
  return this->verify_connections(this->root) && this->root->parent == nullptr;
}

bool sdizo::Tree::verify_connections(TreeNode *node) noexcept
{
  if(node == nullptr)
    return true;

  if(node->left != nullptr && node->left->parent != node)
    return false;

  if(node->right != nullptr && node->right->parent != node)
    return false;

  return sdizo::Tree::verify_connections(node->left) && sdizo::Tree::verify_connections(node->right);

}

void sdizo::Tree::free(TreeNode *to_delete) noexcept
{
  if(to_delete == nullptr)
    return;
  
  sdizo::Tree::free(to_delete->left);
  sdizo::Tree::free(to_delete->right);
  delete to_delete;
}
