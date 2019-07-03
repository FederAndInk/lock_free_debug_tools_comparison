#include <atomic>
#include <iostream>
#include <thread>

int main()
{
  std::atomic<int> x{0};

  std::thread t1([&] {
    //
    x.store(2);
  });
  std::thread t2([&] {
    //
    std::cout << x.load() << "\n";
  });

  t1.join();
  t2.join();
  return 0;
}
