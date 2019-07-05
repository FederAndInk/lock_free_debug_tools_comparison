# CppMem outputs

## Simple data race

CppMem shows us the data race (dr) between the non atomic write to x (b) and the non atomic read from x (c).

There is 2 possible execution:

- One in which we read from (rf) a\
![simple data race 1](../images/cppmem/simple_data_race1.png)
- One in which we read from (rf) b\
![simple data race 2](../images/cppmem/simple_data_race2.png)

## Pseudo notification

This time there is "only one" execution but it is not consistent and has data races:

![pseudo notification](../images/cppmem/pseudo_notif.png)

## Notification load relaxed

CppMem shows us the problem, there is no synchronize-with between write of done and read of done therefor we end up with data race on payload:

![notification load relaxed](../images/cppmem/notif_load_relaxed1.png)

![notification load relaxed](../images/cppmem/notif_load_relaxed2.png)

## Notification load relaxed in loop

This time we have no data race, I don't really know why CppMem does not report it:

We have one consistent execution:\
![notification load relaxed](../images/cppmem/notif_load_relaxed_loop1.png)

And one inconsistent execution:\
![notification load relaxed2](../images/cppmem/notif_load_relaxed_loop2.png)

## Notification load/store relaxed

Two inconsistent executions with data race and no synchronize-with:

![notification store/load relaxed1](../images/cppmem/notif_load_store_relaxed1.png)

![notification store/load relaxed2](../images/cppmem/notif_load_store_relaxed2.png)

## Data race atomic fix

No data race, 8 executions, 2 consistents:

First consistent, 0 is read no synchronize-with:\
![simple data race atomic fix](../images/cppmem/simple_data_race_fix1.png)

Second consistent, 2 is read with synchronize-with:\
![simple data race atomic fix](../images/cppmem/simple_data_race_fix2.png)

CppMem shows well that:

1. We can expect to read 0 or 2
2. Synchronize-with relation does not always appear. It is important for [notification for exemple](#Notification-fix-sequentially-consistent)

## Data race atomic fix relaxed

No data race, 4 executions, 2 consistents:

First consistent, 0 is read no synchronize-with:\
![simple data race relaxed atomic fix](../images/cppmem/simple_data_race_fix_relaxed1.png)

Second consistent, 2 is read no synchronize-with:\
![simple data race relaxed atomic fix](../images/cppmem/simple_data_race_fix_relaxed2.png)

CppMem shows us that there is no sequential consistency (sc) nor synchronize-with relations when using relaxed sementic.

## Notification fix sequentially consistent

No data race, 4 executions, 1 consistent:

![Notification fix sequencially consistent](../images/cppmem/notif_seq_cst.png)

CppMem shows us that `done` read(in g) from b with a synchronize-with relation and that it is sequentially consistent(sc)

## Notification fix acquire release sementic

No data race, 2 executions, 1 consistent:

![Notification fix release sementic](../images/cppmem/notif_acq_rel.png)

CppMem shows us that `done` read(in g) from b with a synchronize-with relation. without sequential consistency.

## store/load sequentially consistent

No data race, 384 executions, 6 consistent:

There is 3 possible results:

- 0, 1:\
![store/load seq cst1](../images/cppmem/store_load_seq_cst1.png)
- 1, 0:\
![store/load seq cst2](../images/cppmem/store_load_seq_cst2.png)
- 1, 1:\
![store/load seq cst3](../images/cppmem/store_load_seq_cst3.png)

## store/load acquire release sementic

No data race, 16 executions, 4 consistents:

This time there is 4 possible results:

- 0, 1:\
![store/load seq cst2](../images/cppmem/store_load_acq_rel2.png)
- 1, 0:\
![store/load seq cst3](../images/cppmem/store_load_acq_rel3.png)
- 1, 1:\
![store/load seq cst3](../images/cppmem/store_load_acq_rel4.png)
- and 0, 0:\
![store/load seq cst1](../images/cppmem/store_load_acq_rel1.png)

CppMem shows us that acquire release sementic isn't enough to prevent the weird result 0, 0 to appear.

## store/load relaxed

No data race, 16 executions, 4 consistents:

As expected there is 4 possible results:

- 0, 1:\
![store/load seq cst2](../images/cppmem/store_load_relaxed2.png)
- 1, 0:\
![store/load seq cst3](../images/cppmem/store_load_relaxed3.png)
- 1, 1:\
![store/load seq cst3](../images/cppmem/store_load_relaxed4.png)
- and 0, 0:\
![store/load seq cst1](../images/cppmem/store_load_relaxed1.png)
