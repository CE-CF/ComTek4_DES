# Lecture 11

## Tidsdomæne

- __Det absolut vigtigste i tidsdomænet er stabilitet__

```

T = {J_1, J_2,...J_n} følge af jobs
J_i = {r_i, c_i, d_i} hvo r d_i = r_i+d (Relative deadline)

Periodiske task
r_(i+1) = r_i + T

```

```
Stabilitet
taskset = {T_1,T_2......,T_n}

U = sum(1, n) (C_k/T_k) < 1

Scheduling
statisk
	cycliske
dynamiske
	round robin 			We
	fixed priority			Critical instans theorem (Sufficient and exact)
	earliest deadline first	(Exact)

d_i = T_i   hvor U<1

```

Afhængige task

```
Deler kritisk data
beskyttet af mutex/Semaphoreo

```c
k_wait(mutex);
	:					|  
	: //Kritisk region	| -> deadlock
	:					|	Priority invasion
k_wait(mutex)

```

Priority inheritance
```
Priority ceiling
Immideiate ceiling

B_i + c_i + sum(1,(i-1)) t/T_j*c_j <= t


RMA 
U <= n(e^(1/n)-1) -> 0.78 (log2)

for alle i i {1....n}
U_i+(B_i/T_i) <= i(2^(1/i))-1
Tilstrækkeligt.

```
### Polling
Man kan sætte en scheduling til at checke om der er sket noget


```
r_(i+1) = r_i + T_i

-----|-----|--------|
    r_1   r_2      r_3
     |_____|________|
       T_1  <  T_2

T_i >= T    

```

### Classification of aperiodic tasks

- Sporadiske task
- - `T << E(r_(i+1)-r_i)` `
- - `d << E(r_(i+1)-r_i)`
- - `d ~ E(r_(i+1)r_i)`
	
	hvor << betyder "langt mindre end forventet
```
Aperiodic server

(>-<) ventil


Billet kommer i bucket med bestemt T_interval

    			|___| <- token bucket
				|_x_| <- biletter i bucket 
   r_i,..r_n ->  >-<  ------> 


Der er kun en task der kan køre når der er en billet i tokeń bucket.

Man ligger typisk en billet i fra starten


Tilfælde hvor det her bliver brugt (Røgalarm, bouncing)

```
Tilfælde hvor de ikke skal serviceres så hurtigt.

```
Aperiodic server

(>-<) ventil


Billet kommer i bucket med bestemt T_interval

    			|___| <- token bucket
     _______	|_x_| <- biletter i bucket 
   	 |  |  | ->  >-<  ------> 
     -------   
     queue


PC = Producer/Consumer
To task der kommunikerer asynkront med hinanden

| | ----> | | | ----> | |
Producer    PC        Consumer


Aperiodiske servere bruger man f.eks. til at modvirke denial of service attack: En bestemt bruger har en limit på forbrug.

Det er tit at der er en sidste "interessedato" altså timer hvorefter man smider gamle eller nye opgaver ud efter.

```

```
M/D/1 

M = Tilfædigt ind
D = Deterministisk ud


     M            D
| | ----> | | | ----> | |
Producer    PC        Consumer

for vores eksempel vist til forelæsning

T_A = 4/(1/6) ? 4*6 = 24

T_s = 10, 20, 30

utilization : (o) = (T_S/T_A) < 1  // (o) er "Rou" mærkeligt o

```

```
d >> E(r_(i+1)-r_i) // deadline er større end execution time.


```

```
|
|-------|-------|-------|-------|
|r_1   r_2     r_3

        |_______|
           T_i

tind r epsillon [0,d_i]

c_i + sum(1, (i-1)) ceil(t/t_J) * c_j <= t           <------- er t <= T_i

3+ceil(t/1000))*500 <= t
hvis t er 1

3+1*500 så der er t ikke >= T_i

så nu spørger vi er t <= 2*T_i

hvis ja ved vi at den kan nå 2 opgaver på t

Dette bliver man så bare ved med

n**c_i + sum(1,(i-1)) ceil(t/T_j) * c_j <= t   bliver vi ved med indtil at t <= n*T_i

vi kunne se i eksemplet af køen blev 25, for at finde om det er rigtigt:

25*3+ceil(t/1000)*500 <= t

t¹ = 0 -> t² = 75 -> t³ = 75+500 -> t⁴ = 75+500

Da t³ og t⁴ gentager sig er vi færdige med at lave iterationen

t <= 20*25 = 500

```
## Real-time
```
Task behavioural Specification (real timeness)

hard realtime: d <= T_min
realtime: d < infinity
soft real time: E(C) < d, P(C<d) >= P_d   (middeltid fra ready til completion skal være et bestemt stykke tid)
non realtime: any other value
``` 

Exercise
			
```
				+-----------------------------------------------------------+
				|  Interarrival			| Sampling	| Update	| Execution	|
				+-----------------------|-----------|-----------+-----------+
4 alarms 		|		10 min  		| 			|			|	10 ms	|
1 user input	| min 1/4 sec mean 2 sec|           |			|   10 ms   | + 10 ms
1 network input | min 10 ms mean 10 sec |			|			|   30 ms   |
#1 rt control	| 						| 100 ms 	|			|   10 ms   | 
#2 rt control 	|						| 1   sec   |			|   10 ms   | + 10 ms 
screen clock    |						| 		    |	100 ms	|   20 ms   |
                +-----------------------------------------------------------+

Deadline
----------------
4 alarmer									 10-40 ms  
network input 								 70 ms 
#2 rt control								 90 ms 								
#1 rt control								100 ms
user input									120 ms
screen clock								140 ms

Fixed Prioriy
-----------------
4 alarmer								  	1,2,3,4
network input 								5
#2 rt control								6								
#1 rt control								6
user input									7
screen clock								8


```