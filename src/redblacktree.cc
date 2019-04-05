#include "treeprinter.hpp"
#include "redblacktree.hpp"
#include <cassert>
#include <stdexcept>
#include <random>

using sdizo::RedBlackNode;

void sdizo::RedBlackTree::insert(int32_t element) noexcept
{
  RedBlackNode *new_node = new RedBlackNode(element);
  this->insert(new_node);
}

void sdizo::RedBlackTree::tree_insert(RedBlackNode *node) noexcept
{
  RedBlackNode *current_parent = nullptr;
  RedBlackNode *current_node = this->root;
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

void sdizo::RedBlackTree::insert(RedBlackNode *node) noexcept
{
  this->tree_insert(node);
  node->color = NodeColor::red;

  if(node == this->root)
  {
    node->color = NodeColor::black;
    return;
  }

  this->fixRedBlackTree(node);
}

void sdizo::RedBlackTree::fixRedBlackTree(RedBlackNode *node) noexcept
{
  using node_ptr = RedBlackNode*;
  while((node != root) && (node->parent->color == NodeColor::red))
  {
    bool parent_is_left_child = node->parent == node->parent->parent->left;

    node_ptr uncle_node = parent_is_left_child ? node->parent->parent->right
                                               : node->parent->parent->left;

    // If uncle not null, pass blackness down.
    if(uncle_node && uncle_node->color == NodeColor::red)
    {
      node->parent->color = NodeColor::black;
      uncle_node->color = NodeColor::black;
      node->parent->parent->color = NodeColor::red;
      node = node->parent->parent;
      continue;
    }

    // Uncle is null, so rotate.
    if(parent_is_left_child)
    {
      if(node == node->parent->right)
      {
        node = node->parent;
        rot_left(node);
      }

      node->parent->color = NodeColor::black;
      node->parent->parent->color = NodeColor::red;
      rot_right(node->parent->parent);
      break;
    }
    else
    {
      if(node == node->parent->left)
      {
        node = node->parent;
        rot_right(node);
      }

      node->parent->color = NodeColor::black;
      node->parent->parent->color = NodeColor::red;
      rot_left(node->parent->parent);
      break;
    }
  }
  root->color = NodeColor::black;
}

void sdizo::RedBlackTree::remove(RedBlackNode *node)
{
  assert(node);

  if(this->root == nullptr)
    throw std::length_error("Tried to remove from already empty tree.");

  RedBlackNode *to_delete;
  RedBlackNode *x;

  if(node->left == nullptr || node->right == nullptr)
    to_delete = node;
  else
    to_delete = sdizo::RedBlackTree::successor(node);

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

RedBlackNode *sdizo::RedBlackTree::search(int32_t element) const noexcept
{
  RedBlackNode *current = this->root;
  while(current != nullptr && current->value != element)
  {
    if(current->value < element)
      current = current->right;
    else
      current = current->left;
  }

  return current;
}

RedBlackNode* sdizo::RedBlackTree::successor(RedBlackNode *root) noexcept
{
  assert(root);

  if(root->right != nullptr)
    return min(root->right);

  RedBlackNode *current_parent = root->parent;

  while
  (current_parent != nullptr &&
   root == current_parent->right)
  {
    root = current_parent;
    current_parent = root->parent;
  }

  return current_parent;
}

RedBlackNode* sdizo::RedBlackTree::predecessor(RedBlackNode *node) noexcept
{
  assert(node);

  if(node->left != nullptr)
    return max(node->right);

  RedBlackNode *current_parent = node->parent;

  while
  (current_parent != nullptr &&
   node == current_parent->left)
  {
    node = current_parent;
    current_parent = node->parent;
  }

  return current_parent;
}

RedBlackNode* sdizo::RedBlackTree::min(RedBlackNode *root) noexcept
{
  assert(root);
  while(root->left != nullptr)
    root = root->left;
  return root;
}

RedBlackNode* sdizo::RedBlackTree::max(RedBlackNode *root) noexcept
{
  assert(root);
  while(root->right != nullptr)
    root = root->right;
  return root;
}

void sdizo::RedBlackTree::rot_left(RedBlackNode *node) noexcept
{
  RedBlackNode *B = node->right;
  RedBlackNode *p = node->parent;

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

void sdizo::RedBlackTree::rot_right(RedBlackNode *node) noexcept
{
  RedBlackNode *B = node->left;
  RedBlackNode *p = node->parent;

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

unsigned sdizo::RedBlackTree::log2(unsigned x) noexcept
{
  unsigned y = 1;

  while((x >>= 1) > 0) y <<= 1;

  return y;
}

void sdizo::RedBlackTree::dsw() noexcept
{
  unsigned n,i,s;
  RedBlackNode* p;

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

  s = n + 1 - sdizo::RedBlackTree::log2(n + 1);

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
void sdizo::RedBlackTree::display() const noexcept
{
  puts("===========================");
  print2DUtil(this->root, 0);
  puts("===========================");
}

bool sdizo::RedBlackTree::verify_values() const noexcept
{
  return sdizo::RedBlackTree::verify_(this->root);
}

bool sdizo::RedBlackTree::verify_(RedBlackNode *root) noexcept
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

bool sdizo::RedBlackTree::verify_connections() const noexcept
{
  return this->verify_connections(this->root) &&
         this->root->parent == nullptr;
}

bool sdizo::RedBlackTree::verify_connections(RedBlackNode *node) noexcept
{
  if(node == nullptr)
    return true;

  if(node->left != nullptr && node->left->parent != node)
    return false;

  if(node->right != nullptr && node->right->parent != node)
    return false;

  return sdizo::RedBlackTree::verify_connections(node->left) &&
         sdizo::RedBlackTree::verify_connections(node->right);

}

void sdizo::RedBlackTree::free(RedBlackNode *to_delete) noexcept
{
  if(to_delete == nullptr)
    return;

  sdizo::RedBlackTree::free(to_delete->left);
  sdizo::RedBlackTree::free(to_delete->right);
  delete to_delete;
}

void sdizo::RedBlackTree::generate
(int32_t rand_range_begin, int32_t rand_range_end, int32_t size) noexcept
{
  std::random_device generator;
  std::uniform_int_distribution<int32_t>
   distribution(rand_range_begin, rand_range_end);

  for(int32_t i = 0; i < size; ++i)
  {
    this->insert(distribution(generator));
  }
}
