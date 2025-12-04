#include "MatrixGraph.h"
#include <iostream>
#include <vector>
#include <string>

MatrixGraph::MatrixGraph(bool type, int size) : Graph(type, size)
{
	// Allocate 2D matrix for adjacency matrix
	m_Mat = new int*[size];
	for (int i = 0; i < size; i++) {
		m_Mat[i] = new int[size];
		// Initialize all edges to 0 (no edge)
		for (int j = 0; j < size; j++) {
			m_Mat[i][j] = 0;
		}
	}
}

MatrixGraph::~MatrixGraph()
{
	// Deallocate 2D matrix
	if (m_Mat) {
		for (int i = 0; i < m_Size; i++) {
			delete[] m_Mat[i];
		}
		delete[] m_Mat;
	}
}

void MatrixGraph::getAdjacentEdges(int vertex, map<int, int>* m)
{	
	// Get adjacent edges for undirected graph
	m->clear();
	
	// Check outgoing edges from vertex
	for (int i = 0; i < m_Size; i++) {
		if (m_Mat[vertex][i] != 0) {
			(*m)[i] = m_Mat[vertex][i];
		}
	}
	
	// For undirected graph, also check incoming edges
	for (int i = 0; i < m_Size; i++) {
		if (i != vertex && m_Mat[i][vertex] != 0) {
			(*m)[i] = m_Mat[i][vertex];
		}
	}
}

void MatrixGraph::getAdjacentEdgesDirect(int vertex, map<int, int>* m)
{
	// Get adjacent edges for directed graph
	m->clear();
	
	// Only check outgoing edges from vertex
	for (int i = 0; i < m_Size; i++) {
		if (m_Mat[vertex][i] != 0) {
			(*m)[i] = m_Mat[vertex][i];
		}
	}
}

void MatrixGraph::insertEdge(int from, int to, int weight)	
{
	// Insert edge from 'from' to 'to' with given weight
	m_Mat[from][to] = weight;
}

bool MatrixGraph::printGraph(ofstream *fout)	
{
	// Check if graph exists
	if (m_Size <= 0)
		return false;
	
	// Print column headers
	*fout << "  ";
	for (int i = 0; i < m_Size; i++) {
		*fout << "[" << i << "] ";
	}
	*fout << endl;
	
	// Print matrix with row headers
	for (int i = 0; i < m_Size; i++) {
		*fout << "[" << i << "] ";
		for (int j = 0; j < m_Size; j++) {
			*fout << m_Mat[i][j];
			if (j < m_Size - 1)
				*fout << " ";
		}
		*fout << endl;
	}
	
	return true;
}