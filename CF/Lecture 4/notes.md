# Lecture 4

## How to execute several processes on one CPU

Important to maintain real-time / timing constraints.

2 different types of criteria to implement multithreading in a system:
Critical and non-critical.

## Fewer CPU's/Cores than tasks
need to economize with our CPU
NO  (!) buy waiting only passive waiting

## A task can

Be active
- Running on CPU
- Waiting to get CPU

Be sleeping (awaiting)

```
+-------------------------------+
|Active <-> Ready <-> Sleeping	|
+-------------------------------+
```

## Task priority model

- Lack of CPU (More tasks than CPUs)

- Need to prioritize

- Time, size, importance(pls define), CPU power requirement, disq requirements.

### In real time systems only one : TIME

- When do I need CPU

- Now ? Within 10 msec

- Or just prioritize execution
```
+------------------------------+
| Jens > Kurt > Mogens >= John |
+------------------------------+
```
- Highest priority wins!

## The goal
- Many independent task

- Communication between them (Not this lecture)

- Sharing unshareable devices (Not this lecture)

### Errors that can occur
CPU with not enough power can result in processes skipping the priority que

## ISR from last time
- Interrupt model - stack handling
- Prolog: Prog status is saved (Often in the stack)
- Epilog: Prog status back again (Often from the stack)


## AVR CPU MODEL
__Every task can be described by what is present in the CPU register__

The 32 general purpose registers
- Holds ALL info for running program

When interrupt hijack the CPU it must
- Save registers for the hijacked
- Run my ISR code
- Reestablish the hijacked
- Let him/her continue

The stack
- Used to push or pop registers

Memory array
- 

You can
- Push a register to the stack
- Pop a register back

## Task description
Code, data and a stack

Prioritize number between 0..99 ?!?
- A convention could be 0: Highest, 99: Lowest

State
- Running	- I Am running NOW == got the CPU
- Ready		- I would like to be in charge of the CPU
- Blocked	- Don't wake me

Transfer/task shit looks like ISR

Not Arduino's speciality

## Task shift ISR extended "Call me"
RETI
- Reestablish stack

Naked argument in void ```__attribute__(Naked)```
- means that it should have no prerequisites.

## Why is sleep more effective than other methods 
Sleep is inserted in a register, after each clock cycle the register is checked and decremented by one. If the register  

## Organisation of task descriptors

Active Queue (AQ)
- Tasks which is running or ready

Other Queues (Sleep queue, Semaphore queue)
- Tasks which waits to be active
	- External event (Interrupt)
	- Internal event (Other task which "kicks" you)

## Sharing of time/multiplexing

When there is fewer CPU's than running/ready tasks

Is named PSEUDO PARALLELLITY

__PSEUDO IS MULTITASKING__

Kernel or operating system divide CPU usage between task acc scheduling policy.

Doin this can risk having processes that never run due to CPU lacking power

__NEVER USE DELAY ON ARDUINO, YOU ARE USING OTHER PROCESSES RESOURCES__

## Task shift ISR extended "Call me"

If a process wakes up with higher/lower priority, it should be placed first in the queue.

This is done by Pushing the current process register to the stack, changning to the process first in the queue, and then that new process register is popped and executed.

## Round robin example

Using the above to reinsert task in active queue.

When dequeueing and reinserting the process, it should be inserted behind processes with the same priority but in front of the processes with lower priority.

## Exercises for today
Install the krnl.h library from link

```k_init(0,0,0)```
- First is how many tasks
- Second is number of message queues
- Number of tasks

```k_crt_tsk(0,0,0,0)```
- First is task used
- Second is array used for stack
- Third is stack size in bytes
- Fourth is stack initialized
- Function to be used as body for task

__It is a good idea to try with all the tasks having the same priority__

When you have several tasks running, you should have several stacks, one for each task.

```k_unused_stak("task")```
- Displays how much of the stack for the task is not used.

If there is less bytes in the stack than is needed, the task stops when the space runs out.

If variables are created that are not used, it does not affect the stack size because the kernel knows that the variable is not in use.

__REMBER TO CHECK HOW MUCH STACK IS USED__