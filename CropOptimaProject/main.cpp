// Main program logic
#include <iostream>
#include <chrono>
#include "Graph.h"
#include "Utility.h"
using namespace std;

int main() {
    cout << "\n=====================================================================================" << endl;
    cout << "                                  WELCOME TO                                     " << endl;
    cout << "\n"
            " ██████╗██████╗  ██████╗ ██████╗      ██████╗ ██████╗ ████████╗██╗███╗   ███╗ █████╗ \n"
            "██╔════╝██╔══██╗██╔═══██╗██╔══██╗    ██╔═══██╗██╔══██╗╚══██╔══╝██║████╗ ████║██╔══██╗\n"
            "██║     ██████╔╝██║   ██║██████╔╝    ██║   ██║██████╔╝   ██║   ██║██╔████╔██║███████║\n"
            "██║     ██╔══██╗██║   ██║██╔═══╝     ██║   ██║██╔═══╝    ██║   ██║██║╚██╔╝██║██╔══██║\n"
            "╚██████╗██║  ██║╚██████╔╝██║         ╚██████╔╝██║        ██║   ██║██║ ╚═╝ ██║██║  ██║\n"
            " ╚═════╝╚═╝  ╚═╝ ╚═════╝ ╚═╝          ╚═════╝ ╚═╝        ╚═╝   ╚═╝╚═╝     ╚═╝╚═╝  ╚═╝\n"
            << endl;
    cout << "=====================================================================================" << endl;
    cout << "\n";
    cout << "Welcome to Crop Optima, where the Dirt Detectives are on the case..."
            "\nLet's 'turnip' the nutrient efficiency of your crop sequence and crack the code to healthier soil!\n" << endl;
    cout << "=====================================================================================" << endl;

    // get user input for specific state for crop rotation
    string state;
    cout << "\nDon't be 'cornfused'! Which state are you planting in? ";
    getline(cin, state);

    //loading crop data based on the specified state
    vector<Crop> crops = readStateCropData("../CropOptimaProject/dataset/cropNutrientDataset.csv", state);

    while(crops.empty()) {
        string newState;
        cout << "Oops! Looks like we can't find " << state << " in the United States." << endl;
        cout << "Try a different state: ";
        getline(cin, newState);
        crops = readStateCropData("../CropOptimaProject/dataset/cropNutrientDataset.csv", newState);
    }

    // create the graph based on how many crops are in the specified state
    Graph cropGraph(crops);

    // populate the graph with the proper compatibility values (edge weights)
    cropGraph.populate();

    //get user input for the starting and ending crop for the rotation
    string startCrop, endCrop;

    // TODO: check for valid crop name input, if it exists in state

    cout << "Let's get this rotation 'growing'! What's your starting crop? ";
    getline(cin, startCrop);

    // search function for valid crops that exist within the graph/state

    cout << "And for the grand finale, what crop would you like to 'leaf' off with? ";
    getline(cin, endCrop);
    cout << "\n";
    cout << "=====================================================================================" << endl;
    cout << "\n";

    //running the algorithms
    // Using Chrono in C++ reference from Geeks for Geeks https://www.geeksforgeeks.org/chrono-in-c/
    // ********************************************************************************************************//
    cout << "Bellman Ford Crop Sequence from " << startCrop << " to " << endCrop << ": " << endl;
    auto bellmanStart = chrono::high_resolution_clock::now();
    cropGraph.bellmanFord(startCrop, endCrop);
    auto bellmanEnd = chrono::high_resolution_clock::now();

    std::chrono::duration<double> bellmanDuration = bellmanEnd - bellmanStart; // calculate and print duration for BF algo
    cout << "Execution Time: " << bellmanDuration.count() << " seconds" << endl;
    cout << "\n";

    // ********************************************************************************************************//
    cout << "=====================================================================================" << endl;
    cout << "\nFloyd Warshall Crop Sequence from "<< startCrop << " to " << endCrop << ": " << endl;
    auto floydStart = chrono::high_resolution_clock::now();
    cropGraph.floydWarshall(startCrop, endCrop);
    auto floydEnd = chrono::high_resolution_clock::now();

    chrono::duration<double> floydDuration = floydEnd - floydStart; // calculate and print duration for FW algo
    cout << "Execution Time: " << floydDuration.count() << " seconds" << endl;

    cout << "\n";
    cout << "=====================================================================================" << endl;
    cout << "\nThanks for using Crop Optima! May your harvests be plentiful and your weeds be few!" << endl;
    cout << R"(
                  _-_                                             _-_
               /~~   ~~\                                       /~~   ~~\
            /~~         ~~\           _.-^-._    .--.       /~~         ~~\
           {               }       .-'   _   '-. |__|      {               }
            \  _-     -_  /       /     |_|     \|  |       \  _-     -_  /
                 \\ //           /               \  |            \\ //
                  | |           /|     _____     |\ |             | |
                  | |            |    |==|==|    |  |             | |
                 // \\           |    |==|==| |  |  |            // \\
~~~^~^~~^~^~~~^~^~~^~^~^~^~^~^~^~~~^~^~~~^~^~~^~^~^~^~^~^~^~^~^~^~~~^~^~~^~^~^~^~^~^~~
)" << endl;

    return 0;
}