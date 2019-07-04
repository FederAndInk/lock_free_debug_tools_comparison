#ifdef USE_VALGRIND
#include <valgrind/helgrind.h>

#include <valgrind/drd.h>
#endif

#include <atomic>
#include <iostream>
#include <optional>
#include <string>
#include <thread>

/**
 * no problem in this code, but "overkill" use of seq_cst
 */
int main(int argc, char* argv[])
{
  size_t nb = 1;
  if (argc >= 2)
  {
    nb = std::stoul(argv[1]);
  }

  for (size_t i = 0; i < nb; i++)
  {
    std::atomic<bool>  done{false};
    std::optional<int> payload;
    std::thread        t1([&] {
      // can also be `done.load(std::memory_order_seq_cst)` or just `done`
      while (!done.load())
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
      // can also be `done.store(true, std::memory_order_seq_cst)` or just `done = true`
      done.store(true);
    });
    t1.join();
    t2.join();
  }
  return 0;
}
