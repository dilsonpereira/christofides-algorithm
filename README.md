# A C++ implementation of the Christofides algorithm for instances in TSPLIB format

To clone the repo and its dependencies (submodules): 
```
git clone --recursive https://github.com/dilsonpereira/christofides-algorithm
```

Compilation on Linux:
```
make christofides
```
Usage:
```
./christofides -f <inputFile> [-p] [-g <outputFile>]
```
Option ```-p``` will print the edges in the solution. 

Option ```-g``` will output the solution in Graphviz neato format to *outpuFile*. If you have Graphviz installed and instance coordinates are available, you can generate an image of the solution by running
```
neato -Tps <christofidesOutputFile> > <image.eps>
```

This code also provides a module for parsing TSPLIB instances, a module for solving minimum spanning tree problems and a module for solving minimum cost perfect matching problems.

See Example.cpp to learn how to use the library.

Feel free to contact me if you have any problem.
