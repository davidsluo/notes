# README
David Luo
811357331

## How to Compile and Run

### Compilation:
```
$ make
```

### Cleaning
```
$ make clean
```

### Running
```
$ ./cat <file 1> <file 2> ...
```
OR
```
$ ./cat -
```
to read from `stdin`. This can be in place of any file arguement.

Ex:
```
$ ./cat - foo bar -
```
will read from `stdin`, then foo, then bar, then `stdin` again.

## Reflection
This lab proved to me that low level IO wasn't as bad as I initially thought.
Previously, I thought it'd take much more research adn looking at exmaples, 
but it turned out that it was all relatively simple.

One hitch I did hit, however, was trying to read from `stdin` more than once. 
Initally, it would complain that the `stdin` file descriptor was bad, until
I discovered that closing `stdin` would prevent it from being reopened. After 
that, I made sure that it only closed the file if the file descriptor was not
`stdin`.
