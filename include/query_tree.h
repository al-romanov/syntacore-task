#ifndef SRC_BALANCED_TREE_H_
#define SRC_BALANCED_TREE_H_

#include <cstddef>
#include <cstdint>
#include <memory>
#include <stack>

namespace syntacore {

class QueryTree final {
  struct Node;

 public:
  QueryTree();

  QueryTree(const QueryTree &rhs);

  QueryTree(QueryTree &&rhs) noexcept;

  QueryTree &operator=(const QueryTree &rhs);

  QueryTree &operator=(QueryTree &&rhs) noexcept;

  void Insert(int64_t value);

  int64_t NthSmallest(size_t n);

  size_t GetNumberOfSmallerValues(int64_t value);

 private:
  struct Node {
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    int64_t left_branch_size;
    int64_t right_branch_size;
    int64_t height;
    int64_t value;

    Node(int64_t val = 0);

    Node(const Node &rhs);

    Node(Node &&rhs) noexcept;

    Node &operator=(const Node &rhs);

    Node &operator=(Node &&rhs) noexcept;

   private:
    static std::unique_ptr<Node> Copy(const Node &src_node);
  };

  static void InsertInSubtree(std::unique_ptr<Node> &node, int64_t value);

  static void CountHeight(std::unique_ptr<Node> &node) noexcept;

  static void CountSizeOfBranches(std::unique_ptr<Node> &node) noexcept;

  static void Rotate(std::unique_ptr<Node> &node, int64_t value) noexcept;

  static void RotateLeft(std::unique_ptr<Node> &node) noexcept;

  static void RotateRight(std::unique_ptr<Node> &node) noexcept;

  std::unique_ptr<Node> root_;
};

}  // namespace syntacore

#endif  // SRC_BALANCED_TREE_H_
