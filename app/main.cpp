/**
 * basic usage of the library
 * by R. Falque
 * 22/06/2019
 **/

#include "polyscope/polyscope.h"
#include "libGraphCpp/graph.hpp"
#include "libGraphCpp/plotGraph.hpp"

#include <yaml-cpp/yaml.h>

int main(int argc, char* argv[])
{
    polyscope::init();

    std::cout << "Progress: load options\n";
    graphOptions opts;
    opts.loadYAML("../config.yaml");


    std::cout << "Progress: load graph\n";
    libgraphcpp::Graph graph(opts.path_graph_obj, opts);


    std::cout << "Progress: plot graph\n";
    visualization::plot(graph);


    std::cout << "Progress: test connectivity\n";
    bool is_connected, is_biconnected, is_triconnected, has_bridges;

    std::vector<int> one_cut_vertices;
    std::vector<std::pair<int, int>> two_cut_vertices;
    std::vector<int> bridges;

    is_connected = graph.is_connected();
    is_biconnected = graph.is_biconnected(one_cut_vertices);
    is_triconnected = graph.is_triconnected(two_cut_vertices);
    has_bridges = graph.has_bridges(bridges);


    std::cout << "Progress: plot connectivity (example in the code)\n";
    std::vector<int> vertices_to_highlight;
    for (int v: one_cut_vertices)
        vertices_to_highlight.push_back(v);
    for (auto pair: two_cut_vertices) {
        vertices_to_highlight.push_back(pair.first);
        vertices_to_highlight.push_back(pair.second);
    }
    std::sort( vertices_to_highlight.begin(), vertices_to_highlight.end() ); // move that in the library?
    vertices_to_highlight.erase( std::unique( vertices_to_highlight.begin(), vertices_to_highlight.end() ), vertices_to_highlight.end() );

    visualization::plot_and_highlight(graph, vertices_to_highlight, bridges);
    

    std::cout << "Progress: plot connectivity (from the library)\n";
    visualization::plot_connectivity(graph);


    std::cout << "Progress: simplify graph\n";
    std::vector <std::vector<int>> cycle_basis;
    std::vector <double> cycle_length;


    graph.remove_flat_triangles();

    std::cout << "graph has cycles: " << graph.has_cycles(cycle_basis, cycle_length) << std::endl;
    for (int i=0; i<cycle_basis.size(); i++)
    {
        std::cout << "cycle number[" << i << "] with length: "<<cycle_length[i] << ", has for path: ";
        for (int node_id : cycle_basis[i])
            std::cout << node_id << " ";
        std::cout << std::endl;

    }

    //graph.make_tree();
    graph.symplify_graph();
    visualization::plot(graph);

    std::cout << "graph has cycles: " << graph.has_cycles() << std::endl;

    std::cout << "Progress: write output\n";
    graph.save("../data/output.obj");
    std::cout << "graph saved at : ../data/output.obj\n";

    return 0;
}
