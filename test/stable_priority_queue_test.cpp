#include "komori/stable_priority_queue.hpp"
#include "gtest/gtest.h"

#include <functional>

using komori::stable_priority_queue;

namespace {
class NonCopyable {
public:
    NonCopyable(int x, int order) : x_(x), order_(order) {}
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable(NonCopyable&&) noexcept = default;
    NonCopyable& operator=(const NonCopyable&) = delete;
    NonCopyable& operator=(NonCopyable&&) noexcept = default;

    int get(void) const { return x_; }
    void set(int x) { x_ = x; }
    int order(void) const { return order_; }

    bool operator<(const NonCopyable& rhs) const { return x_ < rhs.x_; }
    bool operator==(const NonCopyable& rhs) const { return x_ == rhs.x_ && order_ == rhs.order_; }

private:
    int x_;
    int order_;
};

class Copyable {
public:
    Copyable(int x, int order) : x_(x), order_(order) {}
    Copyable(const Copyable&) = default;
    Copyable(Copyable&&) noexcept = default;
    Copyable& operator=(const Copyable&) = default;
    Copyable& operator=(Copyable&&) noexcept = default;

    int get(void) const { return x_; }
    void set(int x) { x_ = x; }
    int order(void) const { return order_; }

    bool operator<(const Copyable& rhs) const { return x_ < rhs.x_; }
    bool operator==(const Copyable& rhs) const { return x_ == rhs.x_ && order_ == rhs.order_; }

private:
    int x_;
    int order_;
};

template <typename T>
class AbsCompare {
public:
    AbsCompare(int z) : z_(z) {}
    AbsCompare(const AbsCompare&) = default;
    AbsCompare(AbsCompare&&) noexcept = default;
    AbsCompare& operator=(const AbsCompare&) = default;
    AbsCompare& operator=(AbsCompare&&) noexcept = default;

    bool operator()(const T& lhs, const T& rhs) const {
        return std::abs(lhs.get() - z_) < std::abs(rhs.get() - z_);
    }

private:
    int z_;
};

class StablePriorityQueueTest : public ::testing::Test {
protected:
    virtual void SetUp(void) {
        qnc1_.push(NonCopyable { 9, 0 });
        qnc1_.push(NonCopyable { 12, 1 });
        qnc1_.push(NonCopyable { 9, 2 });

        qnc2_.push(NonCopyable { 1, 0 });
        qnc2_.push(NonCopyable { 2, 1 });
        qnc2_.push(NonCopyable { 1, 2 });
    }

    stable_priority_queue<Copyable> qc0_ {};
    stable_priority_queue<Copyable, AbsCompare<Copyable>> qc1_{
        {
            Copyable { 9, 0 },
            Copyable { 12, 1 },
            Copyable { 9, 2 },
        },
        AbsCompare<Copyable>{10}
    };
    stable_priority_queue<Copyable> qc2_ {
        Copyable { 1, 0 },
        Copyable { 2, 1 },
        Copyable { 1, 2 }
    };

    stable_priority_queue<NonCopyable> qnc0_;
    stable_priority_queue<NonCopyable, AbsCompare<NonCopyable>> qnc1_{AbsCompare<NonCopyable>(10)};
    stable_priority_queue<NonCopyable> qnc2_;
};

TEST_F(StablePriorityQueueTest, Constructor) {
    EXPECT_TRUE(qc0_.empty());
    EXPECT_FALSE(qc1_.empty());
    EXPECT_FALSE(qc2_.empty());

    EXPECT_TRUE(qnc0_.empty());

    /* <Copy Constructor> */
    auto qc2_copy { qc2_ };
    ASSERT_EQ(qc2_copy.size(), 3);
    EXPECT_EQ(qc2_copy.top(), (Copyable { 2, 1 })); qc2_copy.pop();
    EXPECT_EQ(qc2_.size(), 3);
    EXPECT_EQ(qc2_copy.size(), 2);
    /* </Copy Constructor> */

    /* <Move Constructor> */
    auto qc2_move { std::move(qc2_) };
    EXPECT_TRUE(qc2_.empty());
    ASSERT_EQ(qc2_move.size(), 3);

    auto qnc2_move { std::move(qnc2_) };
    EXPECT_TRUE(qnc2_.empty());
    ASSERT_EQ(qnc2_move.size(), 3);
    /* </Move Constructor> */
}

TEST_F(StablePriorityQueueTest, AssignOperator) {
    /* <Copy Assign> */
    stable_priority_queue<Copyable> qc2_copy;
    qc2_copy = qc2_;

    ASSERT_EQ(qc2_copy.size(), 3);
    EXPECT_EQ(qc2_copy.top(), (Copyable { 2, 1 })); qc2_copy.pop();
    EXPECT_EQ(qc2_.size(), 3);
    EXPECT_EQ(qc2_copy.size(), 2);
    /* </Copy Assign> */

    /* <Move Assign> */
    stable_priority_queue<Copyable> qc2_move;
    qc2_move = std::move(qc2_);
    EXPECT_TRUE(qc2_.empty());
    EXPECT_EQ(qc2_move.size(), 3);

    stable_priority_queue<NonCopyable> qnc2_move;
    qnc2_move = std::move(qnc2_);
    EXPECT_TRUE(qnc2_.empty());
    EXPECT_EQ(qnc2_move.size(), 3);
    /* </Move Assign> */

    /* <initializer list Assign> */
    stable_priority_queue<Copyable> qc_tmp;
    qc_tmp = { Copyable { 10, 0 }, Copyable { 20, 1 }, Copyable { 10, 2 } };
    EXPECT_EQ(qc_tmp.size(), 3);
    EXPECT_EQ(qc_tmp.top(), (Copyable { 20, 1 }));
    /* </initializer list Assign> */
}

TEST_F(StablePriorityQueueTest, Empty) {
    EXPECT_TRUE(qc0_.empty());
    qc0_.push(Copyable { 1, 0 });
    EXPECT_FALSE(qc0_.empty());

    EXPECT_FALSE(qc1_.empty());
    EXPECT_FALSE(qc2_.empty());

    EXPECT_TRUE(qnc0_.empty());
    qnc0_.push(NonCopyable { 1, 0 });
    EXPECT_FALSE(qnc0_.empty());

    EXPECT_FALSE(qnc1_.empty());
    EXPECT_FALSE(qnc2_.empty());
}

TEST_F(StablePriorityQueueTest, Size) {
    EXPECT_EQ(qc0_.size(), 0);
    EXPECT_EQ(qc1_.size(), 3);
    EXPECT_EQ(qc2_.size(), 3);
    qc2_.push(Copyable { 3, 3 });
    EXPECT_EQ(qc2_.size(), 4);

    EXPECT_EQ(qnc0_.size(), 0);
    EXPECT_EQ(qnc1_.size(), 3);
    EXPECT_EQ(qnc2_.size(), 3);
    qnc2_.push(NonCopyable { 3, 3 });
    EXPECT_EQ(qnc2_.size(), 4);
}

TEST_F(StablePriorityQueueTest, Top) {
    EXPECT_EQ(qc2_.top(), (Copyable { 2, 1 }));
    EXPECT_EQ(qnc2_.top(), (NonCopyable { 2, 1 }));
}

TEST_F(StablePriorityQueueTest, Push) {
    qc2_.push(Copyable { 3, 3 });
    EXPECT_EQ(qc2_.top(), (Copyable { 3, 3 }));
    auto c = Copyable { 4, 4 };
    qc2_.push(c);
    EXPECT_EQ(qc2_.top(), (Copyable { 4, 4 }));

    qnc2_.push(NonCopyable { 3, 3 });
    EXPECT_EQ(qnc2_.top(), (NonCopyable { 3, 3 }));
}

TEST_F(StablePriorityQueueTest, Emplace) {
    qc2_.emplace(3, 3);
    EXPECT_EQ(qc2_.top(), (Copyable { 3, 3 }));

    qnc2_.emplace(3, 3);
    EXPECT_EQ(qnc2_.top(), (NonCopyable { 3, 3 }));
}

template <typename Key>
void popTest(void) {
    stable_priority_queue<Key> q;

    q.emplace(3, 0);
    q.emplace(2, 1);
    q.emplace(4, 2);
    q.emplace(3, 3);
    q.emplace(2, 4);

    EXPECT_EQ(q.top(), (Key { 4, 2 })); q.pop();
    EXPECT_EQ(q.top(), (Key { 3, 0 })); q.pop();

    q.emplace(4, 5);
    q.emplace(2, 6);
    q.emplace(3, 7);
    q.emplace(5, 8);

    EXPECT_EQ(q.top(), (Key { 5, 8 })); q.pop();
    EXPECT_EQ(q.top(), (Key { 4, 5 })); q.pop();
    EXPECT_EQ(q.top(), (Key { 3, 3 })); q.pop();
    EXPECT_EQ(q.top(), (Key { 3, 7 })); q.pop();
    EXPECT_EQ(q.top(), (Key { 2, 1 })); q.pop();
    EXPECT_EQ(q.top(), (Key { 2, 4 })); q.pop();
    EXPECT_EQ(q.top(), (Key { 2, 6 })); q.pop();
    EXPECT_TRUE(q.empty());
}

TEST_F(StablePriorityQueueTest, PopCopyable) {
    popTest<Copyable>();
    popTest<NonCopyable>();
}

TEST_F(StablePriorityQueueTest, Swap) {
    qc0_.swap(qc2_);
    EXPECT_EQ(qc0_.size(), 3);
    EXPECT_EQ(qc0_.top(), (Copyable { 2, 1 }));
    EXPECT_EQ(qc2_.size(), 0);
    swap(qc0_, qc2_);
    EXPECT_EQ(qc2_.size(), 3);
    EXPECT_EQ(qc0_.size(), 0);

    qnc0_.swap(qnc2_);
    EXPECT_EQ(qnc0_.size(), 3);
    EXPECT_EQ(qnc0_.top(), (NonCopyable { 2, 1 }));
    EXPECT_EQ(qnc2_.size(), 0);
    swap(qnc0_, qnc2_);
    EXPECT_EQ(qnc2_.size(), 3);
    EXPECT_EQ(qnc0_.size(), 0);
}

}  // namespace
