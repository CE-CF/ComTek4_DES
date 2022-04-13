# Lecture 6

Course from University of Minnesota

``` 
mutex_enter(x)
|
|
mutex_leave(x)
```
Svarer til 
```
wait(x)
|
|
signal(x)
```

## Delaying calls

```
wait()
mutex_enter()

send_msg()			->		get_msg()
receive_element()	->		put_element()
```

```
Active 	-> 		Sleeping	->	Ready

wait()							signal()
mutex_enter()					mutex_leave()
receive_msg()					send_msg()

``` 

## Three different options for delaying calls

case 1:
- the resource is not free
- can wait forever or until something happens

case 2:
- the resource is not free
- does not wait

case 3: 
- the resource is not free
- the semaphore waits for x ticks before continueing. 

```c
// Case 1:
k_wait(semaphore, 0); // 0 means wait until possible ( Maybe forever)


//Case 2:
if (0<=k_wait(semaphore,-1)){ // -1 means that it will not wait if the semaphore is not free
	k_signal(semaphore);
}

//Case 3:
if (0<=k_wait(semaphore, 10)){ // It will only wait 10 ticks before continueing without an action.
	k_signal(semaphore);
}
```

## Interrupts


a UART running at 115200 bit/sec
uses 10 bit pr character sent with , inbetween. Can send 10000 characters per second. or 1 character per 100 microsecond.

Every time a process is interrupted, we need to see if it went as planned, this is to make sure that an interrupt waits indefinately.

this can be done by setting a semaphore wait for x second, so that if nothing happens for x secons, the process continues.

Case 1:
- The APS system for breaks
- Needs to receive RPM for every wheel
- Make a decission based on said RPM

```c
	void receiveRPM(){
		int wheel_1 = receive_msg(Wheel_1, 10);
		int wheel_2 =receive_msg(Wheel_2, 10);
		int wheel_3 = receive_msg(Wheel_3, 10);
		int wheel_4 = receive_msg(Wheel_4, 10);
	}

	if (wheel_x > wheel_y){
		loosen(Wheel_x, break_amount);
	}
```

## Critical region mutex example

Case:
- Entering one semaphore but signaling a different one when the task is over.

```c
wait(x);
signal(y);

mutex_enter(x);
mutex_leave(y); 
```
This results in the next process that needs the semaphore, will wait indefinately.

__Remember to check every task/process with the same priority at the beginning for debugging__

When sending information around, it is important to check if a task is succesfull, a lot of things can happen to make the task fail i.e. buffers too small.

