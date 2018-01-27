[![Build Status](https://travis-ci.org/ZSShen/C-Common-Data-Structures.svg?branch=master)](https://travis-ci.org/ZSShen/C-Common-Data-Structures)  [![Coverage Status](https://coveralls.io/repos/github/ZSShen/C-Common-Data-Structures/badge.svg?branch=master)](https://coveralls.io/github/ZSShen/C-Common-Data-Structures?branch=master)

## **Container Category**
 + Sequential Container
   + **Vector** --- The dynamically growable array  
   + **LinkedList** --- The doubly linked list  
 + Associative Container
   + **TreeMap** --- The ordered map to store key value pairs 
   + **HashMap** --- The unordered map to store key value pairs
   + **HashSet** --- The unordered set to store unique elements  
   + **Trie** --- The string dictionary  
 + Simple Collection Container
   + **Queue** --- The FIFO queue  
   + **Stack** --- The LIFO stack  
   + **PriorityQueue** --- The queue to maintain priority ordering for elements  

## **Installation**
**This section illustrates how to install LibCDS to your working directory.**

First of all, we need to prepare the CMake build tool:  
+ [CMake] - A cross platform build system.

For Ubuntu 12.04 and above, it should be easy:  
``` sh
$ sudo apt-get install -qq cmake
```

Now we can build the entire source tree under the package root folder:
``` sh
$ ./clean.py --rebuild
$ cd build
$ cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/your/destination
$ make
$ make install
```

Upon finishing, the public header should locate at:  
`/path/to/your/destination/include/`  
And the shared library should locate at:  
`/path/to/your/destination/lib/`  

If you plan for debug build, you can specify the CMake argument list like this:
``` sh
$ cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=/path/to/your/destination
```

## **Usage**
**This chapter illustrates how to link and apply LibCDS in your project.**

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
But wait, to run your project, you need to tell the dynamic linker where to find LibCDS:
```
LD_LIBRARY_PATH=/path/to/your/destination/lib/
```
For detailed API usage, you can refer to the manual or check the `demo programs`.

## **Benchmark**

Thanks for the **HashMap** benchmark with abundant key value pair manipulation provided by [kbench]. The results are compared with other 49 similar C data structure libraries.

+ **Criteria** - C style string as hash key
+ **Platform** - Linux nuc 3.2.0-4-amd64 #1 SMP Debian 3.2.81-1 x86_64 GNU/Linux
+ **Date** -  Tue Oct 25 13:25:20 CEST 2016
+ **Result sorted by CPU time**  

| Rank | Implementation    | CPU (secs) | Memory (Mb) |    #    | Notes     |
| ---- | ----------------- | ---------- | ----------- | ------- | --------- |
|    1 | rigtorp-hashmap   |      0.760 |     347.100 |  625792 |           |
|    2 | rdestl            |      0.850 |     290.004 |  625792 |           |
|    3 | ulib              |      1.010 |     240.724 |  625792 |           |
|    4 | libevent          |      1.110 |     504.988 |  625792 |           |
|    5 | khash             |      1.140 |     240.716 |  625792 |           |
|    6 | ccan              |      1.210 |     227.240 |  625792 |           |
|    7 | hashit-overflow   |      1.290 |     415.924 |  636452 | Bug here! |
|    8 | hashit-open       |      1.300 |     346.448 |  625792 |           |
|    9 | mct-closed        |      1.380 |     339.140 |  625792 |           |
|   10 | amtl              |      1.410 |     332.428 |  625792 |           |
|   11 | sys-apr           |      1.410 |     256.584 |  625792 | **        |
|   12 | sys-glib          |      1.440 |     228.968 |  625792 | **        |
|   13 | oddou-hashmap     |      1.460 |     299.112 |  625792 |           |
|   14 | google-dense      |      1.470 |     380.128 |  625792 | **        |
|   15 | gcc-libiberty     |      1.500 |     240.180 |  625792 |           |
|   16 | uthash            |      1.540 |     618.028 |  625792 |           |
|   17 | tommyds-fixed     |      1.570 |     944.988 |  625792 |           |
|   18 | python            |      1.610 |     264.492 |  625792 |           |
|   19 | Qt-hash           |      1.700 |     255.456 |  625792 | **        |
|   20 | cfu               |      1.710 |     272.376 |  625792 |           |
|   21 | ghthash           |      1.720 |     362.476 |  625792 |           |
|   22 | tommyds-dynamic   |      1.760 |     903.452 |  625792 |           |
|   23 | generic-c-hashmap |      1.770 |     502.416 |  698396 | Bug here! |
|   24 | hashit-chain      |      1.830 |     300.464 |  625792 |           |
|   ***25*** | ***CDS***               |      ***1.870*** |     ***252.236*** |  625792 |           |
|   26 | tommyds-linear    |      1.900 |     889.244 |  625792 |           |
|   27 | eastl             |      1.910 |     263.584 |  625792 |           |
|   28 | c-hashtable       |      1.940 |     257.360 |  625792 |           |
|   29 | sys-tcl           |      1.960 |     256.976 |  625792 | **        |
|   30 | apr               |      1.970 |     253.012 |  625792 |           |
|   31 | c-algoritms       |      2.000 |     266.996 |  625792 |           |
|   32 | sys-boost         |      2.080 |     263.516 |  625792 | **        |
|   33 | sys-python        |      2.160 |     834.312 |  625792 | **        |
|   34 | redis             |      2.280 |     253.212 |  625792 |           |
|   35 | htable            |      2.380 |     541.268 |  625792 |           |
|   36 | gcc-unordered_map |      2.550 |     263.592 |  625792 | **        |
|   37 | ruby-st           |      2.950 |     255.504 |  625792 |           |
|   38 | sys-judy          |      3.040 |     259.372 |  625792 | **        |
|   39 | mct-linked        |      3.110 |     363.708 |  625792 |           |
|   40 | sglib             |      3.220 |     450.572 |  625792 |           |
|   41 | sys-perl          |      3.300 |     301.240 |  625792 | **        |
|   42 | stb               |      3.390 |     282.116 |  625792 |           |
|   43 | st                |      3.530 |     245.784 |  625792 |           |
|   44 | google-sparse     |      4.430 |     251.376 |  625792 | **        |
|   45 | sys-LuaHashMap    |      6.110 |     354.948 |  625792 | **        |
|   46 | google-c-sparse   |      6.140 |     244.408 |  625792 |           |
|   47 | google-c-dense    |      6.480 |     244.408 |  625792 |           |
|   48 | gcc-map           |      7.730 |     264.996 |  625792 | **        |
|   49 | Qt-map            |      8.560 |     266.776 |  625792 | **        |
|   50 | lua-table         |    102.350 |     264.744 |  625792 |           |

+ **Result sorted by memory usage**  

| Rank | Implementation    | CPU (secs) | Memory (Mb) | #      | Notes     |
|------|-------------------|------------|-------------|--------|-----------|
| 1    | ccan              | 1.21       | 227.24      | 625792 |           |
| 2    | sys-glib          | 1.44       | 228.968     | 625792 | **        |
| 3    | gcc-libiberty     | 1.5        | 240.18      | 625792 |           |
| 4    | khash             | 1.14       | 240.716     | 625792 |           |
| 5    | ulib              | 1.01       | 240.724     | 625792 |           |
| 6    | google-c-sparse   | 6.14       | 244.408     | 625792 |           |
| 7    | google-c-dense    | 6.48       | 244.408     | 625792 |           |
| 8    | st                | 3.53       | 245.784     | 625792 |           |
| 9    | google-sparse     | 4.43       | 251.376     | 625792 | **        |
| ***10***   | ***CDS***               | ***1.87***       | ***252.236***     | 625792 |           |
| 11   | apr               | 1.97       | 253.012     | 625792 |           |
| 12   | redis             | 2.28       | 253.212     | 625792 |           |
| 13   | Qt-hash           | 1.7        | 255.456     | 625792 | **        |
| 14   | ruby-st           | 2.95       | 255.504     | 625792 |           |
| 15   | sys-apr           | 1.41       | 256.584     | 625792 | **        |
| 16   | sys-tcl           | 1.96       | 256.976     | 625792 | **        |
| 17   | c-hashtable       | 1.94       | 257.36      | 625792 |           |
| 18   | sys-judy          | 3.04       | 259.372     | 625792 | **        |
| 19   | sys-boost         | 2.08       | 263.516     | 625792 | **        |
| 20   | eastl             | 1.91       | 263.584     | 625792 |           |
| 21   | gcc-unordered_map | 2.55       | 263.592     | 625792 | **        |
| 22   | python            | 1.61       | 264.492     | 625792 |           |
| 23   | lua-table         | 102.35     | 264.744     | 625792 |           |
| 24   | gcc-map           | 7.73       | 264.996     | 625792 | **        |
| 25   | Qt-map            | 8.56       | 266.776     | 625792 | **        |
| 26   | c-algoritms       | 2          | 266.996     | 625792 |           |
| 27   | cfu               | 1.71       | 272.376     | 625792 |           |
| 28   | stb               | 3.39       | 282.116     | 625792 |           |
| 29   | rdestl            | 0.85       | 290.004     | 625792 |           |
| 30   | oddou-hashmap     | 1.46       | 299.112     | 625792 |           |
| 31   | hashit-chain      | 1.83       | 300.464     | 625792 |           |
| 32   | sys-perl          | 3.3        | 301.24      | 625792 | **        |
| 33   | amtl              | 1.41       | 332.428     | 625792 |           |
| 34   | mct-closed        | 1.38       | 339.14      | 625792 |           |
| 35   | hashit-open       | 1.3        | 346.448     | 625792 |           |
| 36   | rigtorp-hashmap   | 0.76       | 347.1       | 625792 |           |
| 37   | sys-LuaHashMap    | 6.11       | 354.948     | 625792 | **        |
| 38   | ghthash           | 1.72       | 362.476     | 625792 |           |
| 39   | mct-linked        | 3.11       | 363.708     | 625792 |           |
| 40   | google-dense      | 1.47       | 380.128     | 625792 | **        |
| 41   | hashit-overflow   | 1.29       | 415.924     | 636452 | Bug here! |
| 42   | sglib             | 3.22       | 450.572     | 625792 |           |
| 43   | generic-c-hashmap | 1.77       | 502.416     | 698396 | Bug here! |
| 44   | libevent          | 1.11       | 504.988     | 625792 |           |
| 45   | htable            | 2.38       | 541.268     | 625792 |           |
| 46   | uthash            | 1.54       | 618.028     | 625792 |           |
| 47   | sys-python        | 2.16       | 834.312     | 625792 | **        |
| 48   | tommyds-linear    | 1.9        | 889.244     | 625792 |           |
| 49   | tommyds-dynamic   | 1.76       | 903.452     | 625792 |           |
| 50   | tommyds-fixed     | 1.57       | 944.988     | 625792 |           |



## **Contact**
Please contact me via the mail ***andy.zsshen@gmail.com***.  

[CMake]:http://www.cmake.org/
[Valgrind]:http://valgrind.org/
[CUnit]:http://cunit.sourceforge.net/
[kbench]:https://github.com/rcarbone/kbench
