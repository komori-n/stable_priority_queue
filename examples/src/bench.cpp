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

struct Entry {
  int val;
  int order;
  std::uint8_t payload[64];

  bool operator<(const Entry& entry) const noexcept {
    return this->val < entry.val;
  }

  void print(void) const {
    std::cout << val << " " << order << std::endl;
  }
};


void priority_queue_test(const std::vector<Entry>& data) {
  std::priority_queue<Entry> queue;

  for (const auto& val : data) {
    queue.push(val);
  }

  while (!queue.empty()) {
    queue.pop();
  }
}

void stable_priority_queue_test(const std::vector<Entry>& data) {
  komori::stable_priority_queue<Entry> queue;

  for (const auto& val : data) {
    queue.push(val);
  }

  while (!queue.empty()) {
    queue.pop();
  }
}

void queue_set_test(const std::vector<Entry>& data) {
  std::map<Entry, std::queue<Entry>> queue;

  for (const auto& val : data) {
    queue[val].push(val);
  }

  while (!queue.empty()) {
    auto last = std::prev(queue.end(), 1);

    last->second.pop();
    if (last->second.empty()) {
      queue.erase(last);
    }
  }
}
}  // namespace

int main(void) {
  std::mt19937 mt;
  std::uniform_int_distribution<> dist(0, 1000);

  std::vector<Entry> data;
  for (size_t i = 0; i < kTestSize; ++i) {
    data.push_back(Entry {dist(mt), i});
  }

  auto pq_start = std::chrono::system_clock::now();
  priority_queue_test(data);
  auto pq_end = std::chrono::system_clock::now();
  double pq_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(pq_end - pq_start).count();
  std::cout << "PQ:   " << pq_elapsed << "ms" << std::endl;

  auto spq_start = std::chrono::system_clock::now();
  stable_priority_queue_test(data);
  auto spq_end = std::chrono::system_clock::now();
  double spq_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(spq_end - spq_start).count();
  std::cout << "SPQ:  " << spq_elapsed << "ms" << std::endl;

  auto qset_start = std::chrono::system_clock::now();
  queue_set_test(data);
  auto qset_end = std::chrono::system_clock::now();
  double qset_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(qset_end - qset_start).count();
  std::cout << "QSET: " << qset_elapsed << "ms" << std::endl;


  return EXIT_SUCCESS;
}
