#include <iostream>
#include <shared_mutex>
#include <thread>

int main()
{
  for (size_t i = 0; i < 100; i++)
  {
    struct
    {
      int a : 2, b : 2;
    } x = {0, 0};
    std::shared_mutex mut;
    std::unique_lock  lckG(mut);
    std::thread       t1([&] {
      std::shared_lock lck(mut);
      x.a = -1;
    });
    std::thread       t2([&] {
      std::shared_lock lck(mut);
      x.b = 0;
    });
    lckG.unlock();

    t1.join();
    t2.join();

    std::cout << "a: " << x.a << " b:" << x.b << "\n";
    x.a = 1;
    x.b = 1;
  }

  return 0;
}
