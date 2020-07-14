# Generic Container

I am working on a project that uses a bunch of lists and they all are very similar, but they use different data types. The nature of a list in C is that all of the elements are the same size, and so re-creating the data structures and the functions that use them is a real pain in the but. Unfortunately, C does not have anything like a C++ container. No problem. Just abuse the hell out of the preprocessor. 

The goal of this project is to create an easy-to-use set of macros that facilitate the creation of data structure containers that have different behavious, such as lists and hashes. In the future, other containers will be added such as linked lists and binary trees. 

This code is not pretty but it does work perfectly, and it simplifies creating lists and hash tables for data structures. 



