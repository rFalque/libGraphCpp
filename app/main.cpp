/**
 * basic usage of the library
 * by R. Falque
 * 22/06/2019
 **/

#include "libGraphCpp/graph.hpp"

//#include "libGraphCpp/readGraphOBJ.hpp"
//#include "libGraphCpp/writeGraphOBJ.hpp"
//#include "libGraphCpp/plotGraph.hpp"
//#include "libGraphCpp/graphOptions.hpp"


#include <yaml-cpp/yaml.h>

int main(int argc, char* argv[])
{
    // create a graph
    graphOptions opts;
    opts.loadYAML("../config.yaml");

    libgraphcpp::Graph graph(opts.path_graph_obj, opts);


    // plot the graph
    graph.plot();


    // perform connectivity tests
    bool is_connected, is_biconnected, is_triconnected, has_bridges;

    std::vector<int> one_cut_vertices;
    std::vector<std::pair<int, int>> two_cut_vertices;
    std::vector<int> bridges;

    is_connected = graph.is_connected();
    is_biconnected = graph.is_biconnected(one_cut_vertices);
    is_triconnected = graph.is_triconnected(two_cut_vertices);
    has_bridges = graph.has_bridges(bridges);


    // highlight some specific nodes and edges:
    std::vector<int> vertices_to_highlight;
    for (int v: one_cut_vertices)
        vertices_to_highlight.push_back(v);
    for (auto pair: two_cut_vertices) {
        vertices_to_highlight.push_back(pair.first);
        vertices_to_highlight.push_back(pair.second);
    }
    std::sort( vertices_to_highlight.begin(), vertices_to_highlight.end() ); // move that in the library?
    vertices_to_highlight.erase( std::unique( vertices_to_highlight.begin(), vertices_to_highlight.end() ), vertices_to_highlight.end() );

    graph.plot_and_highlight(vertices_to_highlight, bridges);
    

    // plot the overall connectivity
    graph.plot_connectivity();


    // turn the graph into a tree
    graph.make_tree();
    graph.plot();


    // save the graph
    graph.save("../data/output.obj");
    std::cout << "graph saved at : ../data/output.obj\n";

    return 0;
}
