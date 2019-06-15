/**
 * Author: R. Falque
 * 
 * main for testing the embedded deformation implementation
 * by R. Falque
 * 14/11/2018
 **/

// dependencies
#include <cstdlib>
#include <iostream>
#include <limits>

#define IGL_VIEWER_VIEWER_QUIET

#include <igl/readOBJ.h>
#include <igl/opengl/glfw/Viewer.h>

#include "../graph_lib/graphStructure.hpp"
#include "../graph_lib/readGraphOBJ.hpp"
#include "../graph_lib/plotGraph.hpp"
#include "../graph_lib/options.hpp"

#include <yaml-cpp/yaml.h>

int main(int argc, char* argv[])
{
  options opts;
  opts.loadYAML("../tests_config.yaml");

  /*
    * TEST GRAPH CONNECTIVITY
    */

  std::cout << "Test the connectivity tests :\n";

  Graph * graph;

  // disconnected graph
  graph = new Graph("../data/graphs_test/6_disconnected.obj", opts.verbose);
  graph->init();
  if (!graph->is_connected() && !graph->is_biconnected() && !graph->is_triconnected())
    std::cout << "test connectivity for disconnected graph               [\033[1;32mPASSED\033[0m]\n";
  else
    std::cout << "test connectivity for disconnected graph               [\033[1;31mFAILED\033[0m]\n";
  if (opts.visualization)
    graph->plot();

  // connected graph
  graph = new Graph("../data/graphs_test/3_1_node_connectetivity.obj", opts.verbose);
  graph->init();
  if (graph->is_connected() && !graph->is_biconnected() && !graph->is_triconnected())
    std::cout << "test connectivity for 1-node-connected graph           [\033[1;32mPASSED\033[0m]\n";
  else
      std::cout << "test connectivity for 1-node-connected graph           [\033[1;31mFAILED\033[0m]\n";
  if (opts.visualization)
    graph->plot();

  // biconnected graph
  graph = new Graph("../data/graphs_test/5_2_edge_connectetivity.obj", opts.verbose);
  graph->init();
  if (graph->is_connected() && graph->is_biconnected() && !graph->is_triconnected())
    std::cout << "test connectivity for 2-node-connected graph           [\033[1;32mPASSED\033[0m]\n";
  else
    std::cout << "test connectivity for 2-node-connected graph           [\033[1;31mFAILED\033[0m]\n";
  if (opts.visualization)
    graph->plot();

  // triconnected graph
  graph = new Graph("../data/graphs_test/0_graph_complete.obj", opts.verbose);
  graph->init();
  if (graph->is_connected() && graph->is_biconnected() && graph->is_triconnected())
    std::cout << "test connectivity for 3-node-connected graph           [\033[1;32mPASSED\033[0m]\n";
  else
      std::cout << "test connectivity for 3-node-connected graph           [\033[1;31mFAILED\033[0m]\n";
  if (opts.visualization)
    graph->plot();



  /*
    * TEST Dijkstra
    */

  std::cout << "\nTest Dijkstra's algorithm :\n";
  graph = new Graph("../data/graphs_test/6_disconnected.obj", opts.verbose);
  graph->init();
  if ( graph->dijkstra(0, 1) == std::numeric_limits<double>::infinity() )
    std::cout << "test Dijkstra for disconnected graph                   [\033[1;32mPASSED\033[0m]\n";
  else
  {
    std::cout << "test Dijkstra for disconnected graph                   [\033[1;31mFAILED\033[0m]\n";
    std::cout << "real distance: inf, distance found: " << graph->dijkstra(0, 1) << std::endl;
  }

  
  graph = new Graph("../data/graphs_test/0_graph_complete.obj", opts.verbose);
  graph->init();
  if ( graph->dijkstra(0, 1) == 20 )
    std::cout << "test Dijkstra for connected graph                      [\033[1;32mPASSED\033[0m]\n";
  else
  {
    std::cout << "test Dijkstra for connected graph                      [\033[1;31mFAILED\033[0m]\n";
    std::cout << "real distance: 20, distance found: " << graph->dijkstra(0, 1) << std::endl;
  }
  
  return 0;
}

