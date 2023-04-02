#pragma once

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <set>
#include <thread>
#include <type_traits>
#include <vector>

#include <fmt/color.h>

#include "task.h"
#include "types.h"

#define TASK_TYPE_TEMPLATE                                                                                             \
    template <typename F, typename... Args, typename R = std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>>

class ThreadPool {
  public:
    ThreadPool();
    explicit ThreadPool(num_thread_t num_thread);
    ~ThreadPool();
    ThreadPool(const ThreadPool &) = delete;
    ThreadPool(ThreadPool &&) = delete;
    ThreadPool &operator=(const ThreadPool &) = delete;
    ThreadPool &operator=(ThreadPool &&) = delete;
    num_thread_t num_thread() { return _num_thread; }
    bool running() { return _running; }
    size_t num_running() { return _num_running; }
    size_t num_waiting() { return _num_waiting; }

    TASK_TYPE_TEMPLATE
    [[nodiscard]] std::future<R> submit(F &&fn, Args &&...args);

  private:
    class ThreadQuitReminder {
      public:
        ThreadQuitReminder(thread_id_t id) : _id(id) {}
        ~ThreadQuitReminder() { fmt::print("\x1b[1;35mthread {} quitted.\x1b[0m\n", _id); }

      private:
        thread_id_t _id;
    };

    void worker_loop(thread_id_t thread_id);
    void for_each(std::function<void(thread_id_t)> fn);
    void push_task(const Task &task);

    num_thread_t _num_thread;
    std::vector<std::thread> _threads{};
    std::atomic<bool> _running{true};

    std::queue<Task> _tasks_waiting{};
    std::set<task_id_t> _tasks_running{};
    std::atomic<size_t> _num_waiting{0};
    std::atomic<size_t> _num_running{0};

    std::condition_variable _task_available_cv{};
    std::mutex _task_waiting_mu{};
    std::mutex _task_running_mu{};
};