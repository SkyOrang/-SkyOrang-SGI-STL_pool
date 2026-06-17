# SGI-STL_pool personal implementation version
语言 / Language: [中文](README.md) | [English](README.en.md)
#### Description
Based on the SGI-STL two-level allocator, it achieves high-performance memory pool. It is encapsulated into an independent library and provides interfaces such as PoolMalloc, PoolFree, PoolNew, and PoolDelete. 

Function

1.Solving the problem of memory fragmentation

2.More efficient allocation and deallocation of memory, reducing the time consumption of directly applying for and releasing memory from the operating system

3.Enhancing cache locality, concentrating data to enable faster access by the CPU

#### Software Architecture
1.Implemented solely using the C++ standard library, with no third-party dependencies. 

2.This project design is solely aimed at providing stable support for a single-threaded environment.

#### Installation
(1) Use the source code 

Simply clone and download the file and include the corresponding header file to use it. 

(2) Use the static library (the Windows version can be downloaded in the Release mode) 

Take VS as an example: 

1.In the C++ general settings section of the VS project properties, add the include file path to the additional include library directory.

2.In the linker general settings section of the VS project properties, add the library file path to the additional include library directory.

3.In the linker input section of the VS project properties, add the dependencies to be included by adding the library files.

#### Instruction for Use
For more details, please refer to the wiki.

#### Contribution
(1)Offer suggestions: 

If you have any suggestions regarding this project, please feel free to contact me via the email 759792938@qq.com. 

(2)Provide the code: 

1.  Fork the repository
2.  Create Feat_xxx branch
3.  Commit your code
4.  Create Pull Request
