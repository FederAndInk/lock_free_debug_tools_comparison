#include <iostream>
#include <thread>

int main()
{
  int x = 0;

  std::thread t1([&] {
    // 1: write of x
    x = 2;
  });
  std::thread t2([&] {
    // 2: "at the same time" read of x
    std::cout << x << "\n";
  });

  t1.join();
  t2.join();
  return 0;
}
