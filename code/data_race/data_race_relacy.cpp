#include <cstdlib>
#include <relacy/relacy_std.hpp>
#include <string>

// template parameter '2' is number of threads
struct race_test : rl::test_suite<race_test, 2>
{
  std::atomic<int> a;
  rl::var<int>     x;

  // executed in single thread before main thread function
  void before()
  {
    a($) = 0;
    x($) = 0;
  }

  // main thread function
  void thread(unsigned thread_index)
  {
    if (0 == thread_index)
    {
      x($) = 1;
      a.store(1, rl::memory_order_relaxed);
    }
    else
    {
      if (1 == a.load(rl::memory_order_relaxed))
        x($) = 2;
    }
  }

  // executed in single thread after main thread function
  void after() {}

  // executed in single thread after every 'visible' action in main threads
  // disallowed to modify any state
  void invariant() {}
};

int main(int argc, char* argv[])
{
  size_t nb = 1;
  if (argc >= 2)
  {
    nb = std::atoi(argv[1]);
  }

  for (size_t i = 0; i < nb; i++)
  {
    rl::simulate<race_test>();
  }
}