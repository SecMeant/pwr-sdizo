#pragma once
#include <chrono>
#include <utility>

namespace sdizo {
template <typename Function, typename... Args>
decltype(auto) measure(Function &&f, Args &&... args) noexcept {
  auto start = std::chrono::high_resolution_clock::now();
  f(std::forward<Args>(args)...);
  auto end = std::chrono::high_resolution_clock::now();

  return end - start;
}

template <typename Function, typename... Args>
std::chrono::nanoseconds measure_nano(Function &&f, Args &&... args) {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(
      measure(std::forward<Function>(f), std::forward<Args>(args)...));
}
} // namespace sdizo
