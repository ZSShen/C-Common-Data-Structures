LibCDS
=============

Integration Test (Travis CI)
+ [![Integration Test] (https://travis-ci.org/ZSShen/C-Common-Data-Structures.svg?branch=master)](https://travis-ci.org/ZSShen/C-Common-Data-Structures)

##Objective
LibCDS implements several kinds of classic containers by C. It offers uniform APIs for convenient container interaction.

##Container Category
[API Reference Manual] (http://zsshen.github.io/C-Common-Data-Structures/html/index.html)
+ Tree
  + SimpleTree -- The basic binary search tree.
  + BalancedTree -- The balanced binary search tree implemented with red black tree.
+ List
  + DLinkedList -- The doubly linked list.
+ Array
  + Vector -- The dynamically growable array.
+ Map
  + OrderedMap -- The ordered map to store key value pairs implemented with balanced tree.
+ Heap
  + BinHeap -- The ordered binary heap.
+ Queue
  + PrioQueue -- The priority queue implemented with binary heap.

##Installation
####***Basic***
First of all, we need to prepare the following utilities:
- [CMake] - A cross platform build system.
- [Valgrind] - An instrumentation framework help for memory debug.
- [CUnit] - An unit test framework for C.

For Ubuntu 12.04 and above, it should be easy:
``` sh
$ sudo apt-get install -qq cmake
$ sudo apt-get install -qq valgrind
$ sudo apt-get install -qq libcunit1-dev
```
Now we can build the entire source tree under the project root folder:
``` sh
$ ./clean.py --rebuild
$ cd build
$ cmake ..
$ make
$ cd ..
```
Then the library should be under:  
- `./lib/release/libcds.so`  

The demo program should be under: 
- `./bin/demo/` 

And the header files are grouped be under: 
- `./include/` 

Besides, for development-relevant binaries, the unit tests should be under: 
- `./bin/test/` 

####***Development***
If we try to patch a certain data structure or add a new one, we can apply the following CMake options to configure the building procedures for convenient development:  
- `BUILD_SOURCE`  
  + NAME_SOURCE - Specifying a source file to build a specific data structure and the relevant demo program and unit test.
- `CMAKE_BUILD_TYPE`
  + Release - For performance optimized binary. Library output folder is `./lib/release/`.
  + Debug - For debug information added binary. Library output folder is `./lib/debug/`.
  + Note that for demo program and unit test, the output folders are always in `./bin/demo/` and `./bin/test/`.

For examples:
- For debug version of vector
``` sh
$ cmake .. -DBUILD_SOURCE=vector  -DCMAKE_BUILD_TYPE=Debug
$ make
```
- For debug version of ordered map
``` sh
$ cmake .. -DBUILD_SOURCE=ordered_map  -DCMAKE_BUILD_TYPE=Debug
$ make
```
- For debug version of all the data structures
``` sh
$ cmake .. -DCMAKE_BUILD_TYPE=Debug
$ make
```

[CMake]:http://www.cmake.org/
[Valgrind]:http://valgrind.org/
[CUnit]:http://cunit.sourceforge.net/ 
