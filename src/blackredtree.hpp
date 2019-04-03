#pragma once
#include <cstdint>

namespace sdizo{
  enum class NodeColor
  {
    red, black
  };

  struct BlackRedNode
  {
    int32_t value;
    NodeColor color;
    BlackRedNode *left;
    BlackRedNode *right;
    BlackRedNode *parent;

    BlackRedNode(int32_t value)
      :value{value}, color{NodeColor::red}, left{nullptr}, right{nullptr},
       parent{nullptr} {}
  };

  class BlackRedTree
  {
    private:
      BlackRedNode *root;

    public:
      inline BlackRedTree() noexcept
      :root{nullptr}{}

      inline ~BlackRedTree() noexcept
      {this->free(this->root);}

      int32_t loadFromFile() noexcept;
      void insert(int32_t element) noexcept;
      void insert(BlackRedNode *node) noexcept;
      void remove(int32_t value);
      void remove(BlackRedNode *node);
      void generate(int32_t rand_range_begin, int32_t rand_range_end,
                    int32_t size) noexcept;
      BlackRedNode* search(int32_t element) const noexcept;

      // Returns nullptr if no valid node were found.
      // Otherwise valid poiter is returned.
      static BlackRedNode* successor(BlackRedNode *node) noexcept;
      static BlackRedNode* predecessor(BlackRedNode *node) noexcept;
      static BlackRedNode* min(BlackRedNode *root) noexcept;
      static BlackRedNode* max(BlackRedNode *root) noexcept;

      void rot_left(BlackRedNode *node) noexcept;
      void rot_right(BlackRedNode *node) noexcept;
      static unsigned log2(unsigned x) noexcept;

      void convert_to_vine() noexcept;
      void display() const noexcept;
      void dsw() noexcept;

      bool verify_values() const noexcept;
      bool verify_connections() const noexcept;

    private:
      // Recursively free's node and its childs.
      static void free(BlackRedNode *to_delete) noexcept;
      static bool verify_(BlackRedNode *root) noexcept;
      static bool verify_connections(BlackRedNode *node) noexcept;
  };
}
