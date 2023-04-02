#pragma once

#include <atomic>

#include "types.h"

class TaskIdAllocator {
  public:
    TaskIdAllocator() = default;
    TaskIdAllocator(const TaskIdAllocator &) = delete;
    TaskIdAllocator(TaskIdAllocator &&) = delete;
    TaskIdAllocator &operator=(const TaskIdAllocator &) = delete;
    TaskIdAllocator &operator=(TaskIdAllocator &&) = delete;
    task_id_t next() { return _next_id++; }

  private:
    std::atomic<task_id_t> _next_id{0};
};

extern TaskIdAllocator TASK_ID_ALLOCATOR;