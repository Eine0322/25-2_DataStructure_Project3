#include "Manager.h"
#include "GraphMethod.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

Manager::Manager()	
{
	graph = nullptr;	
	fout.open("log.txt", ios::out | ios::trunc);  // Truncate mode to clear file
	load = 0;	// Nothing is loaded initially
	if (fout.is_open())
		fout.close();  // Close immediately, reopen in run()
}

Manager::~Manager()
{
	if(load)	// If graph is loaded, delete graph to prevent memory leak
		delete graph;	
	if(fout.is_open())	// If output file is opened, close it
		fout.close();	// Close log.txt file
}

void Manager::run(const char* command_txt){
	ifstream fin;	// Command file input stream
	fin.open(command_txt, ios_base::in);  // Open command file in read mode
		
	if(!fin) {  // If command file cannot be opened, print error
		ofstream fout("log.txt", ios::app);
		fout << "command file open error" << endl;
		fout.close();
		return;	
	}
	
	fout.open("log.txt", ios::app);  // Reopen in append mode
	
	string line;
	// Read and process commands line by line
	while (getline(fin, line)) {
		if (line.empty()) continue;  // Skip empty lines
		
		istringstream iss(line);
		string command;
		iss >> command;
		
		// Process each command
		if (command == "LOAD") {
			string filename;
			if (!(iss >> filename)) {
				printErrorCode(100);
			} else if (!LOAD(filename.c_str())) {
				printErrorCode(100);
			} else {
				fout << "========LOAD========" << endl;
				fout << "Success" << endl;
				fout << "====================" << endl << endl;
			}
		}
		else if (command == "PRINT") {
			if (!PRINT()) {
				printErrorCode(200);
			}
		}
		else if (command == "BFS") {
			char option;
			int vertex;
			string extra;
			if (!(iss >> option >> vertex) || (iss >> extra)) {
				printErrorCode(300);
			} else if (option != 'O' && option != 'X') {
				printErrorCode(300);
			} else if (!mBFS(option, vertex)) {
				printErrorCode(300);
			}
		}
		else if (command == "DFS") {
			char option;
			int vertex;
			string extra;
			if (!(iss >> option >> vertex) || (iss >> extra)) {
				printErrorCode(400);
			} else if (option != 'O' && option != 'X') {
				printErrorCode(400);
			} else if (!mDFS(option, vertex)) {
				printErrorCode(400);
			}
		}
		else if (command == "KRUSKAL") {
			string extra;
			if (iss >> extra) {
				printErrorCode(500);
			} else if (!mKRUSKAL()) {
				printErrorCode(500);
			}
		}
		else if (command == "DIJKSTRA") {
			char option;
			int vertex;
			string extra;
			if (!(iss >> option >> vertex) || (iss >> extra)) {
				printErrorCode(600);
			} else if (option != 'O' && option != 'X') {
				printErrorCode(600);
			} else if (!mDIJKSTRA(option, vertex)) {
				printErrorCode(600);
			}
		}
		else if (command == "BELLMANFORD") {
			char option;
			int s_vertex, e_vertex;
			string extra;
			if (!(iss >> option >> s_vertex >> e_vertex) || (iss >> extra)) {
				printErrorCode(700);
			} else if (option != 'O' && option != 'X') {
				printErrorCode(700);
			} else if (!mBELLMANFORD(option, s_vertex, e_vertex)) {
				printErrorCode(700);
			}
		}
		else if (command == "FLOYD") {
			char option;
			string extra;
			if (!(iss >> option) || (iss >> extra)) {
				printErrorCode(800);
			} else if (option != 'O' && option != 'X') {
				printErrorCode(800);
			} else if (!mFLOYD(option)) {
				printErrorCode(800);
			}
		}
		else if (command == "CENTRALITY") {
			string extra;
			if (iss >> extra) {
				printErrorCode(900);
			} else if (!mCentrality()) {
				printErrorCode(900);
			}
		}
		else if (command == "EXIT") {
			fout << "========EXIT========" << endl;
			fout << "Success" << endl;
			fout << "====================" << endl << endl;
			fout.close();  // Close output file
			fin.close();
			
			// Prevent any further algorithm output
			if (graph) {
				delete graph;
				graph = nullptr;
				load = 0;
			}
			return;  // Exit immediately
		}
	}
	
	fout.close();  // Close at end
	fin.close();
	return;
}

bool Manager::LOAD(const char* filename)
{
	ifstream fin(filename);
	
	// Check if file exists and can be opened
	if (!fin) {
		return false;
	}
	
	// If graph already exists, delete it and create new one
	if (load) {
		delete graph;
		graph = nullptr;
		load = 0;
	}
	
	char type;
	int size;
	
	// Read graph type (L for List, M for Matrix) and number of vertices
	fin >> type >> size;
	
	bool isDirected = true;  // Default graph type
	
	// Create appropriate graph type based on input
	if (type == 'L') {
		graph = new ListGraph(isDirected, size);
		
		// Read adjacency list format
		string line;
		getline(fin, line);  // Skip remaining characters on first line
		
		for (int from = 0; from < size; from++) {
			if (!getline(fin, line)) break;
			
			// Parse vertex number line (just vertex index)
			istringstream vss(line);
			int vertex_num;
			if (!(vss >> vertex_num)) continue;
			
			// Read next line for edges
			if (!getline(fin, line)) break;
			istringstream ess(line);
			
			int to, weight;
			// Read all edges for this vertex
			while (ess >> to >> weight) {
				graph->insertEdge(from, to, weight);
			}
		}
	} 
	else if (type == 'M') {
		graph = new MatrixGraph(isDirected, size);
		
		// Read adjacency matrix format
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				int weight;
				fin >> weight;
				if (weight != 0) {  // Only insert non-zero weights
					graph->insertEdge(i, j, weight);
				}
			}
		}
	} 
	else {
		fin.close();
		return false;
	}
	
	fin.close();
	load = 1;  // Mark graph as loaded
	return true;
}

bool Manager::PRINT()	
{
	// Check if graph is loaded
	if (!load || !graph) {
		return false;
	}
	
	// Print graph structure
	fout << "========PRINT========" << endl;
	graph->printGraph(&fout);
	fout << "====================" << endl << endl;
	
	return true;
}

bool Manager::mBFS(char option, int vertex)	
{
	// Validate graph and vertex
	if (!load || !graph || vertex < 0 || vertex >= graph->getSize()) {
		return false;
	}
	
	// Call BFS algorithm
	return BFS(graph, option, vertex);
}

bool Manager::mDFS(char option, int vertex)	
{
	// Validate graph and vertex
	if (!load || !graph || vertex < 0 || vertex >= graph->getSize()) {
		return false;
	}
	
	// Call DFS algorithm
	return DFS(graph, option, vertex);
}

bool Manager::mDIJKSTRA(char option, int vertex)	
{
	// Validate graph and vertex
	if (!load || !graph || vertex < 0 || vertex >= graph->getSize()) {
		return false;
	}
	
	// Call Dijkstra algorithm
	return Dijkstra(graph, option, vertex);
}

bool Manager::mKRUSKAL()
{
	// Check if graph is loaded
	if (!load || !graph) {
		return false;
	}
	
	// Call Kruskal algorithm
	return Kruskal(graph);
}

bool Manager::mBELLMANFORD(char option, int s_vertex, int e_vertex) 
{
	// Validate graph and vertices
	if (!load || !graph || s_vertex < 0 || s_vertex >= graph->getSize() 
		|| e_vertex < 0 || e_vertex >= graph->getSize()) {
		return false;
	}
	
	// Call Bellman-Ford algorithm
	return Bellmanford(graph, option, s_vertex, e_vertex);
}

bool Manager::mFLOYD(char option)
{
	// Check if graph is loaded
	if (!load || !graph) {
		return false;
	}
	
	// Call Floyd-Warshall algorithm
	return FLOYD(graph, option);
}

bool Manager::mCentrality() {
	// Check if graph is loaded
	if (!load || !graph) {
		return false;
	}
	
	// Call Centrality calculation
	return Centrality(graph);
}

void Manager::printErrorCode(int n)
{
	// Print error code in specified format
	fout << "========ERROR========" << endl;
	fout << n << endl;
	fout << "====================" << endl << endl;
}