// Main program logic
#include <iostream>
#include "Graph.h"
#include "Utility.h"
using namespace std;

int main() {
    // get user input for specific state for crop rotation
    string state = "Alabama";

    //loading crop data based on the specified state
    vector<Crop> crops = readStateCropData("../CropOptimaProject/dataset/cropNutrientDataset.csv", state);

    //vector<Crop> newCrops;

//    for(int i = 0; i < 300; i++) {
//        newCrops.push_back(crops[i]);
//    }

    // create the graph based on how many crops are in the specified state
    Graph cropGraph(crops);

    // populate the graph with the proper compatibility values (edge weights)
    cropGraph.populate();

    // print matrix for debugging
    cropGraph.printMatrix();

    //get user input for the starting and ending crop for the rotation
    string startCrop = "Apricot";
    string endCrop = "Almond";

    //running the algorithms
    cropGraph.bellmanFord(startCrop, endCrop);
    cout << "" << endl;
    cout << "" << endl;
    cout << "" << endl;
    cropGraph.floydWarshall(startCrop, endCrop);

    return 0;

}