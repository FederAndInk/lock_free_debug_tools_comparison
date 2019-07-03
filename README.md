# Lock free debug tools

Concurrency is hard but we can improve speed of programs with it.
For that it must be well designed. As for all programs it must be with the less bugs as possible, therefore we use and need tools to guide and correct us.

There are many technics to do concurrent programs and at some point we have to synchronize our threads to make them communicate.
These synchronizations can be effectively made with **locks** (eg: mutexes) or with **lock-free technics** allowing our threads to cooperate enabling maximum concurrency (some thread makes progress every step). This is also more robust, there is less dependencies with other threads (no locks).

Unfortunately lock-free programming has drawbacks and is more complicated than lock based counterpart. For instance to design lock-free algorithms you must be sure to not break invariants all the time as you can't exclude other threads when updating things. You must be aware of the order in which the data will be visible to other threads. And use atomics to avoid undefined behavior. I will discuss about these problems in the next sections.

***note**: you can consult [C++ Concurrency in action](#cpp_concurrency_in_action) by Anthony Williams*

## Problems in lock-free programming

- Data race
- Race Condition
- Memory reordering
- Livelock
- false sharing
- ABA' problem

***note** lock-free programs are not necessarily starvation free*

## Tools

These tools have been tested on:

- <a name="m1"></a>Machine 1(M1): ArchLinux `Linux 5.1.15-arch1-1-ARCH #1 SMP PREEMPT Tue Jun 25 04:49:39 UTC 2019 x86_64 GNU/Linux` (Intel i7-6700HQ (8 cores) 3.5GHz)
  - gcc version 9.1.0 (GCC)
  - clang version 8.0.0 (tags/RELEASE_800/final)

**Note**: Outputs may differs depending on compilers and options:

- Compilers:
  - gcc
  - clang
- Standard library:
  - libstdc++ (gnu)
  - libc++    (llvm)
- Options:
  - `-g`: debug info (file name, symbols, line:column number)

The tools will be tested with different options and differences will be reported.

The tools will be tested with small C++11 programs (multiple times for dynamic tools), focused on lock-free:

- **Programs with bugs**:
  - Data races
    - [Simple data race](./code/data_race/data_race_simple.cpp)
    - [Simple data race CppMem](./code/data_race/data_race_simple.cppmem)
    - [Data race on std::string](./code/data_race/data_race_string.cpp)
    - [Data race trying to notify through boolean flag](./code/data_race/pseudo_notif.cpp)
    - [Data race trying to notify through boolean flag CppMem](./code/data_race/pseudo_notif.cppmem)
    - [Data race on complex type std::map](./code/data_race/race_map.cpp)
    - [Data race and race condition](./code/data_race/data_race_race_cond.cpp)
    - [Data race on object destruction](./code/data_race/race_destruction.cpp)
    - [Data race on small std::string destruction](./code/data_race/race_destruction_SSO.cpp)
    - [Data race on std::string destruction](./code/data_race/race_destruction_string.cpp)
  - ABA':
  - Wrong synchronized producer consumer:
    - TODO: add 1prod-2cons... rel_acq
- **Correct programs**:
  - Producer consumer
  - ABA' fixed
  - Memory ordering

### Valgrind: [DRD](http://valgrind.org/docs/manual/drd-manual.html)

|                    |                               |
| ------------------ | ----------------------------- |
| [M1](#m1) Version: | valgrind-3.14.0               |
| Type:              | dynamic on-the-fly            |
| Plateform:         | Linux/MacOS/Android 32/64bits |

#### Pricipal usage

DRD can detect data races, improper use of POSIX threads, false sharing, deadlock and monitor lock contention. But can't detect wrong lock order.\
Also DRD support detached threads.

It is faster than Helgring but can be less precise.

By default DRD [does not check for local variable](#drd_stack_check) (stack).

1. compile normally (with `-g` for debug info)
2. run: `valgrind --tool=drd program`

***note**: you can colorize the output: `pip install colour-valgrind`*

#### [Options](http://valgrind.org/docs/manual/drd-manual.html#drd-manual.options)

<a name="drd_stack_check"></a>
`--check-stack-var=<yes|no> [default: no]`\
Controls whether DRD detects data races on stack variables.

`--segment-merging=<yes|no> [default: yes]`\
Segment merging is an algorithm to limit memory usage. Disabling segment merging may improve the accuracy displayed in race reports but can also trigger an out of memory error.

`--suppressions=<suppressions-file>`\
Specify user suppressions file.

`--gen-suppressions=all`\
Generate suppression for detected problem. Useful to rapidly suppress problem.

Check other [options](http://valgrind.org/docs/manual/drd-manual.html#drd-manual.options)

#### Tests

// TODO: talk about suppressions (for atomic especially)
// TODO: talk about user annotations (memory order): but we can get wrong :/ so maybe we can wrap them for atomic load/store and check memory_order ?

### Valgrind: [Helgrind](http://valgrind.org/docs/manual/hg-manual.html)

By default Helgrind [checks for local variable](#hg_stack_check) (stack).

#### [Options](http://valgrind.org/docs/manual/hg-manual.html#hg-manual.options)

<a name="hg_stack_check"></a>
`--check-stack-refs=no|yes [default: yes]`\
This flag enables you to skip checking for accesses to thread stacks (local variables). This can improve performance, but comes at the cost of missing races on stack-allocated data.

Check other [options](http://valgrind.org/docs/manual/hg-manual.html#hg-manual.options)

### [ThreadSanitizer (tsan)](https://github.com/google/sanitizers/wiki/ThreadSanitizerCppManual)

ThreadSanitizer as for other sanitiers is integrated in Gcc and Clang

### Intel Inspector ([Free version](https://software.intel.com/en-us/inspector/choose-download#inspector))

### [CppMem](http://svr-pes20-cppmem.cl.cam.ac.uk/cppmem/)

### Other tools

There are some other tools not covered here with given reason:

- [Other sanitizers](https://github.com/google/sanitizers/wiki): these must be useful to catch other bugs not related to concurrency, check it out.
- [Relacy race detector](http://www.1024cores.net/home/relacy-race-detector):
  The code must be instrumented (unit test) and compiled C++03 or C++11 but `std::*` must be changed by `rl::*` [see exemple](./code/)
- [CDSChecker](http://plrg.eecs.uci.edu/software_page/42-2/): poor support of C++, old.
- [IFRit](https://github.com/blucia0a/IFRit): too old, based on old llvm/clang

### Notes

- To initialize something in concurrency (double checked locking) think about using [static local variable](https://en.cppreference.com/w/cpp/language/storage_duration#Static_local_variables)
- **Do not use [volatile](https://github.com/google/sanitizers/wiki/ThreadSanitizerAboutRaces#volatile)** for synchronization!

## Summary

(SEE: if Intel inspector support lockfree)
Most tools have poor support of lock-free programming.\
CppMem can shows us what is going on in lock free but has serious limitations (main function only, no struct, tiny subset of pseudo C supported).\
ThreadSanitizer can work well in lock-free context but has [not been seriously tested on that specific area](#tsan_faq "TSan supports [...] C++ `<atomic>` operations are supported with llvm libc++ (not very throughly tested, though).") and there are no new paper describing the new Thread Sanitizer (v2). It will be necessary to dig and test real lock free programs with TSan.

// TODO: don't had time to test with more than 2 threads: eg: multi conso/prod, ...

## References

- [CppReference](https://cppreference.com/) C++ reference close to the standard but more digest.
- <a name="cpp_concurrency_in_action"></a>Book *C++ concurrency in action* by Anthony Williams good book about concurrency and lock-free
- Conference [CppCon 2014 Herb Sutter "Lock-Free Programming (or, Juggling Razor Blades)"](https://www.youtube.com/watch?v=c1gO9aB9nbs)
- Papers
  - [Memory Barriers: a Hardware View for Software Hackers](https://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.170.3279), Paul E. Mckenney, 10.1.1.170.3279
  - [ThreadSanitizer – data race detection in practice](https://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.308.3963), Konstantin Serebryany and Timur Iskhodzhanov, 10.1.1.308.3963
- Blogs
  - [ModernsCpp](https://www.modernescpp.com/)
  - [Jeff Preshing](https://preshing.com/)
  - [Concurrency Freaks](http://concurrencyfreaks.blogspot.com/)
- [Awesome lock-free resources](https://github.com/rigtorp/awesome-lockfree)
- <a name="tsan_faq"></a>[TSan supports [...] C++ `<atomic>` operations are supported with llvm libc++ (not very throughly tested, though).](https://github.com/google/sanitizers/wiki/ThreadSanitizerCppManual#faq)
