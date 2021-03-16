# stable_priority_queue

[![Test](https://github.com/komori-n/stable_priority_queue/actions/workflows/test.yml/badge.svg)](https://github.com/komori-n/stable_priority_queue/actions/workflows/test.yml)
![examples](https://github.com/komori-n/stable_priority_queue/workflows/examples/badge.svg)

priority queue preserving insertion order


## Getting Started

```sh
git clone https://github.com/komori-n/stable_priority_queue.git
```

You can use `komori::stable_priority_queue<T>` by including `komori/stable_priority_queue.hpp`.
Usage is the same as `std::priority_queue<T>`.

```cpp
#include "komori/stable_priority_queue.hpp"

komori::stable_priority_queue<int> queue;
queue.push(3);
queue.push(3);
queue.push(4);

while (!queue.empty()) {
  std::cout << queue.top() << std::endl;
  queue.pop();
}
// => 4, 3, 3
```

## License

Licensed under MIT license
