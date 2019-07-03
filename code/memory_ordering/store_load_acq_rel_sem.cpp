// Store Buffering (or Dekker's), with release-acquire pairs
// Question: can the two reads both see 0 in the same execution?
#include <atomic>
#include <iostream>
#include <shared_mutex>
#include <thread>

int main()
{
  std::atomic<int>  x = 0;
  std::atomic<int>  y = 0;
  std::shared_mutex mut;
  std::unique_lock  lck(mut);
  int               r1;
  int               r2;
  std::thread       t1([&] {
    std::shared_lock lck1(mut);
    y.store(1, std::memory_order_release);
    r1 = x.load(std::memory_order_acquire);
  });
  std::thread       t2([&] {
    std::shared_lock lck2(mut);
    x.store(1, std::memory_order_release);
    r2 = y.load(std::memory_order_acquire);
  });
  lck.unlock();
  t1.join();
  t2.join();
  std::cout << "r1: " << r1 << ", r2: " << r2 << "\n";
  return 0;
}
