#ifndef KOMORI_STABLE_PRIORITY_QUEUE_HPP_
#define KOMORI_STABLE_PRIORITY_QUEUE_HPP_

#include <functional>
#include <set>
#include <map>
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

    template <typename Key,
      typename Compare,
      bool Copyable = std::is_trivially_copy_constructible<Key>::value &&
        std::is_trivially_copy_constructible<Key>::value>
    class queue_base;

    template <typename Key,
        typename Compare>
    class queue_base<Key, Compare, false> {
    public:
      using value_type = Key;
      using reference = Key&;
      using const_reference = const Key&;
      using size_type = size_t;

      using ReversedCompare = detail::reverse_compare<Compare>;

      queue_base(void) : queue_base(Compare()) {};
      queue_base(const queue_base&) = default;
      queue_base(queue_base&&) = default;

      explicit queue_base(
          const Compare& comp)
          : data_(ReversedCompare { comp }, std::allocator<Key>()) {}
      queue_base(std::initializer_list<Key> init,
          const ReversedCompare& comp=ReversedCompare { Compare() })
          : data_(std::move(init), comp, std::allocator<Key>()) {}

      ~queue_base(void) = default;
      queue_base& operator=(const queue_base&) = default;
      queue_base& operator=(queue_base&&) = default;
      queue_base& operator=(std::initializer_list<Key> init) {
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

      void swap(queue_base& queue) {
        std::swap(data_, queue.data_);
      }

    private:
      std::multiset<Key, ReversedCompare> data_;
    };

    template <typename Key,
        typename Compare>
    class queue_base<Key, Compare, true> {
    public:
      using value_type = Key;
      using reference = Key&;
      using const_reference = const Key&;
      using size_type = size_t;

      using ReversedCompare = detail::reverse_compare<Compare>;

      queue_base(void) : queue_base(Compare()) {};
      queue_base(const queue_base&) = default;
      queue_base(queue_base&&) = default;

      queue_base(
          const Compare& comp)
          : data_(ReversedCompare { comp }) {}
      queue_base(std::initializer_list<Key> init,
          const ReversedCompare& comp=ReversedCompare { Compare() })
          : data_(std::move(init), comp) {}

      ~queue_base(void) = default;
      queue_base& operator=(const queue_base&) = default;
      queue_base& operator=(queue_base&&) = default;
      queue_base& operator=(std::initializer_list<Key> init) {
        data_ = std::move(init);
      }

      bool empty(void) const {
        return data_.empty();
      }

      size_t size(void) const {
        return data_.size();
      }
      
      const Key& top(void) const {
        return data_.begin()->second.front();
      }

      void push(const Key& key) {
        data_[key].push(key);
      }

      void push(Key&& key) {
        auto queue = data_[key];
        queue.push(std::move(key));
      }

      template <typename... ArgTypes>
      void emplace(ArgTypes&&... args) {
        push(Key(std::forward<ArgTypes>(args)...));
      }

      void pop(void) {
        data_.erase(data_.begin());
      }

      void swap(queue_base& queue) {
        std::swap(data_, queue.data_);
      }

    private:
      std::map<Key, std::queue<Key>, ReversedCompare> data_;
    };
  }  // namespace detail

  template <typename Key,
      typename Compare = std::less<Key>> 
  class stable_priority_queue : public detail::queue_base<Key, Compare> {};


  template <typename Key, typename Compare>
  void swap(stable_priority_queue<Key, Compare>& left,
      stable_priority_queue<Key, Compare>& right) {
    left.swap(right);
  }

}  // namespace komori

#endif   // KOMORI_STABLE_PRIORITY_QUEUE_HPP_
