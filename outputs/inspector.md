# Intel Inspector outputs

## Simple data race

Intel Inspector detect the simple data race, showing us the conflict load and store.

Also it output a code snippet.

![simple_data_race](../images/intel_inspector/simple_data_race.gcc.png)

## String data race

![string_data_race](../images/intel_inspector/string_data_race.gcc.png)

## Pseudo notification

Data race on string:\
![pseudo_notify1](../images/intel_inspector/pseudo_notify1.gcc.png)

Data race on `done`:\
![pseudo_notify1](../images/intel_inspector/pseudo_notify2.gcc.png)

## std::map data race

With 6 errors, it detects the race:\
![map](../images/intel_inspector/map_race.gcc.png)

## Data race vs race condition

It detect the multiple data races:

![Data race vs race condition](../images/intel_inspector/data_race_race_cond1.gcc.png)

![Data race vs race condition](../images/intel_inspector/data_race_race_cond2.gcc.png)

## Data race on object destruction

It does not detect anything, this is not the purpose of the data race detector in intel inspector.

## Data race on small string destruction

Same as [above](#Data-race-on-object-destruction).

## Data race on string destruction

Same as [above](#Data-race-on-object-destruction).

## ABA

Intel Inspector does not detect the ABA problem exposed in this [code](../code/aba/aba.cpp).\
Even after looping 1000 times multiple times.

## ABA synchronized

It does not detect the aba problem even synchronized on gcc.
But it detects it on clang:

![aba sync1 clang](../images/intel_inspector/aba_sync1.clang.png)\
![aba sync2 clang](../images/intel_inspector/aba_sync2.clang.png)\
![aba sync3 clang](../images/intel_inspector/aba_sync3.clang.png)

We see the delete conflicting with the while in the `pop` function.

## Notification load relaxed

Intel inspector detect a data race but does not understand nor atomics in gcc/clang nor memory ordering constraints

## Notification load relaxed in loop

Same as [above](#Notification-load-relaxed).

## Notification load/store relaxed

Same as [above](#Notification-load-relaxed).

## Data race atomic fix

Intel inspector detect a data race but does not understand nor atomics in gcc nor memory ordering constraints\
so it output error on atomic:
![atomic read/write gcc](../images/intel_inspector/atomic_data_race_fix.gcc.png)

With clang it is fine.

## Data race atomic fix relaxed

As [expected](#Data-race-atomic-fix) with gcc it outputs errors an atomic.
And with clang it also provoc errors on atomics.

## Notification fix

As expected it does not understand memory ordering constraints.

## ABA fix

