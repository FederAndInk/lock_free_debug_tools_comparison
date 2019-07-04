#include <iostream>
#include <string>
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
  }
  return 0;
}
