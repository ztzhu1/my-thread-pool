#include <chrono>
#include <future>
#include <iostream>

#include <fmt/core.h>
#include <gtest/gtest.h>
#include <thread_pool.h>

void hello() {
    std::cout << "hello from thread " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

int return_int(int x) { return x; }

TEST(BasicTest, HelloTest) {
    ThreadPool pool(2);
    EXPECT_EQ(pool.num_thread(), 2);
    std::future<void> f1 = pool.submit(hello);
    std::future<void> f2 = pool.submit(hello);
    EXPECT_EQ(pool.num_running() + pool.num_waiting() + pool.num_done(), 2);
    f1.get();
    f2.get();
    EXPECT_EQ(pool.num_running() + pool.num_waiting() + pool.num_done(), 2);
}

TEST(BasicTest, ReturnIntTest1) {
    int num_thread = 8;
    ThreadPool pool(num_thread);
    EXPECT_EQ(pool.num_thread(), num_thread);
    std::vector<std::future<int>> futures(num_thread);
    for (int i = 0; i < num_thread; ++i) {
        futures[i] = pool.submit(return_int, i);
    }
    EXPECT_EQ(pool.num_running() + pool.num_waiting() + pool.num_done(), num_thread);
    for (int i = 0; i < num_thread; ++i) {
        EXPECT_EQ(futures[i].get(), i);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_EQ(pool.num_running() + pool.num_waiting(), 0);
    EXPECT_EQ(pool.num_done(), num_thread);
}

TEST(BasicTest, ReturnIntTest2) {
    int num_thread = 8;
    ThreadPool pool(num_thread);
    EXPECT_EQ(pool.num_thread(), num_thread);
    std::vector<std::future<int>> futures(num_thread * 2);
    for (int i = 0; i < num_thread * 2; ++i) {
        futures[i] = pool.submit(return_int, i);
    }
    EXPECT_EQ(pool.num_running() + pool.num_waiting() + pool.num_done(), num_thread * 2);
    for (int i = 0; i < num_thread * 2; ++i) {
        EXPECT_EQ(futures[i].get(), i);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    EXPECT_EQ(pool.num_running() + pool.num_waiting(), 0);
    EXPECT_EQ(pool.num_done(), num_thread * 2);
}