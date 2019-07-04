#include <iostream>
#include <string>
#include <thread>

std::string x;

int main(int argc, char* argv[])
{
  size_t nb = 1;
  if (argc >= 2)
  {
    nb = std::stoul(argv[1]);
  }

  for (size_t i = 0; i < nb; i++)
  {
    std::thread t1([] { //
      x = "long enough string to heap allocate";
    });
    std::thread t2([] { //
      std::cout << x << "\n";
    });

    t1.join();
    t2.join();
  }
  return 0;
}
