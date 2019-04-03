#pragma once
#include <cstdint>

namespace sdizo{
  struct TreeNode
  {
    int32_t value;
    TreeNode *left;
    TreeNode *right;
    TreeNode *parent;

    TreeNode(int32_t element) noexcept
    :value{element}, left{nullptr}, right{nullptr}, parent{nullptr} {}
  };

  class Tree
  {
    public:
      TreeNode *root;

    public:
      Tree() noexcept;
      ~Tree() noexcept;

      int32_t loadFromFile() noexcept;

      void insert(int32_t element) noexcept;
      void insert(TreeNode *node) noexcept;

      // If tries to remove from empty tree, std::length_error is thrown.
      void remove(TreeNode *to_delete);

      // Randomly generates table.
      void generate(int32_t rand_range_begin, int32_t rand_range_end,
                    int32_t size) noexcept;

      TreeNode *search(int32_t element) const noexcept;

      // Returns nullptr if there is no successor.
      // Otherwise valid poiter is returned.
      static TreeNode* successor(TreeNode *node) noexcept;
      static TreeNode* predecessor(TreeNode *node) noexcept;
      static TreeNode* min(TreeNode *root) noexcept;
      static TreeNode* max(TreeNode *root) noexcept;

      void rot_left(TreeNode *node) noexcept;
      void rot_right(TreeNode *node) noexcept;
      static unsigned log2(unsigned x) noexcept;

      void convert_to_vine() noexcept;
      void display() const noexcept;
      void dsw() noexcept;

      bool verify_values() const noexcept;
      bool verify_connections() const noexcept;

    private:
      // Recursively free's node and its childs.
      static void free(TreeNode *to_delete) noexcept;
      static bool verify_(TreeNode *root) noexcept;
      static bool verify_connections(TreeNode *node) noexcept;
    };
}
