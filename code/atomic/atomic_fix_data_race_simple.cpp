#include <atomic>
#include <iostream>
#include <thread>

int main(int argc, char* argv[])
{
  size_t nb = 1;
  if (argc >= 2)
  {
    nb = std::stoul(argv[1]);
  }

  for (size_t i = 0; i < nb; i++)
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
  }
  return 0;
}
