// Store Buffering (or Dekker's), with release-acquire pairs
// Question: can the two reads both see 0 in the same execution?
#include <atomic>
#include <iostream>
#include <shared_mutex>
#include <thread>
int main(int argc, char* argv[])
{
  size_t nb = 1;
  if (argc >= 2)
  {
    nb = std::stoul(argv[1]);
  }
  std::shared_mutex mut;

  for (size_t i = 0; i < nb; i++)
  {
    std::atomic<int> x{0};
    std::atomic<int> y{0};
    int              r1;
    int              r2;
    std::unique_lock lck(mut);
    std::thread      t1([&] {
      std::shared_lock lck1(mut);
      y.store(1, std::memory_order_seq_cst);
      r1 = x.load(std::memory_order_seq_cst);
    });
    std::thread      t2([&] {
      std::shared_lock lck2(mut);
      x.store(1, std::memory_order_seq_cst);
      r2 = y.load(std::memory_order_seq_cst);
    });
    lck.unlock();
    t1.join();
    t2.join();
    std::cout << "r1: " << r1 << ", r2: " << r2 << "\n";
  }
  return 0;
}
