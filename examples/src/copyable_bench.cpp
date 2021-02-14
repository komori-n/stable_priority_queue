#include <queue>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>
#include <chrono>
#include <map>
#include "komori/stable_priority_queue.hpp"

namespace {
constexpr size_t kTestSize = 1000000u;

struct CopyableEntry {
  int val;
  size_t order;
  std::uint8_t payload[64];

  CopyableEntry(const CopyableEntry&) = default;
  CopyableEntry(CopyableEntry&&) noexcept = default;
  CopyableEntry& operator=(const CopyableEntry&) = default;
  CopyableEntry& operator=(CopyableEntry&&) noexcept = default;

  bool operator<(const CopyableEntry& entry) const noexcept {
    return this->val < entry.val;
  }

  void print(void) const {
    std::cout << val << " " << order << std::endl;
  }
};


void priority_queue_test(std::vector<CopyableEntry>&& data) {
  std::priority_queue<CopyableEntry> queue;

  for (auto&& val : data) {
    queue.push(std::move(val));
  }

  while (!queue.empty()) {
    queue.pop();
  }
}

void stable_priority_queue_test(std::vector<CopyableEntry>&& data) {
  komori::stable_priority_queue<CopyableEntry> queue;

  for (auto&& val : data) {
    queue.push(std::move(val));
  }

  while (!queue.empty()) {
    queue.pop();
  }
}

}  // namespace

int main(void) {
  std::uniform_int_distribution<> dist(0, 1000);

  {
    std::mt19937 mt;
    std::vector<CopyableEntry> data;
    for (size_t i = 0; i < kTestSize; ++i) {
      data.push_back(CopyableEntry {dist(mt), i, {}});
    }

    auto pq_start = std::chrono::system_clock::now();
    priority_queue_test(std::move(data));
    auto pq_end = std::chrono::system_clock::now();
    double pq_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(pq_end - pq_start).count();
    std::cout << "PQ:   " << pq_elapsed << "ms" << std::endl;
  }

  {
    std::mt19937 mt;
    std::vector<CopyableEntry> data;
    for (size_t i = 0; i < kTestSize; ++i) {
      data.push_back(CopyableEntry {dist(mt), i, {}});
    }

    auto spq_start = std::chrono::system_clock::now();
    stable_priority_queue_test(std::move(data));
    auto spq_end = std::chrono::system_clock::now();
    double spq_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(spq_end - spq_start).count();
    std::cout << "SPQ:  " << spq_elapsed << "ms" << std::endl;
  }

  return EXIT_SUCCESS;
}
