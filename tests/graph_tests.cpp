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

#include "gtest/gtest.h"

#define IGL_VIEWER_VIEWER_QUIET

#include "../graph_lib/graphStructure.hpp"
#include "../graph_lib/options.hpp"

#include <yaml-cpp/yaml.h>


/*
 * TEST GRAPH CONNECTIVITY
 */
TEST(graphConnectivity, disconnected)
{
    options opts;
    opts.loadYAML("../tests_config.yaml");

    Graph * graph;

    // disconnected graph
    graph = new Graph("../data/graphs_test/6_disconnected.obj", opts);
    graph->init();
    
    EXPECT_EQ(graph->is_connected(), false);
    EXPECT_EQ(graph->is_biconnected(), false);
    EXPECT_EQ(graph->is_triconnected(), false);
    if (opts.visualization)
        graph->plot();
}

TEST(graphConnectivity, connected)
{
    options opts;
    opts.loadYAML("../tests_config.yaml");

    Graph * graph;

    // connected graph
    graph = new Graph("../data/graphs_test/3_1_node_connectetivity.obj", opts);
    graph->init();
    
    EXPECT_EQ(graph->is_connected(), true);
    EXPECT_EQ(graph->is_biconnected(), false);
    EXPECT_EQ(graph->is_triconnected(), false);
    if (opts.visualization)
        graph->plot();
}

TEST(graphConnectivity, biconnected)
{
    options opts;
    opts.loadYAML("../tests_config.yaml");

    Graph * graph;

    // biconnected graph
    graph = new Graph("../data/graphs_test/5_2_edge_connectetivity.obj", opts);
    graph->init();
    
    EXPECT_EQ(graph->is_connected(), true);
    EXPECT_EQ(graph->is_biconnected(), true);
    EXPECT_EQ(graph->is_triconnected(), false);
    if (opts.visualization)
        graph->plot();
}

TEST(graphConnectivity, triconnected)
{
    options opts;
    opts.loadYAML("../tests_config.yaml");

    Graph * graph;

    // triconnected graph
    graph = new Graph("../data/graphs_test/0_graph_complete.obj", opts);
    graph->init();
    
    EXPECT_EQ(graph->is_connected(), true);
    EXPECT_EQ(graph->is_biconnected(), true);
    EXPECT_EQ(graph->is_triconnected(), true);
    if (opts.visualization)
        graph->plot();
}


/*
 * TEST Dijkstra
 */
TEST(graphDikjstra, unreachable)
{
    options opts;
    opts.loadYAML("../tests_config.yaml");

    Graph * graph;
    graph = new Graph("../data/graphs_test/6_disconnected.obj", opts);
    graph->init();

    EXPECT_EQ(graph->dijkstra(0, 1), std::numeric_limits<double>::infinity());
    if (opts.visualization)
        graph->plot();
}

TEST(graphDikjstra, reachable)
{
    options opts;
    opts.loadYAML("../tests_config.yaml");

    Graph * graph;
    graph = new Graph("../data/graphs_test/0_graph_complete.obj", opts);
    graph->init();
    
    EXPECT_EQ(graph->dijkstra(0, 1), 20);
    if (opts.visualization)
        graph->plot();
}
