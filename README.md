#C-Common-Data-Structures
+ Build Status
   - [![Build Status](https://travis-ci.org/ZSShen/C-Common-Data-Structures.svg?branch=master)](https://travis-ci.org/ZSShen/C-Common-Data-Structures)  Travis CI (integration testing)
   - [![Coverage Status](https://coveralls.io/repos/ZSShen/C-Common-Data-Structures/badge.png?branch=master)](https://coveralls.io/r/ZSShen/C-Common-Data-Structures?branch=master)  Coveralls (code coverage testing)

##Introduction
Though STL is convenient, the know-how is more valuable.   

This project implements some classic data structures using C.   
It is not only a memo but also a verification for some advanced   
or theoretical structures.

##Usage
### 1. Source Building
   
#### 1.1 Source Tree
+ The contained data structures and the relevant building tools.   
    - Makefile
    - Tree   
        * BinarySearchTree   
        * RedBlackTree  
        * SparseTrie
    - List   
        * SinglyLinkedList   
    - Array   
        * DynamicArray   
    - Map   
        * OrderedMap   
    - Heap   
        * BinaryHeap   
        * BinomialHeap   
    - Queue   
        * PriorityQueue   
    - Misc   
        * interface_generator.py   
        * dictionary_generator.py

+ The source files for a single structure.
    - DataStructure
        * data_structure.c
        * data_structure.h
        * test_program.c
        * Makefile
   
#### 1.2 Building Entire Source
+ Generally, each data structure is wrapped in a single C structure and   
  is built as an object file. To verify its correctness, we must rely  
  on the test program which prepares several unit test functions.   

+ There is a major Makefile at the top of source tree:   
    - For the normal build, execute `make all`.   
    - For the debug build (memory inspection), executre `make all DEBUG=true`.     

+ By running the above command, we compile the object files and the test   
  programs for all the data structures. Also note that the memory debug   
  build should be driven by the `valgrind` utility.

#### 1.3 Building Specific Structure  
+ To generate object file bundled with test program.  
    - For the normal build, execute `make build_test`.  
    - For the debug build, execute `make build_test DEBUG=true`.  
+ To generate static library  
    - For the normal build, execute `make build_static_lib`.  
    - For the debug build, execute `make build_static_lib DEBUG=true`.  
+ To generate dynamic library
    - For the normal build, execute `make build_dynamic_lib`.  
    - For the debug build, execute `make build_dynamic_lib DEBUG=true`.  

### 2. Binary Execution
+ As aforementioned, we rely on the test program to verify each kind of  
  data structure. For this, the testing can be launched for all the structures  
  (except for the PriorityQueue) with the following commands:  
    - For simple testing, execute `make test`.  
    - For debug testing (the object file and test program should be debug  
      build), execute `make test DEBUG=true`.  

+ For PriorityQueue  
  Currently, the queue is just an interface which relies on the implementation  
  of `binary heap` and `binomial heap`. In the source building stage (entire source  
  or specific structure), the dependent heap implementations are also generated  
  with the makefile command. Thus users do not neet do care about the dependency  
  and can focus on the testing of each kind of implementation.
    - To choose binary heap as implementation,  
      execute `make test IMPLEMENT=binaryheap`. (`DEBUG=true` for debug build)
    - To choose binomial heap as implementation,  
      execute `make test IMPLEMENT=binomialheap`. (the same for debug build)  
