# Light C++11 graph library

We provide a simple and light graph library for visualizing and analyzing undirected graph structures. The library is inspired by [Libigl](https://github.com/libigl/libigl), following its design principle and applied for graphs. The graphs are stored as `.obj` files (not all other formats support edges) which can be easily edited in [Blender](https://www.blender.org/).

![Graph visualization](./images/graph_library.png "light C++ graph library")

## Installation:
The only dependencies are [Eigen](https://eigen.tuxfamily.org/) and [yaml-cpp](https://github.com/jbeder/yaml-cpp/wiki/Tutorial) which can be installed as follows:

```bash
sudo apt-get update
sudo apt-get install libeigen3-dev libyaml-cpp-dev
```

Then, simply type the following commands in the command line:

To build the library and the example
```bash
mkdir build
cd build
cmake ..
make
```

to build and run the tests:
```bash
mkdir build_tests
cd build_tests
cmake ../src/tests
make
```

## Features:
* graph instanciation from nodes list and edges list or adjacency matrix (`Eigen::MatrixXd` for nodes position and `Eigen::MatrixXi` for edges definition)
* load / save graphs as `.obj` files
* graph visualization (using [polyscope](http://polyscope.run/))
* adding / removing / merging nodes
* adding / removing edges
* edge collapsing
* transformation into a tree, and basic trimming
* fundamental cycle basis
* graph simplification based on cycles
* connectivity (linear time)
* biconnectivity (linear time)
* triconnectivity (quadratic time)
* bridges detection (linear time)
* shortest path on the graph (dijkstra)

### Features to add:
* graph augmentation

## Design principles:
* headers only library
* simple structures for storing the graph (`Eigen::Matrix`) and adjacency lists (`std::vector`)
* easy to modify

## Graph visualization:
![Graph visualization](./images/graph_polyscope_viewer.png "Graph visualization with libigl")

## Graph editing:
Open [Blender](https://www.blender.org/) and start creating a graph. A mesh can be easily transformed into a graph by deleting the faces only (change to `edit mode`, select everything `a`, open the delete menu `del`, pick `Only faces`). See this [cheatsheet](https://bcgiu.wordpress.com/2015/02/12/best-blender-key-map-infographic/) for a complete key map.
![Graph editing](./images/graph_editing_blender.png "Graph editing with blender")

## Other alternatives:
* [OGDF](http://www.ogdf.net): (C++) self-contained class library for the automatic layout of diagrams. OGDF offers sophisticated algorithms and data structures to use within your own applications or scientific projects (including triconnectivity tests in linear time with SPQR decomposition)
* [BOOST](https://www.boost.org/doc/libs/1_70_0/libs/graph/doc/index.html): (more?) efficient implementation
* [Matlab graph class](https://www.mathworks.com/help/matlab/graph-and-network-algorithms.html): sweet one
