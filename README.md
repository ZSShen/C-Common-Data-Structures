LibCDS
=============

Integration Test (Travis CI)
+ [![Integration Test] (https://travis-ci.org/ZSShen/C-Common-Data-Structures.svg?branch=master)](https://travis-ci.org/ZSShen/C-Common-Data-Structures)  

Unit Test Coverage Status (Coveralls.io)
+ [![Coverage Status](https://coveralls.io/repos/ZSShen/C-Common-Data-Structures/badge.svg?branch=master)](https://coveralls.io/github/ZSShen/C-Common-Data-Structures?branch=master)


##API Document
[Link of the API Manual] (http://zsshen.github.io/C-Common-Data-Structures/html/index.html)

##Container Category
 - Sequential Container
   - Vector --- The dynamically growable array
   - LinkedList --- The doubly linked list
 - Associative Container
   - TreeMap --- The ordered map to store key value pairs
   - HashMap --- The unordered map to store key value pairs
   - HashSet --- The unordered set to store unique elements
 - Simple Collection Container
   - Queue --- The FIFO queue
   - Stack --- The LIFO stack
   - PriorityQueue --- The queue to maintain priority ordering for elements

##Installation
***This chapter illustrates how to install LibCDS to your working directory.***

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
Now we can build the entire source tree under the package root folder:
``` sh
$ ./clean.py --rebuild
$ cd build
$ cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/your/destination
$ make
$ make install
```
Upon finishing, the public header will reside in `/path/to/your/destination/include/`.
And the shared library will reside in `/path/to/your/destination/lib/`.

If you plan for debug build, you can specify the cmake argument list like this:
``` sh
$ cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=/path/to/your/destination
```

##Usage
***This chapter illustrates how to link and apply LibCDS in your source code.***

For header file, you just need to include the main header:
```
#include "cds.h"
```
Assume that you apply gcc for compilation, then you need the following options:
```
-I/path/to/your/destination/include/
-L/path/to/your/destination/lib/
-lcds
```
Now you successfully link LibCDS with your project!
But wait, to run your project, you need to tell the loader where to find LibCDS:
```
LD_LIBRARY_PATH=/path/to/your/destination/lib/
```
That's all the related stuff!

For detailed API usage, you can refer to the manual or check the `demo programs`.


[CMake]:http://www.cmake.org/
[Valgrind]:http://valgrind.org/
[CUnit]:http://cunit.sourceforge.net/
