C-Utils
========

Just a some usefull C-Utils:


BMP
---
Low level implementation of the BMP file format


Bool Matrix
---
Low level implementation of an Bool Matrix (preprocessor macro based)
(each Bit can be setted)


Dynamic Array
--- 
Low level implementation of an Dynamic Array (preprocessor macro based)


Debug
---
some helpfull debugging stuff


Heap
--- 
Low level implementation of an Array based Heap, macrobased for the speed


Long Array
--- 
Low level implementation of an Long Array (preprocessor macro based)
Long Array is usefull when memory is fragmented and it faild to malloc an
long block of free memory, Long Array uses many smal memory blocks and
let they access you like one long Array.


Point
--- 
A simple two dimensional point impelementation
as a struct and a uint64 which uses the higher 32 bits
as y and the lower as x coordianate


Red Black Tree
--- 
Implementation of an Red Black Tree, currently onley stores uint64 keys,
but aditionaly void pointer could be added when nedded


Sort
--- 
Implementation of an improofed version of Quicksort, using Inserionsort
at a sepecific min Array length.

Severial versions are privided:

  + Void-pointer based
  + Macro based (25% faster than void-pointer)
  + Paralell-void-pointer based (only tested with two cores litle bit faster
    than void-pointer but with more corse it will sure be the fastest version)


String
---
Just a bunch of string manipulation functions


Thread Clients
--- 
With thread clients you can creat a amoung of threads once
and give them work without recreating the threads (less syscalls)


Time Diff
--- 
Just an util to calculate the diffenece between two times (timval struct)


Timeout
---
A fork, shared memory and sigterm based timeout
