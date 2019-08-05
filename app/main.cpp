/**
 * basic usage of the library
 * by R. Falque
 * 22/06/2019
 **/

#include "libGraphCpp/graph.hpp"
#include "libGraphCpp/readGraphOBJ.hpp"
#include "libGraphCpp/plotGraph.hpp"
#include "libGraphCpp/graphOptions.hpp"

#include <yaml-cpp/yaml.h>

int main(int argc, char* argv[])
{
    graphOptions opts;
    opts.loadYAML("../config.yaml");

    libgraphcpp::Graph graph(opts.path_graph_obj, opts);
    graph.init();

    bool is_connected, is_biconnected, is_triconnected, has_bridges;

    std::vector<int> one_cut_vertices;
    std::vector<std::pair<int, int>> two_cut_vertices;
    std::vector<int> bridges;
    
    is_connected = graph.is_connected();
    is_biconnected = graph.is_biconnected(one_cut_vertices);
    is_triconnected = graph.is_triconnected(two_cut_vertices);
    has_bridges = graph.has_bridges(bridges);

    std::vector<int> vertices_to_highlight;
    for (int v: one_cut_vertices)
        vertices_to_highlight.push_back(v);
    
    for (auto pair: two_cut_vertices) {
        vertices_to_highlight.push_back(pair.first);
        vertices_to_highlight.push_back(pair.second);
    }

    std::sort( vertices_to_highlight.begin(), vertices_to_highlight.end() );
    vertices_to_highlight.erase( std::unique( vertices_to_highlight.begin(), vertices_to_highlight.end() ), vertices_to_highlight.end() );

    graph.plot_and_highlight(vertices_to_highlight, bridges);

    graph.plot();

    std::cout << "Make the graph one dimentional:\n";
    graph.make_1D_curve();
    graph.plot();

    return 0;
}
