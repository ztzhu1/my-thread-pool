#include "thread_pool.h"

#include <cassert>
#include <memory>

#include "task.h"

using namespace std;

ThreadPool::ThreadPool() : ThreadPool(thread::hardware_concurrency()) {}

ThreadPool::ThreadPool(num_thread_t num_thread) : _num_thread(num_thread) {
    _threads.reserve(num_thread);
    for_each([this](thread_id_t i) { _threads.emplace_back(thread(&ThreadPool::worker_loop, this, i)); });
}

ThreadPool::~ThreadPool() {
    {
        scoped_lock guard(_task_waiting_queue_mu, _task_running_queue_mu);
        _running = false;
    }
    _task_available_cv.notify_all();
    for_each([this](thread_id_t i) { _threads[i].join(); });
}

// ------ private ------
void ThreadPool::worker_loop(thread_id_t thread_id) {
    ThreadQuitReminder reminder(thread_id);
    while (_running) {
        unique_lock guard(_task_waiting_queue_mu);
        _task_available_cv.wait(guard, [this]() { return !_tasks_waiting.empty() || !_running; });
        if (!_running) {
            return;
        }
        Task task = _tasks_waiting.front();
        _tasks_waiting.pop();
        --_num_waiting;

        auto id = task.id();
        {
            lock_guard running_guard(_task_running_queue_mu);
            assert(_tasks_running.find(id) == _tasks_running.end());
            _tasks_running.insert(id);
            ++_num_running;
        }

        guard.unlock();

        task.invoke();

        {
            lock_guard running_guard(_task_running_queue_mu);
            _tasks_running.erase(id);
            --_num_running;
            ++_num_done;
        }
    }
}

void ThreadPool::for_each(const function<void(thread_id_t)> &fn) {
    for (thread_id_t i = 0; i < _num_thread; ++i) {
        fn(i);
    }
}

void ThreadPool::push_task(const Task &task) {
    {
        lock_guard guard(_task_waiting_queue_mu);
        _tasks_waiting.push(task);
        ++_num_waiting;
    }
    _task_available_cv.notify_one();
}