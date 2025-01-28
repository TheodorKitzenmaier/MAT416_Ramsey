// Simple Graph.
struct graph {
	int V;
	bool* E;

	~graph() {
		delete E;
	}
};

// Creates an empty graph with <v> verticies.
graph* makeGraph(int v) {
	graph* G = new graph();

	G->V = v;
	G->E = new bool[v * v];

	for (int i = 0; i < v * v; i++) {
		G->E[i] = false;
	}

	return G;
}

// Returns the index of the edge from u to v in a graph of size V.
int indexEdge(int V, int u, int v) {
	return V * u + v;
}

// Adds an edge between verticies u and v.
// Returns if the edge already existed.
bool addEdge(graph* G, int u, int v) {
	// Store initial state of edge for return.
	bool f = G->E[indexEdge(G->V, u, v)];

	G->E[indexEdge(G->V, u, v)] = true;
	G->E[indexEdge(G->V, v, u)] = true;

	return f;
}

// Removes an edge between vertices u and v.
// Returns if the edge existed.
bool removeEdge(graph* G, int u, int v) {
	// Store initial state of edge for return.
	bool f = G->E[indexEdge(G->V, u, v)];

	G->E[indexEdge(G->V, u, v)] = false;
	G->E[indexEdge(G->V, v, u)] = false;

	return f;
}

// Returns if there is an edge between u and v in graph G.
bool hasEdge(graph* G, int u, int v) {
	return G->E[indexEdge(G->V, u, v)];
}

// Determines if the vertices in set X form a clique in graph G. X is of size x.
bool isClique(graph* G, int* X, int x) {
	// Loop over all edges. If an edge is not present, the set is not a clique.
	for (int u = 0; u < x; u++) {
		for (int v = u + 1; v < x; v++) {
			if (!hasEdge(G, X[u], X[v]))
				return false;
		}
	}
	return true;
}

// Determines if the vertices in set X form a coclique in graph G. X is of size x.
bool isCoclique(graph* G, int* X, int x) {
	// Loop over all edges. If an edge is present, it is not a coclique.
	for (int u = 0; u < x; u++) {
		for (int v = u + 1; v < x; v++) {
			if (hasEdge(G, X[u], X[v]))
				return false;
		}
	}
	return true;
}
