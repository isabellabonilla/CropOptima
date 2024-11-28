// Main program logic
#include <iostream>
#include "Graph.h"

using namespace std;

int main() {
// get user input for specific state for crop rotation

// loading crop data based on the specified state
vector<Crop> crops = readStateCropData(state);

// create the graph based on how many crops are in the specified state
Graph cropGraph(crops);

// populate the graph with the proper compatibility values (edge weights)
cropGraph.populate();

// print matrix for debugging
cropGraph.printMatrix();

// get user input for the starting and ending crop for the rotation

// running the algorithms


}
