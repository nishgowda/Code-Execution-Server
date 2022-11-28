# Code-Execution-Simulator

A Simple Code Execution simulator that reads, compiles, and executes inputted C programs and tests them on given test files.

Uses Computer System processes like **fork**, **exec**, and **I/O** to handle the above functions. The struct **Target** keeps track of a target (tid), the compilation and test commands, as well as a pointer to the next target. The **Commands** struct is a doubly linked list that stores the name and number of targets as well as the head and tail of the Target list.

You can run ```Make``` from the Makefile however make sure to adjust to the proper UNIX Operating System and also note that some tests may not work since certain tests have been ommitted from the directory.
