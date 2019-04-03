#pragma once
#include <cstdint>

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
  };

  class RedBlackTree
  {
    private:
      RedBlackNode *root;

    public:
      inline RedBlackTree() noexcept
      :root{nullptr}{}

      inline ~RedBlackTree() noexcept
      {this->free(this->root);}

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

      void convert_to_vine() noexcept;
      void display() const noexcept;
      void dsw() noexcept;

      bool verify_values() const noexcept;
      bool verify_connections() const noexcept;

    private:
      // Recursively free's node and its childs.
      static void free(RedBlackNode *to_delete) noexcept;
      static bool verify_(RedBlackNode *root) noexcept;
      static bool verify_connections(RedBlackNode *node) noexcept;
  };
}
