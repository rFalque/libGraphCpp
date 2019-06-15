#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <algorithm>     // std::all_of
#include <Eigen/Dense>
#include <limits>
#include "readGraphOBJ.hpp"
#include "plotGraph.hpp"


/* TODO: check adjacency_list
 *       add linear triconnectivity implementation
 */

class Graph
{
private:
	Eigen::MatrixXd nodes_;
	Eigen::MatrixXi edges_;

	std::vector< std::vector<int> > adjacency_list_;
	std::vector< std::vector<int> > adjacency_edge_list_;
	Eigen::VectorXd edges_length_;

	int num_nodes_;
	int num_edges_;
	
	int is_connected_ = -1; // 0 no, 1 yes, -1 undefined
	int is_biconnected_ = -1; // 0 no, 1 yes, -1 undefined
	int is_triconnected_ = -1; // 0 no, 1 yes, -1 undefined
	int has_bridges_ = -1; // 0 no, 1 yes, -1 undefined

	bool verbose_ = true;

	std::vector< std::pair<int, int> > bridges_;
	std::vector< int > one_cut_vertices_; // articulation points
	std::vector< std::pair<int, int> > two_cut_vertices_;
	
	void removeRow(Eigen::MatrixXd& matrix, unsigned int rowToRemove);
	void removeRow(Eigen::MatrixXi& matrix, unsigned int rowToRemove);
	void removeDuplicates(std::vector<std::pair<int, int>>& v);
	void DFSUtil(int u, std::vector< std::vector<int> > adj, std::vector<bool> &visited);
	void APUtil(int u, std::vector<bool> & visited, int disc[], int low[], std::vector<int> & parent, std::vector<bool> & ap);
	void bridgeUtil(int u, std::vector<bool> & visited, int disc[], int low[], std::vector<int> & parent, std::vector< std::pair<int, int> > & bridges);
	bool is_element_in_vector(int a, std::vector<int> & A, int & element_position);

public:

	Graph(std::string file_name){
		readGraphOBJ(file_name, nodes_, edges_);
	}

	Graph(std::string file_name, bool verbose){
		readGraphOBJ(file_name, nodes_, edges_);
		verbose_ = verbose;
	}

	Graph(Eigen::MatrixXd nodes, Eigen::MatrixXi edges){
		nodes_ = nodes;
		edges_ = edges;
	}

	Graph(Eigen::MatrixXd nodes, Eigen::MatrixXi edges, bool verbose){
		nodes_ = nodes;
		edges_ = edges;
		verbose_ = verbose;
	}

	~Graph(){
	}

	bool init()
	{
		if (nodes_.cols()!=3 || edges_.cols()!=2)
		{
			std::cout << "Error: wrong graph dimensions" << std::endl;
			std::exit(1);
		}

		adjacency_list_.clear();		// for each node, stors adjacent nodes
		adjacency_edge_list_.clear();	// for each edges, store adjacent edges
		bridges_.clear();

		// set up properties
		num_nodes_ = nodes_.rows();
		num_edges_ = edges_.rows();

		// set up edges_length
		edges_length_ = Eigen::VectorXd::Zero(num_edges_);
		for (int i=0; i<num_edges_; i++)
			edges_length_(i) = (nodes_.row(edges_(i,0)) - nodes_.row(edges_(i,1)) ).norm();
		
		// set up the adjacency list
		set_adjacency_lists();

		return true;
	}


	void set_adjacency_lists()
	{
		// TODO: - the if statement should be removed for undirected graphs as they are not needed
		//       - is the adjacency_edge_list_ needed?
		adjacency_list_.resize(num_nodes_);
		adjacency_edge_list_.resize(num_nodes_);
		
		for (int i=0; i<num_edges_; i++)
		{
			// the if statements check if the node has already been inserted
			if (std::find (adjacency_list_[edges_(i, 0)].begin(), adjacency_list_[edges_(i, 0)].end(), edges_(i, 1))==adjacency_list_[edges_(i, 0)].end())
				adjacency_list_[edges_(i, 0)].push_back(edges_(i, 1));
			if (std::find (adjacency_list_[edges_(i, 1)].begin(), adjacency_list_[edges_(i, 1)].end(), edges_(i, 0))==adjacency_list_[edges_(i, 1)].end())
				adjacency_list_[edges_(i, 1)].push_back(edges_(i, 0));
			adjacency_edge_list_[edges_(i, 0)].push_back(i);
			adjacency_edge_list_[edges_(i, 1)].push_back(i);
		}
	}

	bool plot()
	{
		return directional::plot_graph(nodes_, edges_);
	}

	bool augment_connectivity()
	{
		// TBD
		return true;
	}

	int num_nodes(){
		return num_nodes_;
	}

	int num_edges(){
		return num_edges_;
	}

	bool is_connected(){
		if (is_connected_ == -1)
		{
			// check for graph connectivity using DFS:
			std::vector<bool> visited(num_nodes_, false);
			DFSUtil(0, adjacency_list_, visited);
			is_connected_ = std::all_of(visited.begin(), visited.end(), [](bool v) { return v; });

			if (verbose_)
				std::cout << "graph is connected: " << is_connected_ << std::endl;

			// if not connected, update the next ones
			if (is_connected_ == 0)
			{
				is_biconnected_ = 0;
				is_triconnected_ = 0;
			}
		}
		return is_connected_;
	}

	// return the set of one cut vertices
	bool is_biconnected(std::vector< int >& one_cut_vertices){
		if (is_biconnected_ == -1)
		{
			// check for 2 node connectivity:
			std::vector<bool> visited(num_nodes_, false);
			std::vector<int> parent(num_nodes_, -1);
			std::vector<bool> ap(num_nodes_, false);
			int *disc = new int[num_nodes_];
			int *low = new int[num_nodes_];
			// Call the recursive helper function to find articulation points 
			// in DFS tree rooted with vertex '0' 
			for (int i = 0; i < num_nodes_; i++) 
				if (visited[i] == false) 
					APUtil(i, visited, disc, low, parent, ap);
			
			// Now ap[] contains articulation points, print them and store them into one_cut_vertices_
			one_cut_vertices.clear();
			for (int i = 0; i < num_nodes_; i++) 
				if (ap[i] == true)
				{
					one_cut_vertices.push_back(i);
					if (verbose_)
						std::cout << "Articulation point " << i << " at: "  << nodes_.row(i) << std::endl;
				}
			is_biconnected_ = std::none_of(ap.begin(), ap.end(), [](bool v) { return v; });
			if (verbose_)
				std::cout << "graph is 2-connected: " << is_biconnected_ << std::endl;

			// if not biconnected, update triconnectivity
			if (is_biconnected_ == 0)
				is_triconnected_ = 0;
		}
		return is_biconnected_;
	}

	// overload is_biconnected
	bool is_biconnected(){
		std::vector< int > one_cut_vertices;
		return is_biconnected(one_cut_vertices);
	}

	// return the set of two cut vertices
	bool is_triconnected(std::vector< std::pair<int, int> >& two_cut_vertices){ 
		/* this function run in quadratic time, this is not the most efficient to do it
		 * for alternative, see these papers:
		 * - Finding the Triconnected Components of a Graph (1972)
		 * - A Linear Time Implementation of SPQR-Trees (2000)
		 * and these implementations:
		 * - http://www.ogdf.net/doku.php
		 * - https://github.com/adrianN/Triconnectivity
		 */
		if (is_triconnected_ == -1)
		{
			is_triconnected_ = true;
			for (int i=0; i<num_nodes_; i++)
			{
				std::vector< int > one_cut_vertices;
				Graph reduced_graph(nodes_, edges_, false);
				reduced_graph.init();
				reduced_graph.remove_node(i);
				bool reduced_graph_is_biconnected = reduced_graph.is_biconnected(one_cut_vertices);

				if (not reduced_graph_is_biconnected)
					for (int j=0; j<one_cut_vertices.size(); j++)
					{
						int node_offset = (one_cut_vertices[j]>=i); // offset needed because a node has been deleted
						two_cut_vertices.push_back(std::make_pair(i, one_cut_vertices[j]+node_offset));
					}
				is_triconnected_ &= reduced_graph_is_biconnected;
			}
		}

		removeDuplicates(two_cut_vertices);

		if (verbose_)
		{
			std::cout << "graph is 3-connected: " << is_triconnected_ << std::endl;
			for (int i = 0; i<two_cut_vertices.size(); i++)
				std::cout << "two_cut_vertices between node " << two_cut_vertices[i].first <<" and node " << two_cut_vertices[i].second << std::endl;
		}

		return is_triconnected_;
	}

	// overload is_triconnected
	bool is_triconnected(){ 
		std::vector< std::pair<int, int> > two_cut_vertices;
		return is_triconnected(two_cut_vertices);
	}

	// return the set of bridges
	bool has_bridges(std::vector< std::pair<int, int> > bridges){
		if (has_bridges_ == -1)
		{
			// check for bridges:

			std::vector<bool> visited(num_nodes_, false);
			std::vector<int> parent(num_nodes_, -1);
			int *disc = new int[num_nodes_]; 
			int *low = new int[num_nodes_];
			
			// Call the recursive helper function to find Bridges 
			// in DFS tree rooted with vertex 'i' 
			for (int i = 0; i < num_nodes_; i++) 
				if (visited[i] == false) 
					bridgeUtil(i, visited, disc, low, parent, bridges);
			
			delete[] disc;
			delete[] low;

			if (bridges.size() == 0)
				has_bridges_ = 0;
			else
				has_bridges_ = 1;
		}
		return has_bridges_;
	}

	// overload has_bridges
	bool has_bridges(){
		std::vector< std::pair<int, int> > bridges;
		return has_bridges(bridges);
	}

	int adj(int i, int j){
		return adjacency_list_[i][j];
	}

	int edge_source(int i){
		return edges_(i, 0);
	}

	int edge_target(int i){
		return edges_(i, 1);
	}

	void swap_edge(int i){
		int temp = edges_(i,0);
		edges_(i,0) = edges_(i,1);
		edges_(i,1) = temp;
	}

	void remove_node(int nodeToRemove){
		// remove node
		removeRow(nodes_, nodeToRemove);

		// remove related edges
		std::vector<int> edges_to_remove = adjacency_edge_list_[nodeToRemove];
		std::sort(edges_to_remove.begin(), edges_to_remove.end(), std::greater<int>());
		for (int edge_iterator=0; edge_iterator<edges_to_remove.size(); edge_iterator++)
			removeRow(edges_, edges_to_remove[edge_iterator]);

		// update the edge nodes index (we have one less node now)
		for (int i=0; i<edges_.rows(); i++)
			for (int j=0; j<edges_.cols(); j++)
				if (edges_(i,j) >= nodeToRemove)
					edges_(i,j)--;

		init();
	}

	double dijkstra(int source, int target){
		// initialization
		double distance_source_to_target;
		std::vector<double> min_distance(num_nodes_, std::numeric_limits<double>::infinity());
		min_distance.at(source) = 0;

		// set the set of visited nodes:
		std::vector<int> visited;
		std::vector<int> to_visit;

		// initialize the node to start from
		int u = source;
		int element_position;

		// start searching
		bool target_found;
		while (!target_found)
		{
			for (int i = 0; i < adjacency_list_.at(u).size(); ++i)
			{
				int neighbour_node = adjacency_list_.at(u).at(i);
				double edge_length = edges_length_(adjacency_edge_list_.at(u).at(i));
				if (not is_element_in_vector(neighbour_node, to_visit, element_position))
					if (not is_element_in_vector(neighbour_node, visited, element_position))
						to_visit.push_back(neighbour_node);
				
				if ( min_distance.at(u) + edge_length < min_distance.at( neighbour_node ) )
					min_distance.at( neighbour_node ) = min_distance.at(u) +  edge_length;
			}

			visited.push_back(u);

			// check if the visited point is in sub_V
			target_found = is_element_in_vector(target, visited, element_position);

			// set next u
			int index_of_next_point = 0;
			for (int i = 0; i < to_visit.size(); ++i)
				if (min_distance.at( to_visit.at(i) ) < min_distance.at( to_visit.at( index_of_next_point) ) )
					index_of_next_point = i;


			// check if all vertices have been visited:
			if (to_visit.size() == 0)
				break;

			u = to_visit.at(index_of_next_point);
			to_visit.erase(to_visit.begin() + index_of_next_point);

		}
		
		return min_distance.at(target);
	}

};

//https://stackoverflow.com/a/46303314/2562693
void Graph::removeRow(Eigen::MatrixXd& matrix, unsigned int rowToRemove)
{
    unsigned int numRows = matrix.rows()-1;
    unsigned int numCols = matrix.cols();

    if( rowToRemove < numRows )
        matrix.block(rowToRemove,0,numRows-rowToRemove,numCols) = matrix.bottomRows(numRows-rowToRemove);

    matrix.conservativeResize(numRows,numCols);
};
void Graph::removeRow(Eigen::MatrixXi& matrix, unsigned int rowToRemove)
{
    unsigned int numRows = matrix.rows()-1;
    unsigned int numCols = matrix.cols();

    if( rowToRemove < numRows )
        matrix.block(rowToRemove,0,numRows-rowToRemove,numCols) = matrix.bottomRows(numRows-rowToRemove);

    matrix.conservativeResize(numRows,numCols);
};

//https://stackoverflow.com/a/32842128/2562693
void Graph::removeDuplicates(std::vector<std::pair<int, int>>& v)
{
	
	//Normalize == sort the pair members
	for(auto& p : v){
		int x = std::max(p.first, p.second), y = std::min(p.first, p.second);
		p.first = x; p.second = y;
	}

	//Sort the pairs
	std::sort(v.begin(), v.end());

	//Unique the vector
	auto last = unique(v.begin(), v.end() );
	v.erase(last, v.end());
};

// used for checking connectivity:
//
// for reference, see Kosaraju's algorithm (https://en.wikipedia.org/wiki/Kosaraju%27s_algorithm)
//
// from: https://www.geeksforgeeks.org/graph-implementation-using-stl-for-competitive-programming-set-1-dfs-of-unweighted-and-undirected/
// A utility function to do DFS of graph 
// recursively from a given vertex u. 
void Graph::DFSUtil(int u, std::vector< std::vector<int> > adj, std::vector<bool> &visited) 
{ 
	visited[u] = true;
	for (int i=0; i<adj[u].size(); i++) 
		if (visited[adj[u][i]] == false) 
			DFSUtil(adj[u][i], adj, visited); 
};

// used for finding articulation points (1-node-connectivity):
//
// for reference, see Tarjan’s algorithm
//
// from: https://www.geeksforgeeks.org/articulation-points-or-cut-vertices-in-a-graph/
// A recursive function that find articulation points using DFS traversal 
// u --> The vertex to be visited next 
// visited[] --> keeps tract of visited vertices 
// disc[] --> Stores discovery times of visited vertices 
// parent[] --> Stores parent vertices in DFS tree 
// ap[] --> Store articulation points 
void Graph::APUtil(int u, std::vector<bool> & visited, int disc[],  
									int low[], std::vector<int> & parent, std::vector<bool> & ap) 
{ 
	// A static variable is used for simplicity, we can avoid use of static 
	// variable by passing a pointer. 
	static int time = 0; 

	// Count of children in DFS Tree 
	int children = 0; 

	// Mark the current node as visited 
	visited[u] = true; 

	// Initialize discovery time and low value 
	disc[u] = low[u] = ++time; 

	for (int i=0; i<adjacency_list_[u].size(); i++)
	{ 
		int v = adjacency_list_[u][i];  // v is current adjacent of u 

		// If v is not visited yet, then make it a child of u 
		// in DFS tree and recur for it 
		if (!visited[v]) 
		{ 
			children++; 
			parent[v] = u; 
			APUtil(v, visited, disc, low, parent, ap); 

			// Check if the subtree rooted with v has a connection to 
			// one of the ancestors of u 
			low[u]  = std::min(low[u], low[v]); 

			// u is an articulation point in following cases 

			// (1) u is root of DFS tree and has two or more chilren. 
			if (parent[u] == -1 && children > 1) 
			ap[u] = true; 

			// (2) If u is not root and low value of one of its child is more 
			// than discovery value of u. 
			if (parent[u] != -1 && low[v] >= disc[u]) 
			ap[u] = true; 
		} 

		// Update low value of u for parent function calls. 
		else if (v != parent[u]) 
			low[u]  = std::min(low[u], disc[v]); 
	} 
};

// used for finding articulation points (1-node-connectivity):
//
// for reference, see Tarjan’s algorithm
//
// from: https://www.geeksforgeeks.org/bridge-in-a-graph/
// A recursive function that finds and prints bridges using 
// DFS traversal 
// u --> The vertex to be visited next 
// visited[] --> keeps tract of visited vertices 
// disc[] --> Stores discovery times of visited vertices 
// parent[] --> Stores parent vertices in DFS tree
void Graph::bridgeUtil(int u, 
					   std::vector<bool> & visited, 
					   int disc[], 
					   int low[], 
					   std::vector<int> & parent, 
					   std::vector< std::pair<int, int> > & bridges)
{ 
    // A static variable is used for simplicity, we can  
    // avoid use of static variable by passing a pointer. 
    static int time = 0; 
  
    // Mark the current node as visited 
    visited[u] = true; 
  
    // Initialize discovery time and low value 
    disc[u] = low[u] = ++time; 
  
    // Go through all vertices aadjacent to this 
    for (int i=0; i<adjacency_list_[u].size(); i++)
	{ 
		int v = adjacency_list_[u][i];  // v is current adjacent of u 
  
        // If v is not visited yet, then recur for it 
        if (!visited[v]) 
        { 
            parent[v] = u; 
            bridgeUtil(v, visited, disc, low, parent, bridges); 
  
            // Check if the subtree rooted with v has a  
            // connection to one of the ancestors of u 
            low[u]  = std::min(low[u], low[v]); 
  
			bridges.push_back(std::make_pair(u, v));

            // If the lowest vertex reachable from subtree  
            // under v is  below u in DFS tree, then u-v  
            // is a bridge 
            if (low[v] > disc[u]) 
              std::cout << "Bridge at: " << u << " " << v << std::endl;
        } 
  
        // Update low value of u for parent function calls. 
        else if (v != parent[u]) 
            low[u]  = std::min(low[u], disc[v]); 
    } 
};

bool Graph::is_element_in_vector(int a, std::vector<int> & A, int & element_position)
{
	bool point_is_in_set = false;

	for (int i = 0; i < A.size(); ++i)
		if ( a == A.at(i) )
		{
			element_position = i;
			point_is_in_set = true;
		}

	return point_is_in_set;
}


#endif
