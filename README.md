C-Utils
========

C-Utils is a collection of low level speed improved functions for ANSI C


Arg-Parse
---
easy but low speed argument parsing


BMP
---
Low level implementation of the BMP file format


Bool-Array
---
Low level implementation of an true false array (bit array)


Bool Matrix
---
Low level implementation of an Bool Matrix (preprocessor macro based)
(each Bit can be accessed)


Debug
---
some help full debugging stuff


Dynamic Array
--- 
Low level implementation of an Dynamic Array (preprocessor macro based)


Hash
---

Low level implementation of a macro based hash map with double hashing
and multiplication method as hash functions

Currently only inserting is supported


Heap
--- 
Low level implementation of an Array based Heap, macro based for the speed


Long Array
--- 
Low level implementation of an Long Array (preprocessor macro based)
Long Array is useful when memory is fragmented and it failed to malloc an
long block of free memory, Long Array uses many small memory blocks and
let they access you like one long Array.


Memory
---
some memory related functions (get total memory, print bytes)


Point
--- 
A simple two dimensional point implementation
as a struct and an uint64 which uses the higher 32 bits
as y and the lower as x coordinates


Rand
---
Low level 32 bit rand implementation


Red Black Tree
--- 
Implementation of an Red Black Tree, with uint64 keys and void pointer values


Sort
--- 
Implementation of an improved version of Quicksort, using Inserionsort
at a specific min Array length.

Several versions are provided:

  + Void-pointer based
  + Macro based (25% faster than void-pointer)
  + Parallel-void-pointer based (only tested with two cores little bit faster
    than void-pointer but with more corse it will sure be the fastest version)


String
---
Just a bunch of string manipulation functions


Thread Clients
--- 
With thread clients you can create an among of threads once
and give them work without recreating the threads (less syscalls)


Timeout
---
A fork, shared memory and sigterm based timeout


Time Diff
--- 
Just an until to calculate the difference between two times (timval struct)


WAVE
---
Low level implementation of the WAVE-RIFF file format
