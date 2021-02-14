#ifndef KOMORI_STABLE_PRIORITY_QUEUE_HPP_
#define KOMORI_STABLE_PRIORITY_QUEUE_HPP_

#include <functional>
#include <set>
#include <memory>


namespace komori {
  namespace detail {
    template <typename Comp>
    struct reverse_compare {
      using first_argument_type = typename Comp::first_argument_type;
      using second_argument_type = typename Comp::second_argument_type;

      bool operator()(const first_argument_type& x, const first_argument_type& y) const {
        return comp(y, x);
      }

      const Comp& comp;
    };
  }  // namespace detail

  template <typename Key,
      typename Compare = std::less<Key>,
      typename Allocator = std::allocator<Key>>
  class stable_priority_queue {
  public:
    using value_type = Key;
    using reference = Key&;
    using const_reference = const Key&;
    using size_type = size_t;

    using ReversedCompare = detail::reverse_compare<Compare>;

    stable_priority_queue(const stable_priority_queue&) = default;
    stable_priority_queue(stable_priority_queue&&) = default;

    explicit stable_priority_queue(
        const Allocator& alloc=Allocator())
        : data_(ReversedCompare { Compare() }, alloc) {}
    stable_priority_queue(
        const Compare& comp,
        const Allocator& alloc)
        : data_(ReversedCompare { comp }, alloc) {}
    stable_priority_queue(std::initializer_list<Key> init,
        const ReversedCompare& comp=ReversedCompare { Compare() },
        const Allocator& alloc=Allocator())
        : data_(std::move(init), comp, alloc) {}
    stable_priority_queue(std::initializer_list<Key> init,
        const Allocator& alloc)
        : data_(std::move(init), ReversedCompare { Compare() }, alloc) {}

    ~stable_priority_queue(void) = default;
    stable_priority_queue& operator=(const stable_priority_queue&) = default;
    stable_priority_queue& operator=(stable_priority_queue&&) = default;
    stable_priority_queue& operator=(std::initializer_list<Key> init) {
      data_ = std::move(init);
    }

    bool empty(void) const {
      return data_.empty();
    }

    size_t size(void) const {
      return data_.size();
    }
    
    const Key& top(void) const {
      return *data_.begin();
    }

    void push(const Key& key) {
      data_.insert(key);
    }

    void push(Key&& key) {
      data_.insert(std::move(key));
    }

    template <typename... ArgTypes>
    void emplace(ArgTypes&&... args) {
      data_.emplace(std::forward<ArgTypes>(args)...);
    }

    void pop(void) {
      data_.erase(data_.begin());
    }

    void swap(stable_priority_queue& queue) {
      std::swap(data_, queue.data_);
    }

  private:
    std::multiset<Key, ReversedCompare, Allocator> data_;
  };

  template <typename Key, typename Compare, typename Allocator>
  void swap(stable_priority_queue<Key, Compare, Allocator>& left,
      stable_priority_queue<Key, Compare, Allocator>& right) {
    left.swap(right);
  }

}  // namespace komori

#endif   // KOMORI_STABLE_PRIORITY_QUEUE_HPP_
