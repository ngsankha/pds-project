STOCK MARKET SIMULATION
=======================

This was my Programming and Data Structures project for the first semester. Its shitty considering that I didn't make full use of the standard C library. Whatever here is within the purview of the syllabus of the course.

I tried to explore less of stock market simulation algorithms and more of inter-process communication in the project. It used simple file based I/O with locks to do inter-process communication, (a message queue is a better option, and it was out of syllabus).

Compiling
---------
To compile the project execute the follwing commands at the Linux Terminal:

    gcc server.c common.c -o server.o
    gcc client.c common.c -o client.o

Running
-------
First run `server.o` in a separate window followed by `client.o`. The client will not function if the server is not running already.
