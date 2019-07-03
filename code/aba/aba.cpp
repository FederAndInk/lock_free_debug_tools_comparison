#include <atomic>
#include <exception>
#include <iostream>
#include <random>
#include <shared_mutex>
#include <stdexcept>
#include <thread>

template <typename T>
struct node
{
  T     data;
  node* next;

  node(const T& data) : data(data), next(nullptr) {}
};

template <typename T>
class stack
{
  std::atomic<node<T>*> head{nullptr};

public:
  ~stack()
  {
    while (head.load(std::memory_order_relaxed))
    {
      pop();
    }
  }

  void push(const T& data)
  {
    node<T>* new_node = new node<T>(data);

    new_node->next = head.load(std::memory_order_relaxed);

    while (!head.compare_exchange_weak(new_node->next, new_node))
      ; // the body of the loop is empty
  }

  T pop()
  {
    node<T>* old_head = head.load();
    while (old_head && !head.compare_exchange_weak(old_head, old_head->next))
      ;
    if (old_head)
    {
      T tmp = std::move(old_head->data);
      // possible data race:
      delete old_head;
      return tmp;
    }
    throw std::out_of_range("stack is empty");
  }

  template <class T2>
  friend std::ostream& operator<<(std::ostream& out, stack<T2> const& s);
};

int main()
{
  for (size_t i = 0; i < 1000ul; i++)
  {
    stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);
    s.push(4);
    s.push(5);

    std::thread t1([&] {
      // #1 begin pop
      // imagine the thread is preempted after entering CAS
      // therefore we have a pointer the the head: old_head
      // and a pointer to next: old_head_next
      // #4 when comparing head with old_head, the pointer is the same
      // we then change head for old_head_next which point to nothing known
      s.pop();
    });
    std::thread t2([&] {
      // #2 then t2 execute all 3 instructions
      s.pop();
      s.pop();
      // #3 when pushing 5 the memory of old_head is reused
      s.push(5);
    });
    t1.join();
    t2.join();
    std::cout << s << "\n";
  }
}

template <class T>
std::ostream& operator<<(std::ostream& out, stack<T> const& s)
{
  node<T>* cur = s.head.load(std::memory_order_relaxed);
  while (cur)
  {
    out << cur->data;
    if (cur->next)
    {
      out << ", ";
    }
    cur = cur->next;
  }
  return out;
}
