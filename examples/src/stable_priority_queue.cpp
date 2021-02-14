#include <queue>
#include <cstdlib>
#include <iostream>
#include "komori/stable_priority_queue.hpp"

struct Entry {
  int val;
  int order;

  bool operator<(const Entry& entry) const noexcept {
    return this->val < entry.val;
  }

  void print(void) const {
    std::cout << val << " " << order << std::endl;
  }
};


int main(void) {
  komori::stable_priority_queue<Entry> queue;

  queue.push(Entry { 3, 0 });
  queue.push(Entry { 2, 1 });
  queue.push(Entry { 4, 2 });
  queue.push(Entry { 3, 3 });
  queue.push(Entry { 2, 4 });

  for (int i = 0; i < 3; ++i) {
    auto top = queue.top();
    queue.pop();

    top.print();
  }

  queue.push(Entry { 4, 5 });
  queue.push(Entry { 2, 6 });
  queue.push(Entry { 3, 7 });

  while (!queue.empty()) {
    auto top = queue.top();
    queue.pop();

    top.print();
  }

  return EXIT_SUCCESS;
}
