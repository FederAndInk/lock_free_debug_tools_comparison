#ifdef USE_VALGRIND
#include <valgrind/helgrind.h>

#include <valgrind/drd.h>
#endif

#include <atomic>
#include <iostream>
#include <optional>
#include <string>
#include <thread>

int main()
{
  std::atomic<bool>  done{false};
  std::optional<int> payload;
  std::thread        t1([&] {
    while (!done.load(std::memory_order_relaxed)) // problem memory may be reordered
    {
    }
#ifdef USE_VALGRIND
    ANNOTATE_HAPPENS_AFTER(&done);
#endif
    // use the payload
    if (payload)
    {
      std::cout << *payload << "\n";
    }
  });
  std::thread t2([&] {
    payload = 19;
#ifdef USE_VALGRIND
    ANNOTATE_HAPPENS_BEFORE(&done);
#endif
    done.store(true, std::memory_order_relaxed); // problem memory may be reordered
  });
  t1.join();
  t2.join();

  return 0;
}
