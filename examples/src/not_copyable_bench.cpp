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

struct NotCopyableEntry {
  int val;
  size_t order;
  std::uint8_t payload[64];

  NotCopyableEntry(const NotCopyableEntry&) = delete;
  NotCopyableEntry(NotCopyableEntry&&) noexcept = default;
  NotCopyableEntry& operator=(const NotCopyableEntry&) = delete;
  NotCopyableEntry& operator=(NotCopyableEntry&&) noexcept = default;

  bool operator<(const NotCopyableEntry& entry) const noexcept {
    return this->val < entry.val;
  }

  void print(void) const {
    std::cout << val << " " << order << std::endl;
  }
};


void priority_queue_test(std::vector<NotCopyableEntry>&& data) {
  std::priority_queue<NotCopyableEntry> queue;

  for (auto&& val : data) {
    queue.push(std::move(val));
  }

  while (!queue.empty()) {
    queue.pop();
  }
}

void stable_priority_queue_test(std::vector<NotCopyableEntry>&& data) {
  komori::stable_priority_queue<NotCopyableEntry> queue;

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
    std::vector<NotCopyableEntry> data;
    for (size_t i = 0; i < kTestSize; ++i) {
      data.push_back(NotCopyableEntry {dist(mt), i, {}});
    }

    auto pq_start = std::chrono::system_clock::now();
    priority_queue_test(std::move(data));
    auto pq_end = std::chrono::system_clock::now();
    double pq_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(pq_end - pq_start).count();
    std::cout << "PQ:   " << pq_elapsed << "ms" << std::endl;
  }

  {
    std::mt19937 mt;
    std::vector<NotCopyableEntry> data;
    for (size_t i = 0; i < kTestSize; ++i) {
      data.push_back(NotCopyableEntry {dist(mt), i, {}});
    }

    auto spq_start = std::chrono::system_clock::now();
    stable_priority_queue_test(std::move(data));
    auto spq_end = std::chrono::system_clock::now();
    double spq_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(spq_end - spq_start).count();
    std::cout << "SPQ:  " << spq_elapsed << "ms" << std::endl;
  }

  return EXIT_SUCCESS;
}
