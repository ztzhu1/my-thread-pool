#include <fmt/core.h>
#include <gtest/gtest.h>
#include <thread_pool.h>


TEST(BasicTest, BasicTest1) {
    ThreadPool pool(2);
    EXPECT_EQ(pool.num_thread(), 2);
}