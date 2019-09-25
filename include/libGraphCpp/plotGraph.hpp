/**
 * creates a mesh of small cylinders and spheres to visualize the graph
 * Inputs:
 *  nodes:        #N by 3 coordinates of the endpoints of the cylinders
 *  edges:        #E by 2 coordinates of the endpoints of the cylinders
 *  nodesColors:  #N by 3 RBG colors per cylinder
 *  edgesColors:  #E by 3 RBG colors per cylinder
 * Outputs:
 *  plot the graph with polyscope
 * 
 * by R. Falque
 * 20/08/2019
 **/

#ifndef PLOT_GRAPH_HPP
#define PLOT_GRAPH_HPP

#include <Eigen/Core>

#include "graphOptions.hpp"

#include "polyscope/polyscope.h"
#include "polyscope/messages.h"
#include "polyscope/point_cloud.h"
#include "polyscope/surface_mesh.h"
#include "polyscope/curve_network.h"

bool polyscope_is_initialized = false;

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
                polyscope::view::windowWidth = 1024;
                polyscope::view::windowHeight = 1024;

                // Initialize polyscope   
                if (not ::polyscope_is_initialized) {
                    polyscope::init();
                    ::polyscope_is_initialized = true;
                }
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
}



#endif