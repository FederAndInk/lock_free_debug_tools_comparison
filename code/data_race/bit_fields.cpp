#include <iostream>
#include <thread>

int main()
{
  struct
  {
    int a : 2, b : 2;
  } x = {0, 0};
  std::thread t1([&] { x.a = -1; });
  std::thread t2([&] { x.b = 0; });

  t1.join();
  t2.join();

  std::cout << "a: " << x.a << " b:" << x.b << "\n";

  return 0;
}
