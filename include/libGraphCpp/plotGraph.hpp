/**
 * aditional functions to plot a graph
 * 
 * by R. Falque
 * 20/08/2019
 **/

#ifndef PLOT_GRAPH_HPP
#define PLOT_GRAPH_HPP

#include <string>
#include <Eigen/Core>

#include "graphOptions.hpp"
#include "graph.hpp"
#include "polyscopeWrapper.hpp"

namespace visualization {

    void add_graph(libgraphcpp::Graph graph, std::string graph_name) {
        Eigen::MatrixXd nodes = graph.get_nodes();
        Eigen::MatrixXi edges = graph.get_edges();

        visualization::init();
        visualization::add_graph(nodes, edges, graph_name);
    };

    void add_graph(libgraphcpp::Graph graph) {
        visualization::add_graph(graph, "graph");
    };

    void plot(libgraphcpp::Graph graph, std::string graph_name) {
        visualization::add_graph(graph, graph_name);
        visualization::show();
        visualization::clear_structures();
    };

    void plot(libgraphcpp::Graph graph) {
        visualization::plot(graph, "graph");
    };


    void return_colors_highlight(std::vector<int> element_to_highlight, int size_array, Eigen::MatrixXd & colors) {
        colors = Eigen::MatrixXd::Constant(size_array,3,0.1);

        for (int element: element_to_highlight) {
            colors(element, 0) = 0.1;
            colors(element, 1) = 0.1;
            colors(element, 2) = 1.0;
        }
    };

    void plot_connectivity(libgraphcpp::Graph graph) {

        graph.connectivity_tests();
        Eigen::MatrixXd empty; 			// used to pass empty content
        Eigen::MatrixXd highlights;

        visualization::init();
        visualization::clear_structures();
        visualization::add_graph(graph, "graph_connectivity");

        // add color for the one cut vertices
        std::vector<int> one_cut_vertices;
        graph.is_biconnected(one_cut_vertices);
        return_colors_highlight(one_cut_vertices, graph.num_nodes(), highlights);
        visualization::add_color_to_graph(highlights, empty, "graph_connectivity", "one_cut_vertices");

        // add color for the two cut vertices
        std::vector< std::pair<int, int> > two_cut_vertices;
        graph.is_triconnected(two_cut_vertices);
        std::vector <int> temp;
        for (std::pair<int, int> p: two_cut_vertices) {
            temp.push_back(p.first);
            temp.push_back(p.second);
        }
        return_colors_highlight(temp, graph.num_nodes(), highlights);
        visualization::add_color_to_graph(highlights, empty, "graph_connectivity", "two_cut_vertices");

        // add color for the bridges
        std::vector<int> bridges;
        graph.has_bridges(bridges);
        return_colors_highlight(temp, graph.num_edges(), highlights);
        visualization::add_color_to_graph(empty, highlights, "graph_connectivity", "bridges");

        visualization::show();
        visualization::clear_structures();
    };

    void plot_and_highlight(libgraphcpp::Graph graph, std::vector<int> node_list, std::vector<int> edge_list)
    {
        visualization::init();
        visualization::clear_structures();
        visualization::add_graph(graph, "graph");

        Eigen::MatrixXd nodes_colors, edges_colors;


        return_colors_highlight(node_list, graph.num_nodes(), nodes_colors);
        return_colors_highlight(edge_list, graph.num_edges(), edges_colors);

        visualization::add_color_to_graph(nodes_colors, edges_colors, "graph", "highlight");
        visualization::show();

    };

}

/*
namespace libgraphcpp
{
    // polyscope wrapper
    class GraphVisualization
    {
        private:
            std::string graph = "graph";
            std::string graphNodes_ = "graphNodes";
            std::string graphEdges_ = "graphEdges";

        public:

            GraphVisualization()
            {
                init();
            }

            // destructor
            ~GraphVisualization()
            {
            }

            void init() {
                // Options
                //polyscope::options::autocenterStructures = true;

                if (!polyscope_is_initialized) {
                    polyscope::init();
                    polyscope_is_initialized = 1;
                }

                polyscope::view::windowWidth = 1024;
                polyscope::view::windowHeight = 1024;
                polyscope::view::style = polyscope::view::NavigateStyle::Free;
            }

            void add_graph(const Eigen::MatrixXd & nodes,
                           const Eigen::MatrixXi & edges) {
                polyscope::registerPointCloud(graphNodes_, nodes);
                polyscope::getPointCloud(graphNodes_)->pointColor = glm::vec3{1, 0, 0};
                polyscope::registerCurveNetwork(graphEdges_, nodes, edges);
                polyscope::getCurveNetwork(graphEdges_)->baseColor = glm::vec3{0, 0, 0};
            }

            void add_color(const Eigen::MatrixXd & nodes_colors,
                           const Eigen::MatrixXd & edges_colors,
                           std::string color_name) {
                if (nodes_colors.rows() != 0){
                    polyscope::getPointCloud(graphNodes_)->addColorQuantity(color_name, nodes_colors);
                    polyscope::getPointCloud(graphNodes_)->getQuantity(color_name)->setEnabled(true);
                }
                if (edges_colors.rows() != 0){
                    polyscope::getCurveNetwork(graphEdges_)->addEdgeColorQuantity(color_name, edges_colors);
                    polyscope::getCurveNetwork(graphEdges_)->getQuantity(color_name)->setEnabled(true);
                }
            }

            void add_vector_quantity(const Eigen::VectorXd & nodes_colors,
                                    const Eigen::VectorXd & edges_colors,
                                    std::string color_name) {
                if (nodes_colors.rows() != 0){
                    polyscope::getPointCloud(graphNodes_)->addVectorQuantity(color_name, nodes_colors);
                    polyscope::getPointCloud(graphNodes_)->getQuantity(color_name)->setEnabled(true);
                }
                if (edges_colors.rows() != 0){
                    polyscope::getCurveNetwork(graphEdges_)->addEdgeVectorQuantity(color_name, edges_colors);
                    polyscope::getCurveNetwork(graphEdges_)->getQuantity(color_name)->setEnabled(true);
                }
            }


            void add_scalar_quantity(const Eigen::VectorXd & nodes_colors,
                                    const Eigen::VectorXd & edges_colors,
                                    std::string color_name) {
                if (nodes_colors.rows() != 0){
                    polyscope::getPointCloud(graphNodes_)->addScalarQuantity(color_name, nodes_colors);
                    polyscope::getPointCloud(graphNodes_)->getQuantity(color_name)->setEnabled(true);
                }
                if (edges_colors.rows() != 0){
                    polyscope::getCurveNetwork(graphEdges_)->addEdgeScalarQuantity(color_name, edges_colors);
                    polyscope::getCurveNetwork(graphEdges_)->getQuantity(color_name)->setEnabled(true);
                }
            }

            void show() {
                polyscope::show();
            }
    };

    inline bool polyscope_plot(const Eigen::MatrixXd & nodes,
                            const Eigen::MatrixXi & edges)
    {
        GraphVisualization viz;
        viz.add_graph(nodes, edges);
        viz.show();
        return true;
    };

    inline bool polyscope_plot_with_color(const Eigen::MatrixXd & nodes,
                                        const Eigen::MatrixXi & edges,
                                        const Eigen::MatrixXd & nodes_colors,
                                        const Eigen::MatrixXd & edges_colors)
    {
        GraphVisualization viz;
        viz.add_graph(nodes, edges);
        viz.add_color(nodes_colors, edges_colors, "highlight");
        viz.show();
        return true;
    };

    inline bool polyscope_plot_with_color(const Eigen::MatrixXd & nodes,
                                        const Eigen::MatrixXi & edges,
                                        const Eigen::VectorXd & nodes_colors,
                                        const Eigen::VectorXd & edges_colors)
    {
        GraphVisualization viz;
        viz.add_graph(nodes, edges);
        viz.add_scalar_quantity(nodes_colors, edges_colors, "highlight");
        viz.show();
        return true;
    };

    inline bool add_graph_with_scalar_to_plot(const Eigen::MatrixXd & nodes,
                                            const Eigen::MatrixXi & edges,
                                            const Eigen::VectorXd & nodes_colors,
                                            const Eigen::VectorXd & edges_colors)
    {
        std::string n = std::to_string(polyscope_object_counter);

        polyscope::registerPointCloud("graph" + n, nodes);
        polyscope::getPointCloud("graph" + n)->pointColor = glm::vec3{1, 0, 0};
        polyscope::registerCurveNetwork("edges" + n, nodes, edges);
        polyscope::getCurveNetwork("edges" + n)->baseColor = glm::vec3{0, 0, 0};

        if (nodes_colors.rows() != 0){
            polyscope::getPointCloud("graph" + n)->addScalarQuantity("highlight" + n, nodes_colors);
            polyscope::getPointCloud("graph" + n)->getQuantity("highlight" + n)->setEnabled(true);
        }
        if (edges_colors.rows() != 0){
            polyscope::getCurveNetwork("edges" + n)->addEdgeScalarQuantity("highlight" + n, edges_colors);
            polyscope::getCurveNetwork("edges" + n)->getQuantity("highlight" + n)->setEnabled(true);
        }
        polyscope_object_counter ++;
    };
    
}
*/

#endif
