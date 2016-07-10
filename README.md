# Implementation in C++ of the Edmonds–Karp algorithm


### University
> Instituto Superior Tecnico, Lisbon, Portugal

### Course
> Analysis and Synthesis of Algorithms

### Date
> April , 2014

### Authores
> [Nuno Cameira](https://github.com/nuno-cameira)
>
> [Vasco Loureiro](https://github.com/UTurista)

---------------------------------------
## What is it
>Edmonds–Karp Algorithm is an implementation of the Ford–Fulkerson method for computing the maximum flow in a flow network. The algorithm was first published by Yefim (Chaim) Dinic in 1970 and independently published by Jack Edmonds and Richard Karp in 1972.

## Implementation
The requested project consisted in receiving an input with the following format:
 
    N S     	(line 1)
    A B     	(line 2)
    ...
	K			(line S+1)
	Z E ... U	(line S+2)
	...
	

> N was the ammount of nodes, S the number of connections and the following lines defined each connection between the nodes.
> K is the number of problems to solve (in the lines below)
> Z is the ammount of critical nodes, E ... U are those critical nodes

and we would need to return 1 value for each problem

	R - Minimum number of connections needed to solve the problem
    
Input example:

    4 4
    0 2
    0 3
    1 2
    1 3
    2
    2 0 1
    2 0 2

Matching output example

    2
    2

---------------------------------------
##### Disclaimer
Should be fully functional and without bugs but use this at you own risk, this was just a small treasure found between many bits and bytes, the upload is just to prevent its loss.  

##### License
MIT
