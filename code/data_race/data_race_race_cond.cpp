#include <functional>
#include <iostream>
#include <thread>

struct Account
{
  int balance{100};
};

void transferMoney(int amount, Account& from, Account& to)
{
  if (from.balance >= amount) // data race
  {
    from.balance -= amount; // data race
    // race condition: change in the invariant can be visible to other thread !
    to.balance += amount; // data race
  }
}

int main()
{
  Account account1;
  Account account2;

  std::thread thr1(transferMoney, 50, std::ref(account1), std::ref(account2));
  // race condition: output can change
  // depending on the execution order of thr1 and thr2
  std::thread thr2(transferMoney, 130, std::ref(account2), std::ref(account1));

  thr1.join();
  thr2.join();

  std::cout << "account1.balance: " << account1.balance << "\n";
  std::cout << "account2.balance: " << account2.balance << "\n";
}