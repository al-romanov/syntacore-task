#include "query_tree.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <type_traits>

namespace syntacore {

QueryTree::QueryTree() : root_(nullptr) {}

QueryTree::QueryTree(const QueryTree &rhs)
    : root_(std::make_unique<Node>(rhs.root_ ? *rhs.root_: nullptr)) {}

QueryTree::QueryTree(QueryTree &&rhs) noexcept : root_(std::move(rhs.root_)) {}

QueryTree &QueryTree::operator=(const QueryTree &rhs) {
  QueryTree tree{ rhs };
  std::swap(*this, tree);
  return *this;
}

QueryTree &QueryTree::operator=(QueryTree &&rhs) noexcept {
  root_ = std::move(rhs.root_);
  rhs.root_ = nullptr;
  return *this;
}

void QueryTree::Insert(int64_t value) { InsertInSubtree(root_, value); }

void QueryTree::InsertInSubtree(std::unique_ptr<Node> &node, int64_t value) {
  if (!node) {
    node = std::make_unique<Node>(value);
    return;
  }
  if (value < node->value) {
    InsertInSubtree(node->left, value);
  } else if (value > node->value) {
    InsertInSubtree(node->right, value);
  } else {
    std::ostringstream error;
    error << "QueryTree already contains value " << value;
    throw std::invalid_argument(error.str());
  }
  CountSizeOfBranches(node);
  CountHeight(node);
  Rotate(node, value);
}

void QueryTree::CountSizeOfBranches(std::unique_ptr<Node> &node) noexcept {
  node->left_branch_size = 0;
  node->right_branch_size = 0;
  if (node->left) {
    node->left_branch_size =
        node->left->left_branch_size + node->left->right_branch_size + 1;
  }
  if (node->right) {
    node->right_branch_size =
        node->right->left_branch_size + node->right->right_branch_size + 1;
  }
}

void QueryTree::CountHeight(std::unique_ptr<Node> &node) noexcept {
  node->height = 0;
  if (node->left) { node->height = node->left->height; }
  if (node->right) {
    node->height = std::max(node->height, node->right->height);
  }
  ++node->height;
}

void QueryTree::Rotate(std::unique_ptr<Node> &node, int64_t value) noexcept {
  int balance = 0;
  if (node->left) { balance += node->left->height; }
  if (node->right) { balance -= node->right->height; }
  if (balance > 1 && value < node->left->value) { RotateRight(node); }
  if (balance < -1 && value > node->right->value) { RotateLeft(node); }
  if (balance > 1 && value > node->left->value) {
    RotateLeft(node->left);
    RotateRight(node);
  }
  if (balance < -1 && value < node->right->value) {
    RotateRight(node->right);
    RotateLeft(node);
  }
}

void QueryTree::RotateLeft(std::unique_ptr<Node> &node) noexcept {
  auto right_child = std::move(node->right);
  auto child_left_subtree = std::move(right_child->left);
  right_child->left = std::move(node);
  right_child->left->right = std::move(child_left_subtree);
  node = std::move(right_child);
  CountSizeOfBranches(node->left);
  CountHeight(node->left);
  CountSizeOfBranches(node);
  CountHeight(node);
}

void QueryTree::RotateRight(std::unique_ptr<Node> &node) noexcept {
  auto left_child = std::move(node->left);
  auto child_right_subtree = std::move(left_child->right);
  left_child->right = std::move(node);
  left_child->right->left = std::move(child_right_subtree);
  node = std::move(left_child);
  CountSizeOfBranches(node->right);
  CountHeight(node->right);
  CountSizeOfBranches(node);
  CountHeight(node);
}

int64_t QueryTree::NthSmallest(size_t n) const {
  if (n == 0) {
    auto error = "QueryTree.NthSmallest supports n > 1 (0 given)";
    throw std::invalid_argument(error);
  }
  if (!root_ || root_->left_branch_size + root_->right_branch_size + 1 < n) {
    std::ostringstream error;
    error << "QueryTree contains less than " << n << " elements";
    throw std::invalid_argument(error.str());
  }
  auto *ptr_node = &root_;
  while (n != (*ptr_node)->left_branch_size + 1) {
    if ((*ptr_node)->left_branch_size >= n) {
      ptr_node = &(*ptr_node)->left;
    } else {
      n -= (*ptr_node)->left_branch_size + 1;
      ptr_node = &(*ptr_node)->right;
    }
  }
  return (*ptr_node)->value;
}

size_t QueryTree::GetNumberOfSmallerValues(int64_t value) const noexcept {
  size_t result = 0;
  auto *ptr_node = &root_;
  while (*ptr_node && (*ptr_node)->value != value) {
    if (value < (*ptr_node)->value) {
      ptr_node = &(*ptr_node)->left;
    } else {
      result += (*ptr_node)->left_branch_size + 1;
      ptr_node = &(*ptr_node)->right;
    }
  }
  if (*ptr_node) { result += (*ptr_node)->left_branch_size; }
  return result;
}

QueryTree::Node::Node(int64_t val)
    : left(nullptr),
      right(nullptr),
      value(val),
      left_branch_size(0),
      right_branch_size(0),
      height(1) {}

QueryTree::Node::Node(const Node &rhs)
    : left(nullptr),
      right(nullptr),
      value(rhs.value),
      left_branch_size(rhs.left_branch_size),
      right_branch_size(rhs.right_branch_size),
      height(rhs.height) {
  if (rhs.left) { left = Copy(*rhs.left); }
  if (rhs.right) { right = Copy(*rhs.right); }
}

QueryTree::Node::Node(QueryTree::Node &&rhs) noexcept
    : left(std::move(rhs.left)),
      right(std::move(rhs.right)),
      value(rhs.value),
      left_branch_size(rhs.left_branch_size),
      right_branch_size(rhs.right_branch_size),
      height(rhs.height) {}

QueryTree::Node &QueryTree::Node::operator=(const QueryTree::Node &rhs) {
  if (this == &rhs) { return *this; }
  Node dst_node{ rhs };
  std::swap(*this, dst_node);
  return *this;
}

QueryTree::Node &QueryTree::Node::operator=(QueryTree::Node &&rhs) noexcept {
  left = std::move(rhs.left);
  right = std::move(rhs.right);
  left_branch_size = rhs.left_branch_size;
  right_branch_size = rhs.right_branch_size;
  height = rhs.height;
  value = rhs.value;
  return *this;
}

std::unique_ptr<QueryTree::Node> QueryTree::Node::Copy(const Node &src_node) {
  auto dst_node = std::make_unique<Node>(src_node.value);
  dst_node->left_branch_size = src_node.left_branch_size;
  dst_node->right_branch_size = src_node.right_branch_size;
  dst_node->height = src_node.height;
  if (src_node.left) { dst_node->left = Copy(*src_node.left); }
  if (src_node.right) { dst_node->right = Copy(*src_node.right); }
  return dst_node;
}

}  // namespace syntacore
