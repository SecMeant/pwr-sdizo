#pragma once
#include <cstdint>
#include <cstdio>

namespace sdizo{
  enum class NodeColor
  {
    red, black
  };

  struct RedBlackNode
  {
    int32_t value;
    NodeColor color;
    RedBlackNode *left;
    RedBlackNode *right;
    RedBlackNode *parent;

    RedBlackNode(int32_t value)
      :value{value}, color{NodeColor::red}, left{nullptr}, right{nullptr},
       parent{nullptr} {}

    inline void info() const noexcept
    {
      if(this == nullptr)
      {
        std::puts("NULL");
        return;
      }

      auto value = this->value;
      auto color = this->color == NodeColor::black ? "black" : "red";
      auto left = this->left != nullptr ? this->left->value : -1;
      auto right = this->right != nullptr ? this->right->value : -1;
      auto parent = this->parent != nullptr ? this->parent->value : -1;
      std::printf("Value: %i, Color: %s, Left: %i, Right: %i, Parent: %i\n",
             value, color, left, right, parent);
    }

  };

  class RedBlackTree
  {
    private:
      RedBlackNode *root;
      RedBlackNode *null_node;

    public:
      inline RedBlackTree() noexcept
      :root{nullptr}, null_node{new RedBlackNode(0)}{}

      inline ~RedBlackTree() noexcept
      {this->free(this->root);delete this->null_node;}

      int32_t loadFromFile() noexcept;
      void insert(int32_t element) noexcept;
      void insert(RedBlackNode *node) noexcept;
      void remove(int32_t value);
      void remove(RedBlackNode *node);
      void generate(int32_t rand_range_begin, int32_t rand_range_end,
                    int32_t size) noexcept;
      RedBlackNode* search(int32_t element) const noexcept;

      // Returns nullptr if no valid node were found.
      // Otherwise valid poiter is returned.
      static RedBlackNode* successor(RedBlackNode *node) noexcept;
      static RedBlackNode* predecessor(RedBlackNode *node) noexcept;
      static RedBlackNode* min(RedBlackNode *root) noexcept;
      static RedBlackNode* max(RedBlackNode *root) noexcept;

      void rot_left(RedBlackNode *node) noexcept;
      void rot_right(RedBlackNode *node) noexcept;
      static unsigned log2(unsigned x) noexcept;

      void display() const noexcept;

      bool verify_values() const noexcept;
      bool verify_connections() const noexcept;

      inline RedBlackNode *get_root() const noexcept
      {return this->root;}

    private:
      // Recursively free's node and its childs.
      static void free(RedBlackNode *to_delete) noexcept;
      static bool verify_(RedBlackNode *root) noexcept;
      static bool verify_connections(RedBlackNode *node) noexcept;

      void tree_insert(RedBlackNode *node) noexcept;
  };
}
