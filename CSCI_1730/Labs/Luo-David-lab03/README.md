# README
David Luo  
811357331
## Compiling and Running
To compile:
```
$ make
```
To run:
```
$ ./armstrong
$ ./primesum
etc.
```
To clean:
```
make clean
```
## Reflection
This lab was basically what I expected it to be: mostly a few novel problems
to be solved in novel ways. In particular, I was able to reduce the 
computation time for `primesum` from 1 minute 45 seconds initially to around
150 milliseconds with an algorithm I found in Wikipedia that ruled out 
everything that was divisible by 2 or 3 before checking everything else. Also
`exclusive` was interesting in the use of XOR logic in order to "toggle" the
exclusive number.
