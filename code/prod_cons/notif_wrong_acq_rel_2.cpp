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
    int d = done.load(std::memory_order_acquire); // good acquire sementic, sync with #1
    while (!d)
    {
      // problem wrong ordering does not sync with #1
      d = done.load(std::memory_order_relaxed);
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
    done.store(true, std::memory_order_release); // #1
  });
  t1.join();
  t2.join();

  return 0;
}
