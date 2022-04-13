# Lecture 5

## Semaphores
### Problems
- Who is running
- Enough CPU power?

#### Functions
```c
void tgeneric(){
	k_eat_msec(pRun-> * 200);
	k_sleep(1000);
}
```

```c
k_eat_msec(pRun-> * 200);```
- Eats CPU time like an algorithm
```

### Who is running
Need to intercept task shift in krnl
- Beware of hidden task(dummy w/ lowest priority) eat excessive CPU

Krnl has a break out function

```c
	void k_breakout()
```
- which is called from every task shift from dispatcher

- Defined as weak so compiler will take yours instead of mine. It inserts a strong function and throws away a weak.

### Who is running II
task are numbered 0(dummy),1,2,3 in the order they are created

Idea let map task numbers
- 0,1,2,3,4,5 to digital 8,9,10,11,12,13

D8 high when task 0 (dummy) running - and D9-13 low

D9 High when task 1 running

On UNO
- D8 is PORTB bit 0
- D9 is PORTB bit 1 

```c
Set PORTB bit 2

PORTB = (1<<2);

do also set bit 0,1,2,3,4... to 
```
An example of using k_breakout to setup output ports

```c
	void k_breakout()
	{
		PORTB = (1<<pRun->nr);
	}
```

### Priority Oscilator
Two task with same priority switches between control of the CPU every operation (1ms in this  case).


## Sharing - a problem - what to expect
Always try to set up a thesis before just running

All three task same priority

Preemptive schedule - 1 kHz

Task shift takes place without noticeing the tasks

Task are printing one long unique line each

Might be interrupted during printing

No access control on Serial.print :-/

So print might be interleaved:

123abABCcd456 etc or similar pattern (Refers to below):
- Where the slow function prints integers, and to similar functions prints lower and UPPERCASE.

Issues about baudrate - we might change it

## Very shor duration ( << Kernel tick speed)

```c
	disable_intr();
	atmoisk_kode();
	enable_intr();
```
Very effective - nearly no overhead

Not useful in multi CPU systems

If atomic code duration is too long the ... crap

In real life duration less than 1/4 period for the highest frequency of interrupts in the system.

But open for negotiations in a specific case.

## Critical region

Task 1:
```c
	wait(gateToData);
	manipulatedata();
	signal(gateToData);
```
Task 2:
```c
	wait(gateToData);
	SeomethingElse();
	signal(gateToData);
```
Semaphore gateToData is initialized to "1"

Real code:
```c
	void t3(void){ 
		k_wait(semWC, 0); // semWC is the semaphore (Key) that you wait for,  0 is the time the wait function can wait for. 0 means wait indefinately, -1 means that i don't want to wait and any positive number indicates the number of ms i would like to wait.
		Serial.print("ABCDEFGHIJKLMN\n");
		k_signal(senWC);
	}

	void setup(){
		k_init(3, 1, 0);
		senWC = k_crt_sem(1, 10); // 1 == 1 key to hang on the door
		k_crt_task(t3, 11, 200);
		k_start(1);
	}
```

### Semaphores in k function
```c
	semaphore = k_crt_sem(0,10);
```
- ```-->(0,10)``` 
- 
- The left value stands for how many processes that can access the information !(keys)!

- ```(0,10)<--```
- 
- Den højre værdi står for


### Interleaving in multitasking environment


```
I = 0;

task1: I+1;

task2: I-1;

```
Without semaphores problems with these task sharing a variable can happen.
```
1. mov ax, I 		a. mov ax, I
2. inc ax			b. decr ac
3. mov I, ax		c. mov I, ax

runs on CPU

1 2 3 a b c | I = 0
a b c 1 2 3 | I = 0
1 2 a b c 3 | I = 1
a b 1 2 3 c | I = -1
```

Semaphore value can go into negative, showing how many processes are in queue.

In critical regions semaphores can be initialized to be 1, 2 or 3.

```c
	k_set_sem_timer(loopSem,20);
```
- The timer kicks the active process out and gives a new process access every 20 ms.
- it helps to enure that a new process has the semaphore every 20 ms.

