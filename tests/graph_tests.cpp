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

#include "libGraphCpp/graph.hpp"
#include "libGraphCpp/graphOptions.hpp"

#include <yaml-cpp/yaml.h>


/*
 * TEST GRAPH CONNECTIVITY
 */
TEST(graphConnectivity, disconnected)
{
    graphOptions opts;
    opts.loadYAML("../tests_config.yaml");

    libgraphcpp::Graph * graph;

    // disconnected graph
    graph = new libgraphcpp::Graph("../data/graphs_test/6_disconnected.obj", opts);
    graph->init();
    
    EXPECT_EQ(graph->is_connected(), false);
    EXPECT_EQ(graph->is_biconnected(), false);
    EXPECT_EQ(graph->is_triconnected(), false);
    if (opts.visualization)
        graph->plot();
}

TEST(graphConnectivity, connected)
{
    graphOptions opts;
    opts.loadYAML("../tests_config.yaml");

    libgraphcpp::Graph * graph;

    // connected graph
    graph = new libgraphcpp::Graph("../data/graphs_test/3_1_node_connectetivity.obj", opts);
    graph->init();
    
    EXPECT_EQ(graph->is_connected(), true);
    EXPECT_EQ(graph->is_biconnected(), false);
    EXPECT_EQ(graph->is_triconnected(), false);
    if (opts.visualization)
        graph->plot();
}

TEST(graphConnectivity, biconnected)
{
    graphOptions opts;
    opts.loadYAML("../tests_config.yaml");

    libgraphcpp::Graph * graph;

    // biconnected graph
    graph = new libgraphcpp::Graph("../data/graphs_test/5_2_edge_connectetivity.obj", opts);
    graph->init();
    
    EXPECT_EQ(graph->is_connected(), true);
    EXPECT_EQ(graph->is_biconnected(), true);
    EXPECT_EQ(graph->is_triconnected(), false);
    if (opts.visualization)
        graph->plot();
}

TEST(graphConnectivity, triconnected)
{
    graphOptions opts;
    opts.loadYAML("../tests_config.yaml");

    libgraphcpp::Graph * graph;

    // triconnected graph
    graph = new libgraphcpp::Graph("../data/graphs_test/0_graph_complete.obj", opts);
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
    graphOptions opts;
    opts.loadYAML("../tests_config.yaml");

    libgraphcpp::Graph * graph;
    graph = new libgraphcpp::Graph("../data/graphs_test/6_disconnected.obj", opts);
    graph->init();

    EXPECT_EQ(graph->dijkstra(0, 1), std::numeric_limits<double>::infinity());
    if (opts.visualization)
        graph->plot();
}

TEST(graphDikjstra, reachable)
{
    graphOptions opts;
    opts.loadYAML("../tests_config.yaml");

    libgraphcpp::Graph * graph;
    graph = new libgraphcpp::Graph("../data/graphs_test/0_graph_complete.obj", opts);
    graph->init();
    
    EXPECT_EQ(graph->dijkstra(0, 1), 20);
    if (opts.visualization)
        graph->plot();
}
