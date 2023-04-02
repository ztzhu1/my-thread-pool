#pragma once

#include <cstdint>
#include <functional>
#include <thread>

using num_thread_t = std::invoke_result_t<decltype(std::thread::hardware_concurrency)>;
using thread_id_t = num_thread_t;

using task_t = std::function<void()>;
using task_id_t = uint64_t;