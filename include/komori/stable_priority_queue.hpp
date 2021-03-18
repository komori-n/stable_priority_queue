#ifndef KOMORI_STABLE_PRIORITY_QUEUE_HPP_
#define KOMORI_STABLE_PRIORITY_QUEUE_HPP_

#include <functional>
#include <map>
#include <memory>
#include <queue>
#include <set>

namespace komori {
namespace detail {
template <typename Comp>
struct reverse_compare {
  template <typename T, typename U>
  bool operator()(const T& x, const U& y) const {
    return comp(y, x);
  }

  Comp comp;
};
}  // namespace detail

template <typename Key, typename Compare = std::less<Key>>
class noncopyable_queue {
 public:
  using value_type = Key;
  using reference = Key&;
  using const_reference = const Key&;
  using size_type = size_t;

  using ReversedCompare = detail::reverse_compare<Compare>;

  noncopyable_queue(void) : noncopyable_queue(Compare()){};
  noncopyable_queue(const noncopyable_queue&) = default;
  noncopyable_queue(noncopyable_queue&&) noexcept = default;

  explicit noncopyable_queue(const Compare& comp)
      : data_(ReversedCompare{comp}, std::allocator<Key>()) {}
  template <typename ParamDummy = Key,
            std::enable_if_t<std::is_copy_constructible<ParamDummy>::value>>
  noncopyable_queue(std::initializer_list<Key> init,
                    const Compare& comp = Compare())
      : data_(std::move(init), ReversedCompare{comp}, std::allocator<Key>()) {}

  ~noncopyable_queue(void) = default;
  noncopyable_queue& operator=(const noncopyable_queue&) = default;
  noncopyable_queue& operator=(noncopyable_queue&&) noexcept = default;
  noncopyable_queue& operator=(std::initializer_list<Key> init) {
    data_ = std::move(init);
  }

  bool empty(void) const { return data_.empty(); }

  size_t size(void) const { return data_.size(); }

  const Key& top(void) const { return *data_.begin(); }

  void push(const Key& key) { data_.insert(key); }

  void push(Key&& key) { data_.insert(std::move(key)); }

  template <typename... ArgTypes>
  void emplace(ArgTypes&&... args) {
    data_.emplace(std::forward<ArgTypes>(args)...);
  }

  void pop(void) { data_.erase(data_.begin()); }

  void swap(noncopyable_queue& queue) { std::swap(data_, queue.data_); }

 private:
  std::multiset<Key, ReversedCompare> data_;
};

template <typename Key, typename Compare = std::less<Key>>
class copyable_queue {
 public:
  using value_type = Key;
  using reference = Key&;
  using const_reference = const Key&;
  using size_type = size_t;

  using ReversedCompare = detail::reverse_compare<Compare>;

  copyable_queue(void) : copyable_queue(Compare()){};
  copyable_queue(const copyable_queue&) = default;
  copyable_queue(copyable_queue&&) noexcept = default;

  explicit copyable_queue(const Compare& comp) : data_(ReversedCompare{comp}) {}
  copyable_queue(std::initializer_list<Key> init,
                 const Compare& comp = Compare())
      : data_(ReversedCompare{comp}) {
    for (auto&& v : std::move(init)) {
      push(std::move(v));
    }
  }

  ~copyable_queue(void) = default;
  copyable_queue& operator=(const copyable_queue&) = default;
  copyable_queue& operator=(copyable_queue&&) noexcept = default;
  copyable_queue& operator=(std::initializer_list<Key> init) {
    for (auto&& v : std::move(init)) {
      push(std::move(v));
    }
    return *this;
  }

  bool empty(void) const { return data_.empty(); }

  size_t size(void) const {
    size_t size_tmp{0};
    for (const auto& kv : data_) {
      size_tmp += kv.second.size();
    }
    return size_tmp;
  }

  const Key& top(void) const { return data_.begin()->second.front(); }

  void push(const Key& key) { data_[key].push(key); }

  void push(Key&& key) {
    auto& top_queue = data_[key];
    top_queue.push(std::move(key));
  }

  template <typename... ArgTypes>
  void emplace(ArgTypes&&... args) {
    push(Key(std::forward<ArgTypes>(args)...));
  }

  void pop(void) {
    auto top_pair = data_.begin();

    top_pair->second.pop();
    if (top_pair->second.empty()) {
      data_.erase(top_pair);
    }
  }

  void swap(copyable_queue& queue) { std::swap(data_, queue.data_); }

 private:
  std::map<Key, std::queue<Key>, ReversedCompare> data_;
};

template <typename Key, typename Compare = std::less<Key>>
using stable_priority_queue =
    typename std::conditional<std::is_copy_constructible<Key>::value &&
                                  std::is_copy_assignable<Key>::value,
                              copyable_queue<Key, Compare>,
                              noncopyable_queue<Key, Compare>>::type;

template <typename Key, typename Compare>
void swap(stable_priority_queue<Key, Compare>& left,
          stable_priority_queue<Key, Compare>& right) {
  left.swap(right);
}

}  // namespace komori

#endif  // KOMORI_STABLE_PRIORITY_QUEUE_HPP_
