#pragma once

#include <cassert>

#include "task_id_allocator.h"
#include "types.h"

class Task {
  public:
    Task() = delete;
    explicit Task(task_t task_fn) : _task_fn(task_fn), _id(TASK_ID_ALLOCATOR.next()) {}
    Task(const Task &that) {
        _task_fn = that._task_fn;
        _id = that._id;
        _invoked = that._invoked;
    };
    Task(Task &&) = delete;

    void invoke() {
        assert(!_invoked);
        _invoked = true;
        _task_fn();
    }
    task_id_t id() { return _id; }

  private:
    task_t _task_fn;
    task_id_t _id;
    bool _invoked{false};
};