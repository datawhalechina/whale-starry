#include <benchmark/benchmark.h>
#include <list>

// A helper function to fill a list with some values
void fill_list(std::list<int>& lst, int n) {
  for (int i = 0; i < n; ++i) {
    lst.push_back(i);
  }
}

// A benchmark function to test the prefix decrement
static void BM_PrefixDecrement(benchmark::State& state) {
  // Get the input size from the state
  int n = state.range(0);
  // Create a list with n elements
  std::list<int> lst;
  fill_list(lst, n);
  // Start the benchmark loop
  for (auto _ : state) {
    // Get an iterator to the end of the list
    auto it = lst.end();
    // Decrement the iterator n times using prefix decrement
    for (int i = 0; i < n; ++i) {
      --it;
    }
    // Prevent the compiler from optimizing away the loop
    benchmark::DoNotOptimize(it);
  }
}

// A benchmark function to test the postfix decrement
static void BM_PostfixDecrement(benchmark::State& state) {
  // Get the input size from the state
  int n = state.range(0);
  // Create a list with n elements
  std::list<int> lst;
  fill_list(lst, n);
  // Start the benchmark loop
  for (auto _ : state) {
    // Get an iterator to the end of the list
    auto it = lst.end();
    // Decrement the iterator n times using postfix decrement
    for (int i = 0; i < n; ++i) {
      it--;
    }
    // Prevent the compiler from optimizing away the loop
    benchmark::DoNotOptimize(it);
  }
}

BENCHMARK(BM_PrefixDecrement)->Range(8, 8 << 10);
BENCHMARK(BM_PostfixDecrement)->Range(8, 8 << 10);
BENCHMARK_MAIN();
