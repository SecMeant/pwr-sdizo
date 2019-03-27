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
    private:
      TreeNode *root;
    
    public:
      Tree() noexcept;
      ~Tree() noexcept;
      
      int32_t loadFromFile() noexcept;

      void insert(int32_t element) noexcept;
      void insert(TreeNode *node) noexcept;

      void remove(TreeNode *to_delete) noexcept;

      // Searches for element in container.
      // Returns true if element is in container.
      // False otherwise.
      bool contains(int32_t element) const noexcept;

      // Randomly generates table.
      void generate(int32_t rand_range_begin, int32_t rand_range_end,
                    int32_t size) noexcept;

      // Returns nullptr if there is no successor.
      // Otherwise valid poiter is returned.
      static TreeNode* successor(TreeNode *node) noexcept;
      static TreeNode* min(TreeNode *root) noexcept;
      static TreeNode* max(TreeNode *root) noexcept;

      void display() const noexcept;

    private:
      // Recursively free's node and its childs.
      static void free(TreeNode *to_delete) noexcept;
    };
}
