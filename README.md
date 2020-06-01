# CircleAlgorithms
Timing various algorithms used to determine points in a circle.

I was watching a video about giving technical interviews.  The speaker mentioned finding technical questions which had multiple layers/solutions, starting with the obvious and then introducing levels of optimiziation. These can be worked through during an interview to explore critical thinking skills.  An example given was algorithms to determine the points a circle (think pixels that would need to be colored to draw the circle).  This peaked my interest, so I am attempting to implement has many circle algorithms as possible.  I am also timing them to see what improvements there really are...

## Overview
The algorithms I am creating produce an array of integer pairs describing the (X,Y) coordinates that make up the circle.  The only input required for the algorithm is the radius of the circle (all circles are centered at (0, 0)).  

Many of my algorithm implementation use the radius to determine indexing into the result array.  Based on shortcuts I am taking during implementation this requires the radius to be a multiple of 4 (or 8 in a few cases).  If it is not, integer math rounding could throw off the results or lead to buffer overflow issues.

## Floating Point Algorithms

I am splitting results into two classes: those that require floating point math (and then convert to integers at the end) and those which work on integers only).  The floating point algorithms all increment through angles and calculate (X,Y) using trig functions (or equivalent formulas).  

### Naive Implementation
The "naive implementation" uses the Math.h trig functions (sin/cos) to calculate all points between 0 and 2 PI.  I expect this to be the worst implementation because of the number of calls to the library functions.

### Naive Eighths Implementation
An obvious improvement to the naive implementation is to take advantage of the symmetry of a circle.  Using the symmetry properties between sin and cos, we only actually need to calculate 1/8th of a circle and can then mirror the result around to fill in the rest.  I expect this to see a 8x improvement over the naive implementation.  Subsequent implementations will take advantage of the symmetry to the maximum extent possible.

### Taylor Expansion Implementation
Thinking way back to math I rarely use, both sin and cos can be solved using a Taylor series expansion.  I had to look up these formulas and did experintation to find that you only need the first 4 powers to accurately calculate the circle points (espexially considering we are rounding them to nearest whole number). Not sure if this will be more efficient than the math libraries or not (I suspect under the hood the math libraries may be doing Taylor expansions to calculate sin/con... if so they probably take it to many more powers than I am).  


## Integer Algorithms
The integer algorithms "brute force" or otherwise indirectly calculate the points on the circle.  In doing so, they do not rely on floating point numbers because they do not increment through angles.  

### Radius Limits
My first attempt relied on the formula for a circle (X^2 + Y^2 = R^2).  It looped through all X values (radius argument of the calculate function down to 0) and then looked for a Y value that falls on the radius of the circle.  The downside to this implementation is that for many X integer values, there is not a Y integer value that exactly fits the formula for the circle (i.e. 20^2 + 20^2 = 400, 20^2 + 21^2 = 441... if the radius 420 there isn't an integer Y that works for that X).  

### Radius Bound
Thinking about it a little more, this is how radius limits should have been implemented.  Loop through all the X values and then look for the first Y value that causes you to bust the radius.  Stop looking when that happens and use Y - 1 as the Y value (the  last value that didn't bust the radius).  

TODO: Need to implement the eights algorithm for these, currently only doing quarter symmetry.  Problem with the eights algorithm is figuring out when you have calculate 1/8th of the circle (could look for point at which X = Y, assuming that will always happen using these techniques).  

