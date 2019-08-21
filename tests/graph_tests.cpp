/**
 * main for testing the graphs
 * by R. Falque
 * 20/08/2019
 **/

// dependencies
#include <cstdlib>
#include <iostream>
#include <limits>

#include <Eigen/Core>

#include "gtest/gtest.h"

#include "libGraphCpp/graph.hpp"
#include "libGraphCpp/graphOptions.hpp"

#include <yaml-cpp/yaml.h>

/*
 * TEST CONSTRUCTORS
 */
TEST(constructors, string)
{
    libgraphcpp::Graph graph("../data/graphs_test/0_graph_complete.obj");

    EXPECT_EQ(graph.num_nodes(), 36);
    EXPECT_EQ(graph.num_edges(), 68);
}

TEST(constructors, stringAndOpts)
{
    graphOptions opts;
    opts.loadYAML("../tests_config.yaml");

    libgraphcpp::Graph graph("../data/graphs_test/0_graph_complete.obj", opts);

    EXPECT_EQ(graph.num_nodes(), 36);
    EXPECT_EQ(graph.num_edges(), 68);
}

TEST(constructors, edgeList)
{
    Eigen::MatrixXd nodes;
    nodes.resize(5, 3);
    nodes << 1, 2, 3,
             4, 5, 6,
             1, 8, 9,
             2, 8, 9,
             3, 8, 9;

    Eigen::MatrixXi edges;
    edges.resize(4, 2);
    edges << 0, 1,
             1, 2,
             2, 3,
             3, 4;

    libgraphcpp::Graph graph(nodes, edges);

    EXPECT_EQ(graph.num_nodes(), 5);
    EXPECT_EQ(graph.num_edges(), 4);
}

TEST(constructors, edgeListAndOpts)
{
    graphOptions opts;
    opts.loadYAML("../tests_config.yaml");

    Eigen::MatrixXd nodes;
    nodes.resize(5, 3);
    nodes << 1, 2, 3,
             4, 5, 6,
             1, 8, 9,
             2, 8, 9,
             3, 8, 9;

    Eigen::MatrixXi edges;
    edges.resize(4, 2);
    edges << 0, 1,
             0, 2,
             0, 3,
             0, 4;

    libgraphcpp::Graph graph(nodes, edges, opts);

    EXPECT_EQ(graph.num_nodes(), 5);
    EXPECT_EQ(graph.num_edges(), 4);
}

TEST(constructors, adjacencyMatrix)
{
    Eigen::MatrixXd nodes;
    nodes.resize(4, 3);
    nodes << 1, 2, 3,
             4, 5, 6,
             1, 8, 9,
             2, 8, 9;

    Eigen::MatrixXi edges;
    edges.resize(4, 4);
    edges << 0, 1, 0, 1,
             1, 0, 0, 1,
             0, 0, 0, 1,
             1, 1, 1, 0;

    libgraphcpp::Graph graph(nodes, edges);

    EXPECT_EQ(graph.num_nodes(), 4);
    EXPECT_EQ(graph.num_edges(), 4);
}

TEST(constructors, adjacencyMatrixAndOpts)
{
    graphOptions opts;
    opts.loadYAML("../tests_config.yaml");

    Eigen::MatrixXd nodes;
    nodes.resize(4, 3);
    nodes << 1, 2, 3,
             4, 5, 6,
             1, 8, 9,
             2, 8, 9;

    Eigen::MatrixXi edges;
    edges.resize(4, 4);
    edges << 0, 1, 0, 1,
             1, 0, 0, 1,
             0, 0, 0, 1,
             1, 1, 1, 0;

    libgraphcpp::Graph graph(nodes, edges, opts);

    EXPECT_EQ(graph.num_nodes(), 4);
    EXPECT_EQ(graph.num_edges(), 4);
}


/*
 * TEST GRAPH CONNECTIVITY
 */
TEST(graphConnectivity, disconnected)
{
    libgraphcpp::Graph graph("../data/graphs_test/6_disconnected.obj");

    EXPECT_EQ(graph.is_connected(), false);
    EXPECT_EQ(graph.is_biconnected(), false);
    EXPECT_EQ(graph.is_triconnected(), false);
}

TEST(graphConnectivity, connected)
{
    libgraphcpp::Graph graph("../data/graphs_test/3_1_node_connectetivity.obj");
    
    EXPECT_EQ(graph.is_connected(), true);
    EXPECT_EQ(graph.is_biconnected(), false);
    EXPECT_EQ(graph.is_triconnected(), false);
}

TEST(graphConnectivity, biconnected)
{
    libgraphcpp::Graph graph("../data/graphs_test/5_2_edge_connectetivity.obj");

    EXPECT_EQ(graph.is_connected(), true);
    EXPECT_EQ(graph.is_biconnected(), true);
    EXPECT_EQ(graph.is_triconnected(), false);
}

TEST(graphConnectivity, triconnected)
{
    libgraphcpp::Graph graph("../data/graphs_test/0_graph_complete.obj");

    EXPECT_EQ(graph.is_connected(), true);
    EXPECT_EQ(graph.is_biconnected(), true);
    EXPECT_EQ(graph.is_triconnected(), true);
}


/*
 * TEST Dijkstra
 */
TEST(graphDikjstra, unreachable)
{
    libgraphcpp::Graph graph("../data/graphs_test/6_disconnected.obj");

    EXPECT_EQ(graph.dijkstra(0, 1), std::numeric_limits<double>::infinity());
}

TEST(graphDikjstra, reachable)
{
    libgraphcpp::Graph graph("../data/graphs_test/0_graph_complete.obj");

    EXPECT_EQ(graph.dijkstra(0, 1), 20);
}
