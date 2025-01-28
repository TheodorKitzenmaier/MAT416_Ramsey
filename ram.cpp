#include <iostream>
#include <cstring>
#include "graph.cpp"

#define SAFETY_LIMIT 30

// Static log variable.
bool DO_LOG = false;
bool DO_PROGRESS = false;

// Generates the graph with <v> vertices and <e> edges. <e> has size (v-1) + (v-2) + (v-3)... + 1.
// <e> is of form {neighbors of 0} , {neighbors of 1 > 1}, {neighbors of 2 > 2}, ...
graph* generateGraph(int v, bool* e) {
	//int elen = (v * (v - 1)) / 2;
	graph* G = makeGraph(v);
	int i = 0;

	// Populate with edges from e.
	for (int a = 0; a < v; a++) {
		for (int b = a + 1; b < v; b++) {
			if (e[i++])
				addEdge(G, a, b);
		}
	}

	return G;
}

// Uses same logic as generateGraph(), but modifies an existing graph.
void setGraph(graph* G, bool* e) {
	int i = 0;

	// Populate with edges from e.
	for (int a = 0; a < G->V; a++) {
		for (int b = a + 1; b < G->V; b++) {
			if (e[i++])
				addEdge(G, a, b);
			else
				removeEdge(G, a, b);
		}
	}
}

// Returns the base set of the given size.
// Set allocated with new, delete after use.
int* getSet(int size) {
	int* set = new int[size];
	for (int i = 0; i < size; i++) {
		set[i] = i;
	}

	return set;
}

// Steps to the next set, returns if the new set is valid.
bool stepSet(graph* G, int* set, int length) {
	int index = length - 1;

	// Search back for first element to increment, then restore forwards.
	// Invalid if an element is outside of V(G).
	bool run = true;
	while (run) {
		// If the index is at its max value, move to previous index.
		// If the index is not at its max, done.
		if (set[index] >= G->V + index - length)
			index--;
		else
			run = false;

		// Return false, first index has overrun its max value.
		if (index < 0)
			return false;
	}

	// Increment index.
	set[index]++;

	// Set values after index to min values.
	index++;
	while (index < length) {
		set[index] = set[index - 1] + 1;
		index++;
	}

	// If all assumptions hold, success!
	return true;
}

// Returns omega(G) or alpha(G), depending on function passed.
int largestX(graph* G, bool (*valid)(graph*, int*, int)) {
	// Upper bound of |G|.
	// Lower bound of 1.
	int max = 1;

	// Search all combinations to see if there is a clique/coclique of the given size. If there is, skip others and determine if there is one of a larger size.
	bool larger;
	do {
		larger = false;

		// Initial vertex set. Vertices are in ascending order.
		int* set = getSet(max + 1);

		// Test all combinations of vertices.
		bool next;
		do {
			next = true;

			// Test if combination is a clique or coclique. If it is, we are done.
			if (valid(G, set, max + 1)) {
				next = false;
				larger = true;
			}

			// Advance set.
			if (!stepSet(G, set, max + 1))
				next = false;
		} while (next);

		// Cleaning up my mess.
		delete set;

		if (larger)
			max++;
	} while (larger && max < G->V);

	return max;
}

// Returns true if all graphs of size n satisfy omega(G) >= a or alpha(G) >= b.
bool testn(int a, int b, int n) {
	if (DO_LOG) std::clog << "Testing graphs of size " << n << std::endl;
	int len = n * (n - 1) / 2;
	bool* state = new bool[len];

	for (int i = 0; i < len; i++) {
		state[i] = false;
	}

	// Iterate over all graphs of size n.
	bool done = false;
	bool failure = false;
	graph* G = generateGraph(n, state);
	do {
		// Log state.
		if (DO_LOG) {
			std::clog << "State: ";
			for (int x = 0; x < len; x++) {
				std::clog << (state[x] ? "*" : "_");
			}
			std::clog << std::endl;
		}

		// Check if graph conforms, if not, fail.
		if (!(largestX(G, &isClique) >= a) && !(largestX(G, &isCoclique) >= b)) {
			failure = true;
			break;
		}

		// Attempt to step graph state.
		int i = 0;
		do {
			done = true;

			if (state[i]) {
				state[i] = false;
				i++;
			}
			else {
				state[i] = true;
				done = false;
				i = len;
			}
		} while (i < len);
		setGraph(G, state);
	} while (!done);

	// No leaky memory (hopefully).
	delete state;
	delete G;

	return !failure;
}

// Returns the smallest number n such that all graphs of size n have omega(G) >= a or alpha(G) >= b.
// All of the time complexity.
int ram(int a, int b) {
	// Increment graph size until all graphs satisfy requirements. Stop at safety limit.
	int n = 1;
	while (n <= SAFETY_LIMIT && !testn(a, b, n)) {
		n++;
		if (DO_LOG || DO_PROGRESS) std::clog << "Increased graph size to " << n << "..." << std::endl;
		int count = 1;
		for (int i = 0; i < (n * (n - 1) / 2); i++) {
			count *= 2;
		}
		if (DO_LOG || DO_PROGRESS) std::clog << "Up to " << count << " graphs to check." << std::endl << std::endl;
	}

	return n;
}

int main (int argc, char** argv) {
	for (int i = 0; i < argc; i++) {
		if (!std::strcmp(argv[i], "-log"))
			DO_LOG = true;
		if (!std::strcmp(argv[i], "-pro"))
			DO_PROGRESS = true;
	}

	// Prompt user for a and b values.
	int a, b;
	std::cout << "Ram calculator." << std::endl;
	std::cout << "Enter value for a (clique minimum):";
	std::cin >> a;

	std::cout << "Enter value for b (coclique minimum):";
	std::cin >> b;

	// Calculate Ram(a, b).
	int r = ram(a, b);

	// Print results, or an error.
	if (r == SAFETY_LIMIT)
		std::cout << "Potential error. Ram(a, b) was equal to the safety limit." << std::endl;
	else
		std::cout << "Ram(" << a << ", " << b << ") = " << r << ".\nThank you for using Ram calculator. Goodbye." << std::endl;

	return 0;
}
