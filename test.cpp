#include <atomic>
#include <iostream>

int main(int argc, char* argv[])
{
  std::atomic<int> i;
  std::cout << i.is_lock_free() << "\n";
}