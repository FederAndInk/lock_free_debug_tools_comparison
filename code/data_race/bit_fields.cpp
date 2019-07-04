#include <iostream>
#include <thread>

struct T
{
  int a : 2, b : 2;
};

int main(int argc, char* argv[])
{
  size_t nb = 1;
  if (argc >= 2)
  {
    nb = std::stoul(argv[1]);
  }

  for (size_t i = 0; i < nb; i++)
  {
    T x = {0, 0};

    std::thread t1([&] { x.a = -1; });
    std::thread t2([&] { x.b = 0; });

    t1.join();
    t2.join();

    std::cout << "a: " << x.a << " b:" << x.b << "\n";
  }
  return 0;
}
