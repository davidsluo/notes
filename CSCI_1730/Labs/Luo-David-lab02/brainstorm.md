# Brainstorm
David Luo with Sophia Mapua and Stone Daniel

## stat.cpp
Have one variable for the total sum and one for the number of numbers. Use these
to calculate the sum, average, sum of squares, and variance. I think I read 
somewhere that `cin` ignores whitespace, so I think that's how I'll parse the
input.
## minmax.cpp
Have two variables, one for min, one for max. Using the aforementioned `cin`
behaviour, update the respective `min` and `max` variables if there is something
greater than or less than the corresponding values.
## numbers.cpp
Use the libraries included in C++ to convert decimal to the other bases and
maybe use some string formatting to make it look pretty. Append the correct 
suffixes if divisible by 3 or 5 or both or neither.
## degrees.cpp
This just an implementation of the formula to convert Celcius to Farenheight.

C = F * 9 / 5 + 32
## time.cpp
This is a simple modulus/division problem.

hours = input % 3600  
minutes = (input - (hours * 3600)) % 60
seconds = input - (hours * 3600) - (minutes * 60)
