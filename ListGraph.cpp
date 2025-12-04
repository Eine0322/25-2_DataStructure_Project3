#include "ListGraph.h"
#include <iostream>
#include <utility>

ListGraph::ListGraph(bool type, int size) : Graph(type, size)
{
	// Allocate adjacency list for each vertex
	m_List = new map<int, int>[size];
}

ListGraph::~ListGraph()	
{
	// Deallocate adjacency list
	if (m_List)
		delete[] m_List;
}

void ListGraph::getAdjacentEdges(int vertex, map<int, int>* m)	 
{
	// Get adjacent edges for undirected graph
	// Copy edges from vertex's adjacency list
	*m = m_List[vertex];
	
	// For undirected graph, also add reverse edges
	for (int i = 0; i < m_Size; i++) {
		if (i != vertex && m_List[i].find(vertex) != m_List[i].end()) {
			(*m)[i] = m_List[i][vertex];
		}
	}
}

void ListGraph::getAdjacentEdgesDirect(int vertex, map<int, int>* m)	
{
	// Get adjacent edges for directed graph
	// Simply copy edges from vertex's adjacency list
	*m = m_List[vertex];
}

void ListGraph::insertEdge(int from, int to, int weight) 
{
	// Insert edge from 'from' to 'to' with given weight
	m_List[from][to] = weight;
}

bool ListGraph::printGraph(ofstream *fout)	
{
	// Check if graph exists
	if (m_Size <= 0)
		return false;
	
	// Print adjacency list format
	for (int i = 0; i < m_Size; i++) {
		*fout << "[" << i << "]";
		
		// Sort and print adjacent vertices
		for (auto& edge : m_List[i]) {
			*fout << " -> (" << edge.first << "," << edge.second << ")";
		}
		*fout << endl;
	}
	
	return true;
}