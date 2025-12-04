#include <iostream>
#include <vector>
#include <fstream>
#include "GraphMethod.h"
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <list>
#include <utility>
#include <algorithm>
#include <climits>

using namespace std;

// Union-Find data structure for Kruskal's algorithm
class UnionFind {
private:
	vector<int> parent;
	vector<int> rank;
	
public:
	UnionFind(int n) {
		parent.resize(n);
		rank.resize(n, 0);
		for (int i = 0; i < n; i++) {
			parent[i] = i;
		}
	}
	
	// Find with path compression
	int find(int x) {
		if (parent[x] != x) {
			parent[x] = find(parent[x]);
		}
		return parent[x];
	}
	
	// Union by rank
	bool unite(int x, int y) {
		int px = find(x);
		int py = find(y);
		
		if (px == py) return false;
		
		if (rank[px] < rank[py]) {
			parent[px] = py;
		} else if (rank[px] > rank[py]) {
			parent[py] = px;
		} else {
			parent[py] = px;
			rank[px]++;
		}
		return true;
	}
};

bool BFS(Graph* graph, char option, int vertex)
{
	ofstream fout("log.txt", ios::app);
	
	int size = graph->getSize();
	vector<bool> visited(size, false);
	queue<int> q;
	vector<int> result;
	
	// Start BFS from given vertex
	q.push(vertex);
	visited[vertex] = true;
	
	while (!q.empty()) {
		int curr = q.front();
		q.pop();
		result.push_back(curr);
		
		// Get adjacent edges based on direction option
		map<int, int> edges;
		if (option == 'O') {
			graph->getAdjacentEdgesDirect(curr, &edges);
		} else {
			graph->getAdjacentEdges(curr, &edges);
		}
		
		// Visit adjacent vertices in sorted order (lowest number first)
		for (auto& edge : edges) {
			int next = edge.first;
			if (!visited[next]) {
				visited[next] = true;
				q.push(next);
			}
		}
	}
	
	// Print result
	fout << "========BFS========" << endl;
	if (option == 'O') {
		fout << "Directed Graph BFS" << endl;
	} else {
		fout << "Undirected Graph BFS" << endl;
	}
	fout << "Start: " << vertex << endl;
	
	for (size_t i = 0; i < result.size(); i++) {
		fout << result[i];
		if (i < result.size() - 1) fout << " -> ";
	}
	fout << endl;
	fout << "====================" << endl << endl;
	
	fout.close();
	return true;
}

bool DFS(Graph* graph, char option, int vertex)
{
	ofstream fout("log.txt", ios::app);
	
	int size = graph->getSize();
	vector<bool> visited(size, false);
	stack<int> s;
	vector<int> result;
	
	// Start DFS from given vertex
	s.push(vertex);
	
	while (!s.empty()) {
		int curr = s.top();
		s.pop();
		
		if (visited[curr]) continue;
		
		visited[curr] = true;
		result.push_back(curr);
		
		// Get adjacent edges based on direction option
		map<int, int> edges;
		if (option == 'O') {
			graph->getAdjacentEdgesDirect(curr, &edges);
		} else {
			graph->getAdjacentEdges(curr, &edges);
		}
		
		// Push adjacent vertices in reverse order for correct DFS order
		vector<int> neighbors;
		for (auto& edge : edges) {
			neighbors.push_back(edge.first);
		}
		sort(neighbors.rbegin(), neighbors.rend());
		
		for (int next : neighbors) {
			if (!visited[next]) {
				s.push(next);
			}
		}
	}
	
	// Print result
	fout << "========DFS========" << endl;
	if (option == 'O') {
		fout << "Directed Graph DFS" << endl;
	} else {
		fout << "Undirected Graph DFS" << endl;
	}
	fout << "Start: " << vertex << endl;
	
	for (size_t i = 0; i < result.size(); i++) {
		fout << result[i];
		if (i < result.size() - 1) fout << " -> ";
	}
	fout << endl;
	fout << "====================" << endl << endl;
	
	fout.close();
	return true;
}

bool Kruskal(Graph* graph)
{
	ofstream fout("log.txt", ios::app);
	
	int size = graph->getSize();
	vector<pair<int, pair<int, int>>> edges;  // {weight, {from, to}}
	
	// Collect all edges (treating as undirected)
	set<pair<int, int>> added;
	for (int i = 0; i < size; i++) {
		map<int, int> adjEdges;
		graph->getAdjacentEdgesDirect(i, &adjEdges);
		
		for (auto& edge : adjEdges) {
			int to = edge.first;
			int weight = edge.second;
			int a = min(i, to);
			int b = max(i, to);
			
			if (added.find({a, b}) == added.end()) {
				edges.push_back({weight, {i, to}});
				added.insert({a, b});
			}
		}
	}
	
	// Sort edges by weight
	sort(edges.begin(), edges.end());
	
	// Apply Kruskal's algorithm using Union-Find
	UnionFind uf(size);
	vector<map<int, int>> mst(size);
	int totalCost = 0;
	int edgeCount = 0;
	
	for (auto& e : edges) {
		int weight = e.first;
		int from = e.second.first;
		int to = e.second.second;
		
		if (uf.unite(from, to)) {
			mst[from][to] = weight;
			mst[to][from] = weight;
			totalCost += weight;
			edgeCount++;
		}
	}
	
	// Check if MST is valid (all vertices connected)
	if (edgeCount != size - 1) {
		fout.close();
		return false;
	}
	
	// Print MST in adjacency list format
	fout << "========KRUSKAL========" << endl;
	for (int i = 0; i < size; i++) {
		fout << "[" << i << "]";
		for (auto& edge : mst[i]) {
			fout << " " << edge.first << "(" << edge.second << ")";
		}
		fout << endl;
	}
	fout << "Cost: " << totalCost << endl;
	fout << "====================" << endl << endl;
	
	fout.close();
	return true;
}

bool Dijkstra(Graph* graph, char option, int vertex)
{
	ofstream fout("log.txt", ios::app);
	
	int size = graph->getSize();
	
	// Check for negative weights
	for (int i = 0; i < size; i++) {
		map<int, int> edges;
		graph->getAdjacentEdgesDirect(i, &edges);
		for (auto& edge : edges) {
			if (edge.second < 0) {
				fout.close();
				return false;
			}
		}
	}
	
	// Initialize distances and previous vertices
	vector<int> dist(size, INT_MAX);
	vector<int> prev(size, -1);
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
	
	dist[vertex] = 0;
	pq.push({0, vertex});
	
	// Dijkstra's algorithm
	while (!pq.empty()) {
		int d = pq.top().first;
		int curr = pq.top().second;
		pq.pop();
		
		if (d > dist[curr]) continue;
		
		// Get adjacent edges
		map<int, int> edges;
		if (option == 'O') {
			graph->getAdjacentEdgesDirect(curr, &edges);
		} else {
			graph->getAdjacentEdges(curr, &edges);
		}
		
		// Relax edges
		for (auto& edge : edges) {
			int next = edge.first;
			int weight = edge.second;
			
			if (dist[curr] != INT_MAX && dist[curr] + weight < dist[next]) {
				dist[next] = dist[curr] + weight;
				prev[next] = curr;
				pq.push({dist[next], next});
			}
		}
	}
	
	// Print results
	fout << "========DIJKSTRA========" << endl;
	if (option == 'O') {
		fout << "Directed Graph Dijkstra" << endl;
	} else {
		fout << "Undirected Graph Dijkstra" << endl;
	}
	fout << "Start: " << vertex << endl;
	
	for (int i = 0; i < size; i++) {
		fout << "[" << i << "] ";
		
		if (dist[i] == INT_MAX) {
			fout << "x" << endl;
		} else {
			// Reconstruct path
			vector<int> path;
			int curr = i;
			while (curr != -1) {
				path.push_back(curr);
				curr = prev[curr];
			}
			reverse(path.begin(), path.end());
			
			for (size_t j = 0; j < path.size(); j++) {
				fout << path[j];
				if (j < path.size() - 1) fout << " -> ";
			}
			fout << " (" << dist[i] << ")" << endl;
		}
	}
	fout << "====================" << endl << endl;
	
	fout.close();
	return true;
}

bool Bellmanford(Graph* graph, char option, int s_vertex, int e_vertex) 
{
	ofstream fout("log.txt", ios::app);
	
	int size = graph->getSize();
	vector<int> dist(size, INT_MAX);
	vector<int> prev(size, -1);
	
	dist[s_vertex] = 0;
	
	// Collect all edges
	vector<tuple<int, int, int>> edges;  // {from, to, weight}
	for (int i = 0; i < size; i++) {
		map<int, int> adjEdges;
		if (option == 'O') {
			graph->getAdjacentEdgesDirect(i, &adjEdges);
		} else {
			graph->getAdjacentEdges(i, &adjEdges);
		}
		
		for (auto& edge : adjEdges) {
			edges.push_back({i, edge.first, edge.second});
		}
	}
	
	// Relax edges |V| - 1 times
	for (int i = 0; i < size - 1; i++) {
		for (auto& e : edges) {
			int from = get<0>(e);
			int to = get<1>(e);
			int weight = get<2>(e);
			
			if (dist[from] != INT_MAX && dist[from] + weight < dist[to]) {
				dist[to] = dist[from] + weight;
				prev[to] = from;
			}
		}
	}
	
	// Check for negative cycles
	for (auto& e : edges) {
		int from = get<0>(e);
		int to = get<1>(e);
		int weight = get<2>(e);
		
		if (dist[from] != INT_MAX && dist[from] + weight < dist[to]) {
			fout.close();
			return false;  // Negative cycle detected
		}
	}
	
	// Print result
	fout << "========BELLMANFORD========" << endl;
	if (option == 'O') {
		fout << "Directed Graph Bellman-Ford" << endl;
	} else {
		fout << "Undirected Graph Bellman-Ford" << endl;
	}
	
	if (dist[e_vertex] == INT_MAX) {
		fout << "x" << endl;
	} else {
		// Reconstruct path
		vector<int> path;
		int curr = e_vertex;
		while (curr != -1) {
			path.push_back(curr);
			curr = prev[curr];
		}
		reverse(path.begin(), path.end());
		
		for (size_t i = 0; i < path.size(); i++) {
			fout << path[i];
			if (i < path.size() - 1) fout << " -> ";
		}
		fout << endl;
		fout << "Cost: " << dist[e_vertex] << endl;
	}
	fout << "====================" << endl << endl;
	
	fout.close();
	return true;
}

bool FLOYD(Graph* graph, char option)
{
	ofstream fout("log.txt", ios::app);
	
	int size = graph->getSize();
	
	// Initialize distance matrix
	vector<vector<int>> dist(size, vector<int>(size, INT_MAX));
	
	// Set diagonal to 0
	for (int i = 0; i < size; i++) {
		dist[i][i] = 0;
	}
	
	// Fill in edge weights
	for (int i = 0; i < size; i++) {
		map<int, int> edges;
		if (option == 'O') {
			graph->getAdjacentEdgesDirect(i, &edges);
		} else {
			graph->getAdjacentEdges(i, &edges);
		}
		
		for (auto& edge : edges) {
			dist[i][edge.first] = edge.second;
		}
	}
	
	// Floyd-Warshall algorithm
	for (int k = 0; k < size; k++) {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX) {
					if (dist[i][k] + dist[k][j] < dist[i][j]) {
						dist[i][j] = dist[i][k] + dist[k][j];
					}
				}
			}
		}
	}
	
	// Check for negative cycles
	for (int i = 0; i < size; i++) {
		if (dist[i][i] < 0) {
			fout.close();
			return false;
		}
	}
	
	// Print result
	fout << "========FLOYD========" << endl;
	if (option == 'O') {
		fout << "Directed Graph Floyd" << endl;
	} else {
		fout << "Undirected Graph Floyd" << endl;
	}
	
	// Print column headers
	fout << "  ";
	for (int i = 0; i < size; i++) {
		fout << "[" << i << "] ";
	}
	fout << endl;
	
	// Print matrix
	for (int i = 0; i < size; i++) {
		fout << "[" << i << "] ";
		for (int j = 0; j < size; j++) {
			if (dist[i][j] == INT_MAX) {
				fout << "x";
			} else {
				fout << dist[i][j];
			}
			if (j < size - 1) fout << " ";
		}
		fout << endl;
	}
	fout << "====================" << endl << endl;
	
	fout.close();
	return true;
}

bool Centrality(Graph* graph) {
	ofstream fout("log.txt", ios::app);
	
	int size = graph->getSize();
	
	// Use Floyd-Warshall to get all-pairs shortest paths (undirected)
	vector<vector<int>> dist(size, vector<int>(size, INT_MAX));
	
	// Set diagonal to 0
	for (int i = 0; i < size; i++) {
		dist[i][i] = 0;
	}
	
	// Fill in edge weights (undirected)
	for (int i = 0; i < size; i++) {
		map<int, int> edges;
		graph->getAdjacentEdges(i, &edges);
		
		for (auto& edge : edges) {
			dist[i][edge.first] = edge.second;
		}
	}
	
	// Floyd-Warshall algorithm
	for (int k = 0; k < size; k++) {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX) {
					if (dist[i][k] + dist[k][j] < dist[i][j]) {
						dist[i][j] = dist[i][k] + dist[k][j];
					}
				}
			}
		}
	}
	
	// Check for negative cycles
	for (int i = 0; i < size; i++) {
		if (dist[i][i] < 0) {
			fout.close();
			return false;
		}
	}
	
	// Calculate closeness centrality for each vertex
	vector<pair<double, int>> centrality(size);
	
	for (int i = 0; i < size; i++) {
		int sum = 0;
		bool unreachable = false;
		
		// Sum distances from all other vertices to vertex i
		for (int j = 0; j < size; j++) {
			if (i != j) {
				if (dist[j][i] == INT_MAX) {
					unreachable = true;
					break;
				}
				sum += dist[j][i];
			}
		}
		
		if (unreachable || sum == 0) {
			centrality[i] = {-1, i};  // Mark as unreachable
		} else {
			centrality[i] = {(double)(size - 1) / sum, i};
		}
	}
	
	// Find maximum centrality
	double maxCentrality = -1;
	for (int i = 0; i < size; i++) {
		if (centrality[i].first > maxCentrality) {
			maxCentrality = centrality[i].first;
		}
	}
	
	// Print results
	fout << "========CENTRALITY========" << endl;
	
	for (int i = 0; i < size; i++) {
		fout << "[" << i << "] ";
		
		if (centrality[i].first < 0) {
			fout << "x" << endl;
		} else {
			int sum = 0;
			for (int j = 0; j < size; j++) {
				if (i != j && dist[j][i] != INT_MAX) {
					sum += dist[j][i];
				}
			}
			fout << (size - 1) << "/" << sum;
			
			if (centrality[i].first == maxCentrality) {
				fout << " <- Most Central";
			}
			fout << endl;
		}
	}
	fout << "====================" << endl << endl;
	
	fout.close();
	return true;
}