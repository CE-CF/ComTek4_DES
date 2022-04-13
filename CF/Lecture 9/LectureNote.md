# Lecture 9
# Scheduling in embedded systems

## Definition/Characterization
- System context is mainly technical

- Context actors are incoherent

- Non-functional/quantative requirements (time, reliability)

- Technological limitations (cost, size power, CPU, memort, comm)

- Design involves both HW and SW

- Distributed systems comprise multiple processors on different locations connected by thin communication channels.

### Adminstrative
- Functional requirements (Use-case)

- Non-technical (Problem domain)

- Non-technical limitations

### Embedded
- Non-Functioonal requiremtnes (performance)

- Technical problen

- technical limitations

### Analysis and design of embedded systems
- COBRA (Concurrent Object Based RT Analysis)

- CODARTS (Concurrent Object Based Design and Analysis of RT systems)

```
+-----------+	+-------+
|	   |__| |---|		| Incoherent actor
|			|	+-------+
|			|
+-----------+
```

```
Task in time domain

T(greek tau) = {J_1, J_2.....,J_n} infinite // How much time the task takes

J_i = 
(One job, a loop or other limited action)



				   +--> Computational/execution time of other tasks
^ Running          |
|		_____________		   
|	   |		     |
|      ......	......
|      :	:	:	 :
|	   :	:	:	 :
+----|-:--------------------|------>+
   r_i 				       d_i
(Ready Instans)	        (Deadline)

```

```
Periodic Task ( Timer semaphore)

r_i = i*T+O // i gange periodetid + offset

d_i = r_i + d ((relative deadline))

L_i = / for every i
(wget worst case execution time)


^
|
|
|       d                       d
|   __________			   __________
|  |          |			  |          |
|r_0 = 0    d_0        r_1 = O+T    d_1      r_2 = O+2T
+--|---------:------------|----------:-----------|---->
   0                      |						 |
   |		              |                      |
   |______________________|______________________|
   		     T                       T

```

```
R(t) // Work ordered from the task within [o,t]

|									  __
|                           _________|__ τ
|                 _________|
|       _________|
|      |
|     r_0 		r_1       r_2       r_3
+------|---------|---------|---------|
	   |         |		   |		 |
	   |_________|_________|_________|
	        T         T         T

	        [X] <= x+1

o <= Θ <= T

R(t) = τ[ (t- Θ)/T] <= ceil

τ[t/T] <= τ(t/(T+1)= t*λ/T + λ
```

```
T = 1000
τ < 1000
________
τ < T

U = τ/T < 1 // Hvilket betyder at loaden skal være under 100 %

^ Q = 
|                                            Q(t) = τ 
|        ^                  :          \     U < T
|        :    Busy         τ:    Busy   \    u-1 < 9
|        :    ___           :     ___    \
|        : __/   \__        :  __/   \__  \
|   Idle :/         \ Idle  : /         \
+----|---:-------------|----:------------> t
        t_0

 Q(t) = Into [0,t] - Out of [0,t] 

 Q(t) <= t*(τ/T) + τ - t = t((τ/T)-1)

Every CPU's time frame is defined in CPU time, 1 tick = 1 CPU operation

```

```
Taskset = {τ_1 τ_2.......τ_n}
τ_i = {J_i1, J_i2, J_i3.....J_in}
J_ji
τ_j = (T_j, λ_j, d_j)

 
                 ____ Absolute deadline
                /     
J_ji = (r_ji, d_ji, λ_ji)

R_j = t* λi/T_i + λ_i

R(t) = sum(j=1, n) R_j(t) = sum(j=1, n) t*(λ_j/T_j) + λ_j

t * sum(j=1, n) (λj/T_j) + sum(j=1, n) λ_j

(Hvor lang tid skal jeg bruge per job og hvor lang tid skal jeg afvikle det)

Sum(j=1, n) (λ_j/T_j)  where λ_j = Time a job uses 	T_j = How long is needed for the cpu to do the job

Stability = U < 1
```
Hej

```
d = 0.1*T

|
|
|
|
|   d = 0.1
|   <---->
+--|------|------------------------------->
   r
   |______________|
          0.2 T

When and how do we plan things affects how a system is run

```

## Scheduling

```
Fixed
- Cyclic
- Tabellagte ( Slå det op)
Dynamic
- Fixed priorities (FP)


```

```
τ_j = {J_i1, J_i2,....J_in}


τ_1 = (T=5, λ=2, d=4)
τ_2 = (T=10,λ=3, d=7)

U = 2/5 +3/10 = 4/10+3/10 = t/10 < 1


	^ Task running
	|
	|
	|
	|
	|
2 _ |       ______        ______        ______       
	|      |      |      |      |      |      |
1 _	|______|      |______|      |______|	  |
	|      |      |      |      |      |      |  
	+------5------10-----15-----20-----25--------> 
												t -> infinity


Another example
τ_1 = (T=8, λ=2, d=4)
τ_2 = (T=10,λ=3, d=7)

LCM(8,10) | 8 = 2*2*2 | 10 = 2*5 | LCM = 2*2*2*5 = 40 |

U = 2/8 +3/10 = 10/40+12/40 = t/40 < 1

```

### Round Robin
- Each task gets a time slot of length T_s

- Within each time slot task switching lasting T_c has to tak  place

- If a task τ is not ready in due time the next task is used instead

__WORST CASE SCENARIO__

- What is the worst situation that can be presented to a task, to make it finish at the slowest possible rate

- The  worst situation is that the task arrivves right after another task has been started, resulting in it needing to wait for a new slot to open up

### Fixed priorities

__WORST CASE SCENARIO__

Critical instant theorem
- Latest completion time for τ_i, is achieved if r_i is at the same time as r_1....r_i-1

- Meaning that joining at the same time as a task with higher priority

Every higher prioritezed jobs/tasks should be done for [o,C]

```
|            [0,C]
|______________________________
|                              |                        
|                              |
|------------------------------|
|							   C
r_i=r_1...r_i-1

(sum(j=1, n) (C/T_j)*c_j + c_j)  <= C 	|	where c_j = Time a job uses 	T_j = How long is needed for the cpu to do the job


LHS = ceil(C/T_1)*c_1+ceil(C/T_2)*c_2

plot(C,C,C,LHS)


C = 0
C = LHS(C)


RMA (Rate monotonic Assignment)
T_1 < T_2

T_1 higher priority than T_2 

This means we end up with:
T_1 < T_2 < ...... < T_n
High prio            Low prio

Resiproggen
ohm_1 = 1/T_2 , Ohm_i = 1/T_i

RDA (Rate Deadline assignmant)

``` 