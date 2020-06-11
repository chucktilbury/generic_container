# List Container

I am working on a project that uses a bunch of lists and they all are very similar, but they use different data types. The nature of a list in C is that all of the elements are the same size, and so re-creating the data structures and the functions that use them is a real pain in the but. Unfortunately, C does not have anything like a C++ container. No problem. Just abuse the hell out of the preprocessor. 

This code is not pretty but it does work perfectly, and it simplifies creating lists for data structures. 

This repo has 3 files. ```list_container.h```, ```generic_list.c```, and ```generic_list.h``` that implements the container and some simple tests. To compile it: ```gcc -Wall -Wextra -o gt generic_list.c```. To run the test: ```./gt```. Simple, eh?
