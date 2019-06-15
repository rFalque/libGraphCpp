/**
 * Author: R. Falque
 * 
 * main for testing the embedded deformation implementation
 * by R. Falque
 * 14/11/2018
 **/

// dependencies

#include <igl/readOBJ.h>
#include <igl/opengl/glfw/Viewer.h>

#include "graph_lib/graphStructure.hpp"
#include "graph_lib/readGraphOBJ.hpp"
#include "graph_lib/plotGraph.hpp"

#include "graph_lib/options.hpp"

#include <yaml-cpp/yaml.h>



int main(int argc, char* argv[])
{
  options opts;
  opts.loadYAML("../config.yaml");

  /* 
    * N: nodes of the graph
    * E: edges of the graph
    */
  Eigen::MatrixXd N;
  Eigen::MatrixXi E;
  
  readGraphOBJ(opts.path_graph_obj ,N, E);
  directional::plot_graph(N, E, opts);

  return 0;
}

