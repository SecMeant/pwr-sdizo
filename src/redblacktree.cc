#include "treeprinter.hpp"
#include "redblacktree.hpp"
#include <cassert>
#include <stdexcept>
#include <random>

using sdizo::RedBlackNode;

void sdizo::RedBlackTree::insert(int32_t element) noexcept
{
  RedBlackNode *new_node = new RedBlackNode(element);
  new_node->right = this->null_node;
  new_node->left = this->null_node;
  this->insert(new_node);
}

void sdizo::RedBlackTree::tree_insert(RedBlackNode *node) noexcept
{
  RedBlackNode *current_parent = this->null_node;
  RedBlackNode *current_node = this->root;
  while(current_node != this->null_node)
  {
    current_parent = current_node;

    if(node->value < current_node->value)
      current_node = current_node->left;
    else
      current_node = current_node->right;
  }

  node->parent = current_parent;
  if(current_parent == this->null_node)
    this->root = node;
  else if(node->value < current_parent->value)
    current_parent->left = node;
  else
    current_parent->right = node;
}

void sdizo::RedBlackTree::insert(RedBlackNode *node) noexcept
{
  this->tree_insert(node);

  RedBlackNode *uncle;
  // redblacktree structure fix
  while((node != root) && (node->parent->color == NodeColor::red))
  {
    bool is_parent_left_child = 
      node->parent == node->parent->parent->left;

    uncle = is_parent_left_child ? node->parent->parent->right 
                                 : node->parent->parent->left;

    if(uncle->color == NodeColor::red)
    {
      node->parent->color = NodeColor::black;
      uncle->color = NodeColor::black;
      node->parent->parent->color = NodeColor::red;
      node = node->parent->parent;
      continue;
    }

    if(is_parent_left_child)
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
  if(el != this->null_node)
    this->remove(el);
}

void sdizo::RedBlackTree::remove(RedBlackNode *node)
{
  RedBlackNode *to_delete;
  RedBlackNode *to_delete_child;
  RedBlackNode *uncle;

  if ((node->left == this->null_node) || (node->right == this->null_node))
    to_delete = node;
  else
    to_delete = this->successor(node);

  if (to_delete->left != this->null_node)
    to_delete_child = to_delete->left;
  else
    to_delete_child = to_delete->right;

  to_delete_child->parent = to_delete->parent;

  if (to_delete->parent == this->null_node)
    root = to_delete_child;
  else if (to_delete == to_delete->parent->left)
    to_delete->parent->left = to_delete_child;
  else
    to_delete->parent->right = to_delete_child;

  if (to_delete != node)
    node->value = to_delete->value;

  // If removing black node, tree's black height needs to be fixed
  if (to_delete->color == NodeColor::black)
  {
    while((to_delete_child != root) &&
          (to_delete_child->color == NodeColor::black))
    {
      if (to_delete_child == to_delete_child->parent->left)
      {
        uncle = to_delete_child->parent->right;

        if (uncle->color == NodeColor::red)
        {
          uncle->color = NodeColor::black;
          to_delete_child->parent->color = NodeColor::red;
          rot_left(to_delete_child->parent);
          uncle = to_delete_child->parent->right;
        }

        if ((uncle->left->color == NodeColor::black) &&
            (uncle->right->color == NodeColor::black))
        {
          uncle->color = NodeColor::red;
          to_delete_child = to_delete_child->parent;
          continue;
        }

        if (uncle->right->color == NodeColor::black)
        {
          uncle->left->color = NodeColor::black;
          uncle->color = NodeColor::red;
          rot_right(uncle);
          uncle = to_delete_child->parent->right;
        }

        uncle->color = to_delete_child->parent->color;
        to_delete_child->parent->color = NodeColor::black;
        uncle->right->color = NodeColor::black;
        rot_left(to_delete_child->parent);
        to_delete_child = root;
      }
      else
      {
        uncle = to_delete_child->parent->left;

        if (uncle->color == NodeColor::red)
        {
          uncle->color = NodeColor::black;
          to_delete_child->parent->color = NodeColor::red;
          rot_right(to_delete_child->parent);
          uncle = to_delete_child->parent->left;
        }

        if ((uncle->left->color == NodeColor::black) &&
            (uncle->right->color == NodeColor::black))
        {
          uncle->color = NodeColor::red;
          to_delete_child = to_delete_child->parent;
          continue;
        }

        if (uncle->left->color == NodeColor::black)
        {
          uncle->right->color = NodeColor::black;
          uncle->color = NodeColor::red;
          rot_left(uncle);
          uncle = to_delete_child->parent->left;
        }

        uncle->color = to_delete_child->parent->color;
        to_delete_child->parent->color = NodeColor::black;
        uncle->left->color = NodeColor::black;
        rot_right(to_delete_child->parent);
        to_delete_child = root;
      }
    }
  }

  to_delete_child->color = NodeColor::black;
  delete to_delete;
}

RedBlackNode *sdizo::RedBlackTree::search(int32_t element) const noexcept
{
  RedBlackNode *current = this->root;
  while(current != this->null_node && current->value != element)
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

  if(root->right != this->null_node)
    return min(root->right);

  RedBlackNode *current_parent = root->parent;

  while
  (current_parent != this->null_node &&
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

  if(node->left != this->null_node)
    return max(node->right);

  RedBlackNode *current_parent = node->parent;

  while
  (current_parent != this->null_node &&
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
  while(root->left != this->null_node)
    root = root->left;
  return root;
}

RedBlackNode* sdizo::RedBlackTree::max(RedBlackNode *root) noexcept
{
  assert(root);
  while(root->right != this->null_node)
    root = root->right;
  return root;
}

void sdizo::RedBlackTree::rot_left(RedBlackNode *node) noexcept
{
  RedBlackNode *B = node->right;
  RedBlackNode *p = node->parent;

  if(B != this->null_node)
  {
    node->right = B->left;
    if(node->right != this->null_node) node->right->parent = node;

    B->left = node;
    B->parent = p;
    node->parent = B;

    if(p != this->null_node)
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

  if(B != this->null_node)
  {
    node->left = B->right;
    if(node->left != this->null_node) node->left->parent = node;

    B->right = node;
    B->parent = p;
    node->parent = B;

    if(p != this->null_node)
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

bool sdizo::RedBlackTree::verify_(RedBlackNode *root) const noexcept
{
  if(root == this->null_node)
    return true;

  if(!(verify_(root->left) & verify_(root->right)))
    return false;

  if(root->right != this->null_node && root->value > root->right->value)
    return false;

  if(root->left != this->null_node && root->value <= root->left->value)
    return false;

  return true;
}

bool sdizo::RedBlackTree::verify_connections() const noexcept
{
  return this->verify_connections(this->root) &&
         this->root->parent == this->null_node;
}

bool sdizo::RedBlackTree::verify_connections(RedBlackNode *node)
const noexcept
{
  if(node == this->null_node)
    return true;

  if(node->left != this->null_node && node->left->parent != node)
    return false;

  if(node->right != this->null_node && node->right->parent != node)
    return false;

  return sdizo::RedBlackTree::verify_connections(node->left) &&
         sdizo::RedBlackTree::verify_connections(node->right);
}

void sdizo::RedBlackTree::free(RedBlackNode *to_delete) noexcept
{
  if(to_delete == this->null_node)
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
