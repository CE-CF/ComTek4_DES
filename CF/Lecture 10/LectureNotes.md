# Lecture 10
# Feasibility of dynamic prioríty

## DMA/RMA

RMA = Laveste eksekverings tid først (Højeste prioritet)
DMA = Laveste deadline først (Højeste prioritet)

"Hvis der er en feasible FP så vil der også være en  DMA"

## Dynamic priority

EDF = Earliest Deadline First

"Højeste prioritet til det job der har den tidligste absolutte deadline"

R(t): Requested i ```[0,t] = ((t-o)/T)*c ```
L(t): To be completed in [0,t] = ```((t-o-d)/T)*c```

O: Start at busy period

L(t) <= t : Ellers ville man misse deadline

Kan vi misse en deadline alligevel hvis L(t) <= t?
```
|
| 
|
|
|
|                             Deadline miss
|                                 \ /
+-------------------------|-----|--|--------> t
start                     r'<-->d' d
busy
period
```

For alle t>0
L(t) <= t : Ellers ville man misse deadline

for alle t>0
L(t) >= t : Så ville man altid holde deadlines

L_i(t) = ```((t-o_i-d_i)/T_i)*c_i <= ((t-d_i)/T_i)```

L_i(t) = ```sum(L_i(t)) = sum(1, n)((t-d_i)/T_i)*c_i <= t for alle t```

```L(t) <= sum(1,n) ((t-d_i)/T_i)*c_i``` <= ```sum(1, n) ((t-T_i)/T_i)*c_i = sum (t/T_i)-1+1)*'c_i = sum(1,n) (t/T_i)*c_i``` <= t for alle t

```t*sum(1,n)(c_i/T_i) <= t```

```t*U <= t <-> U < 1```

## IPC (Inter process communication)

- Timing
- Synchronization
- Mutual exclusion
- Randevouz 

+-----------------------+
|Fokus: Mutual exclusion|
+-----------------------+

Mutual exclusion is important with shared data to avoid corruption.

```c
void Task(){
	k_wait(t1);
	k_wait(mutex);
		CriticalFunction(); // Critical region
	k_signal(mutex);
	k_signal(t1);


void Task2(){
	k_wait(t2);
	k_wait(mutex);
		CriticalFunction(); // Critical region
	k_signal(mutex);
	k_signal(t2);
}
```

```c
Task1 		| 		Task2		// deadlock example
--------------------------
wait(s1)	|		wait(s2)
wait(s2)	|		wait(s1)
   :				   :	
signal(s2)	|		signal(s1)
signal(s1)	|		signal(s2)
```

## Priority Ceiling Protocol
```
Priority ceiling C(s) for some semaphore S by:
	
	C(S) = max{prio(T) | T may lock S}
```

- at some time t, some task T attempts to lock som emaphore S
- if prio(T) > max{C(S") | S" locked by tasks other than T} 
- - T runs

- else
- - T is suspended an no lock is put on S. Suspension is released when a semaphore is released and the criterion for suspension is no longer fulfilled

- if S itself is locked by some lower priority task T_L
- - T_L inherits the priority of T until it signals/releases T

## Completion time analysis

```C_i = c_i + sum(1, (i-1)) (C_i/T_j)*c_j + B_i``` Where B_i is the longes critical region that may block T_i

```
Task 1 
  :
Deler kritisk region med
  :
Task 2
```

```
+->Task 1
|
|
|  Task 2
|
|
+-> Task 3

Task 1 og Task 3 deler kritisk region
```

## Exercises:

__Opgave 1__
Taskset {T1,T2}

T1 = 4
c1 = 3
d1 = 4

T2 = 12
c2 = 2
d2 = 7

- Examine the taskset w.r.t. schedulability using the exact criterion for DMA scheduling. Is the taskset EDF schedulable.

DMA 

t = 1

ceil(1/4) = 1 
1 * c1 = 3 


__Opgave 2__

```
T1 = 40;
c1 = 10;
d1 = 20;

T2 = 60;
c2 = 20;
d2 = 30;

T3 = 80;
c3 = 20;
d3 = 80;

t = 0:0.01:50;
L = ceil((t-d1)/T1)*c1+ceil((t-d2)/T2)*c2+ceil((t-d3)/T3)*c3;

plot(t,t,t,L)
```
