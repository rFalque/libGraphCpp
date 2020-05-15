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

#include <string>
#include <Eigen/Core>

#include "graphOptions.hpp"

#include "polyscope/polyscope.h"
#include "polyscope/messages.h"
#include "polyscope/point_cloud.h"
#include "polyscope/surface_mesh.h"
#include "polyscope/curve_network.h"

#ifndef POLYSCOPE_IS_INITIALIZED
#define POLYSCOPE_IS_INITIALIZED
int polyscope_is_initialized { 0 };
int polyscope_object_counter { 0 };
#endif

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

#endif
