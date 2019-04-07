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

  using node_ptr = RedBlackNode*;
  // redblacktree structure fix
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

void sdizo::RedBlackTree::remove(int32_t element)
{
  auto el = this->search(element);
  if(el)
    this->remove(el);
}

void sdizo::RedBlackTree::remove(RedBlackNode *node)
{
  assert(node);

  if(this->root == nullptr)
    throw std::length_error("Tried to remove from already empty tree.");

  RedBlackNode *to_delete;
  RedBlackNode *to_delete_child;

  if(node->left == nullptr || node->right == nullptr)
    to_delete = node;
  else
    to_delete = sdizo::RedBlackTree::successor(node);

  if(to_delete->left == nullptr)
    to_delete_child = to_delete->right;
  else
    to_delete_child = to_delete->left;

  if(to_delete_child == nullptr)
  {
    to_delete_child = this->null_node;
    this->null_node->parent = to_delete;
  }

  to_delete_child->parent = to_delete->parent;

  if(to_delete->parent == nullptr)
    this->root = to_delete_child;
  else if(to_delete == to_delete->parent->left)
  {
    to_delete->parent->left = to_delete_child;
  }
  else
  {
    to_delete->parent->right = to_delete_child;
  }

  if(to_delete != node)
  {
    node->value = to_delete->value;
  }

  if(to_delete->color == NodeColor::black)
  {
    RedBlackNode *brother;
    while(to_delete_child != this->root &&
          to_delete_child->color == NodeColor::black)
    {
      puts("While");
      to_delete_child->parent->info();
      to_delete_child->info();
      if(to_delete_child == to_delete_child->parent->left)
      {
        brother = to_delete_child->parent->right;
        if(brother->color == NodeColor::red)
        {
          brother->color = NodeColor::black;
          to_delete_child->parent->color = NodeColor::red;
          rot_left(to_delete_child->parent);
          brother = to_delete_child->parent->right;
        }
        if(brother->left->color == NodeColor::black &&
           brother->right->color == NodeColor::black)
        {
          brother->color = NodeColor::red;
          to_delete_child = to_delete_child->parent;
        }
        else if(brother->right->color == NodeColor::black)
        {
          brother->left->color = NodeColor::black;
          brother->color = NodeColor::red;
          rot_right(brother);
          brother = brother->parent->right;
        }
        brother->color = brother->parent->color;
        to_delete_child->parent->color = NodeColor::black;
        brother->right->color = NodeColor::black;
        rot_left(brother->parent);
        to_delete_child = this->root;
      }
      else
      {
        brother = to_delete_child->parent->left;
        if(brother == nullptr)
          brother = null_node;
        if(brother->color == NodeColor::red)
        {
          brother->color = NodeColor::black;
          to_delete_child->parent->color = NodeColor::red;
          rot_right(to_delete_child->parent);
          brother = to_delete_child->parent->left;
        }
        if(brother->left->color == NodeColor::black &&
           brother->right->color == NodeColor::black)
        {
          brother->color = NodeColor::red;
          to_delete_child = to_delete_child->parent;
        }
        else if(brother->left->color == NodeColor::black)
        {
          brother->right->color = NodeColor::black;
          brother->color = NodeColor::red;
          rot_left(brother);
          brother = brother->parent->left;
        }
        brother->color = brother->parent->color;
        to_delete_child->parent->color = NodeColor::black;
        brother->left->color = NodeColor::black;
        rot_right(brother->parent);
        to_delete_child = this->root;
      }
    }
    to_delete_child->color = NodeColor::black;
  }

  if(null_node->parent->left == null_node)
    null_node->parent->left = nullptr;
  if(null_node->parent->right == null_node)
    null_node->parent->right = nullptr;
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
