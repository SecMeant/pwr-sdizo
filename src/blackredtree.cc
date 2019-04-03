#include "treeprinter.hpp"
#include "blackredtree.hpp"
#include <cassert>
#include <stdexcept>

using sdizo::BlackRedNode;

void sdizo::BlackRedTree::insert(int32_t element) noexcept
{
  BlackRedNode *new_node = new BlackRedNode(element);
  this->insert(new_node);
}

void sdizo::BlackRedTree::insert(BlackRedNode *node) noexcept
{
  BlackRedNode *current_parent = nullptr;
  BlackRedNode *current_node = this->root;
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

void sdizo::BlackRedTree::remove(BlackRedNode *node)
{
  assert(node);

  if(this->root == nullptr)
    throw std::length_error("Tried to remove from already empty tree.");

  BlackRedNode *to_delete;
  BlackRedNode *x;

  if(node->left == nullptr || node->right == nullptr)
    to_delete = node;
  else
    to_delete = sdizo::BlackRedTree::successor(node);

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

BlackRedNode *sdizo::BlackRedTree::search(int32_t element) const noexcept
{
  BlackRedNode *current = this->root;
  while(current != nullptr && current->value != element)
  {
    if(current->value < element)
      current = current->right;
    else
      current = current->left;
  }

  return current;
}

BlackRedNode* sdizo::BlackRedTree::successor(BlackRedNode *root) noexcept
{
  assert(root);

  if(root->right != nullptr)
    return min(root->right);

  BlackRedNode *current_parent = root->parent;

  while
  (current_parent != nullptr &&
   root == current_parent->right)
  {
    root = current_parent;
    current_parent = root->parent; 
  }

  return current_parent;
}

BlackRedNode* sdizo::BlackRedTree::predecessor(BlackRedNode *node) noexcept
{
  assert(node);

  if(node->left != nullptr)
    return max(node->right);

  BlackRedNode *current_parent = node->parent;

  while
  (current_parent != nullptr &&
   node == current_parent->left)
  {
    node = current_parent;
    current_parent = node->parent; 
  }

  return current_parent;
}

BlackRedNode* sdizo::BlackRedTree::min(BlackRedNode *root) noexcept
{
  assert(root);
  while(root->left != nullptr)
    root = root->left;
  return root;
}

BlackRedNode* sdizo::BlackRedTree::max(BlackRedNode *root) noexcept
{
  assert(root);
  while(root->right != nullptr)
    root = root->right;
  return root;
}

void sdizo::BlackRedTree::rot_left(BlackRedNode *node) noexcept
{
  BlackRedNode *B = node->right;
  BlackRedNode *p = node->parent;

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

void sdizo::BlackRedTree::rot_right(BlackRedNode *node) noexcept
{
  BlackRedNode *B = node->left;
  BlackRedNode *p = node->parent;

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

unsigned sdizo::BlackRedTree::log2(unsigned x) noexcept
{
  unsigned y = 1;

  while((x >>= 1) > 0) y <<= 1;

  return y;
}

void sdizo::BlackRedTree::dsw() noexcept
{
  unsigned n,i,s;
  BlackRedNode* p;

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

  s = n + 1 - sdizo::BlackRedTree::log2(n + 1);

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
void sdizo::BlackRedTree::display() const noexcept
{
  puts("===========================");
  print2DUtil(this->root, 0);
  puts("===========================");
}

bool sdizo::BlackRedTree::verify_values() const noexcept
{
  return sdizo::BlackRedTree::verify_(this->root);
}

bool sdizo::BlackRedTree::verify_(BlackRedNode *root) noexcept
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

bool sdizo::BlackRedTree::verify_connections() const noexcept
{
  return this->verify_connections(this->root) && this->root->parent == nullptr;
}

bool sdizo::BlackRedTree::verify_connections(BlackRedNode *node) noexcept
{
  if(node == nullptr)
    return true;

  if(node->left != nullptr && node->left->parent != node)
    return false;

  if(node->right != nullptr && node->right->parent != node)
    return false;

  return sdizo::BlackRedTree::verify_connections(node->left) && sdizo::BlackRedTree::verify_connections(node->right);

}

void sdizo::BlackRedTree::free(BlackRedNode *to_delete) noexcept
{
  if(to_delete == nullptr)
    return;

  sdizo::BlackRedTree::free(to_delete->left);
  sdizo::BlackRedTree::free(to_delete->right);
  delete to_delete;
}
