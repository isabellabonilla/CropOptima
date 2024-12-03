#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include <iomanip>
#include <unordered_set>
using namespace std;


struct Crop {
    string name;
    int inputN, outputN;
    int inputP, outputP;
    int inputK, outputK;

    Crop(string name, int inputN, int outputN, int inputP, int outputP, int inputK, int outputK) {
        this->name = name;
        this->inputN = inputN;
        this->outputN = outputN;
        this->inputP = inputP;
        this->outputP = outputP;
        this->inputK = inputK;
        this->outputK = outputK;
    }

};

class Graph { // loose references: graph terminology and implementation, lecture video 6
private:

    int numCrops; // number of vertices in graph

    vector<Crop> crops; // holds all of the crop objects
    vector<vector<tuple<int, int, int>>> adjMatrix; // adjacency matrix that holds the nutrient difference values between crops

    tuple<int, int, int> getNutrientDifference(Crop &crop1, Crop &crop2) { // calculates differences in crop2's nutrient requirements and crop1's nutrient output
        tuple<int, int, int> nutrientDiff;
        int diffN = crop1.outputN - crop2.inputN; // difference in nitrogen
        int diffP = crop1.outputP - crop2.inputP; // difference in phosphorus
        int diffK = crop1.outputK - crop2.inputK; // difference in potassium

        nutrientDiff = make_tuple(diffN, diffP, diffK);
        return nutrientDiff;
    }

public:

    Graph(const vector<Crop> &crops) { // graph constructor
        this->crops = crops;
        this->numCrops = crops.size();

        // initialize adjacency matrix with the correct size and nutrient difference values (edge weights) to -1
        adjMatrix = vector<vector<tuple<int, int, int>>>(numCrops, vector<tuple<int, int, int>>(numCrops, make_tuple(INT_MAX, INT_MAX, INT_MAX)));
    }

    void populate() { // iterate through the entire matrix generating the nutrient differences between each crop (edge weights)
        for (int i = 0; i < numCrops; i++) {
            for (int j = 0; j < numCrops; j++) {
                if (i != j) { // do not add a nutrient difference for a crop to itself (no self loops)
                    tuple<int, int, int> weight = getNutrientDifference(crops[i], crops[j]);
                    adjMatrix[i][j] = weight; // assign the nutrient difference to the edge from crop i to crop j
                }
            }
        }
    }

    void printMatrix() { // for debugging purposes
        cout << "Adjacency Matrix: Nutrient Differences (N, P, K)" << endl;
        cout << "                     ";

        for(auto& crop : crops) {
            cout << setw(18) << right << crop.name;
        }
        cout << endl;

        for (int i = 0; i < numCrops; i++) {
            cout << setw(20) << left << crops[i].name; // print crop names as rows of the matrix

            for (int j = 0; j < numCrops; j++) { // print the nutrient difference between crop row to crop col
                tuple <int, int, int> weight = adjMatrix[i][j];
                if (get<0>(weight) == INT_MAX && get<1>(weight) == INT_MAX && get<2>(weight) == INT_MAX) {
                    cout << setw(20) << right << "(INF, INF, INF)";
                }
                else {
                    cout << setw(5) << right << "(" << get<0>(weight) << ", " << get<1>(weight) << ", " << get<2>(weight) << ") ";
                }
            }
            cout << endl;
        }
        cout << endl;
    }

    // TODO: minimum rotation constraint needs to be larger than 3 crops
    vector<string> bellmanFord(string& startCrop, string& endCrop) { // loose reference: discussion 11 - graph algorithms - slide 29 to 41
        // step 1: initialize map for distances and predecessors
        unordered_map<string, tuple<int, int, int>> dist;
        unordered_map<string, string> predecessor;

        for(auto& crop: crops) { // initialize the dist and predecessor chart
            dist[crop.name] = make_tuple(INT_MAX, INT_MAX, INT_MAX);
            predecessor[crop.name] = "";
        }

        dist[startCrop] = make_tuple(0, 0, 0); // set starting crop's distance from source to 0 (starting crop is the source)

        // step 2: relax edges repeatedly
        for(int i = 0; i < numCrops - 1; i++) {
            for(int x = 0; x < numCrops; x++) {
                for(int y = 0; y < numCrops; y++) { // iterating through the matrix is equivalent to iterating through the edges
                    tuple<int, int, int> weight = adjMatrix[x][y];
                    if(get<0>(weight) != INT_MAX && get<1>(weight) != INT_MAX && get<2>(weight) != INT_MAX) { // self loops are not valid edges (denoted by INFINITY)
                        string fromCrop = crops[x].name;
                        string toCrop = crops[y].name;

                        // accessing the distance tuple values of the fromCrop from the source and adding the nutrient
                        // differences (edge weight) between the fromCrop and toCrop
                        tuple<int, int, int> distFrom = dist[fromCrop];
                        int fromTup1 = abs(get<0>(distFrom) + get<0>(weight));
                        int fromTup2 = abs(get<1>(distFrom) + get<1>(weight));
                        int fromTup3 = abs(get<2>(distFrom) + get<2>(weight));

                        tuple<int, int, int> distTo = dist[toCrop];

                        // decided to add the absolute values of the tuple to get cumulative distance in terms of nutrient values left
                        // update the distance and predecessor of the visited crop if the distance from the toCrop to the source is smaller from the fromCrop
                        if(fromTup1 + fromTup2 + fromTup3 <  abs(get<0>(distTo) + get<1>(distTo) + get<2>(distTo)) && (get<0>(distFrom) != INT_MAX && get<1>(distFrom) != INT_MAX && get<2>(distFrom) != INT_MAX)) {
                            int newTup1 = get<0>(distFrom) + get<0>(weight); // take out the absolute value to maintain proper nutrient difference for updated toCrop distance value
                            int newTup2 = get<1>(distFrom) + get<1>(weight);
                            int newTup3 = get<2>(distFrom) + get<2>(weight);
                            dist[toCrop] = make_tuple(newTup1, newTup2, newTup3);
                            predecessor[toCrop] = fromCrop;
                        }
                    }
                }
            }
        }

        // step 3: check for negative weights (deficits in nutrient differences)
        for (int x = 0; x < numCrops; x++) {
            for (int y = 0; y < numCrops; y++) {
                tuple<int, int, int> weight = adjMatrix[x][y];
                if (get<0>(weight) != INT_MAX && get<1>(weight) != INT_MAX && get<2>(weight) != INT_MAX) {
                    string fromCrop = crops[x].name;
                    string toCrop = crops[y].name;

                    tuple<int, int, int> distFrom = dist[fromCrop];
                    tuple<int, int, int> distTo = dist[toCrop];

                    // check if we can still relax an edge after V-1 iterations
                    if (get<0>(distFrom) + get<0>(weight) < get<0>(distTo) ||
                        get<1>(distFrom) + get<1>(weight) < get<1>(distTo) ||
                        get<2>(distFrom) + get<2>(weight) < get<2>(distTo)) {
                        cout << "Negative weight cycle detected. " << endl;
                        return {}; // return empty sequence to
                    }
                }
            }
        }

        // step 4: reconstruct path from start crop to end crop
        vector<string> cropSequence;
        string currCrop = endCrop;

        // stop backtracking once you reach a crop with no predecessor (could mean the start crop is reached or there is no path)
        while(currCrop != "" && currCrop != startCrop) {
            cropSequence.push_back(currCrop); // add the current crop to the sequence
            currCrop = predecessor[currCrop]; // current crop is now the predecessor
        }

        // if the path is complete and reaches the start crop
        if(currCrop == startCrop) {
            cropSequence.push_back(startCrop); // add start crop to the sequence
            reverse(cropSequence.begin(), cropSequence.end()); // reference for reversing a vector: https://www.geeksforgeeks.org/how-to-reverse-a-vector-using-stl-in-c/
        }
        else { // if there is no path between the end and start crop
            cropSequence.erase(cropSequence.begin(), cropSequence.end());
            cout << "No path exists." << endl;
            cropSequence = {};
        }

        cout << "The following crop sequence minimizes overall nutrient waste from " << startCrop << " to " << endCrop << ": " << endl;
        for(int i = 0; i < cropSequence.size(); i++) { // printing out the sequence (currently for debugging purposes)
            if(i == cropSequence.size() -1) {
                cout << cropSequence[i] << endl;
            }
            else {
                cout << cropSequence[i] << " --> ";
            }
        }
        cout << endl;

        tuple<int, int, int> leftovers = dist[endCrop]; // the leftover nutrients are the distance value of the end crop to the start crop
        cout << "Nutrients Leftover: " << get<0>(leftovers) << " Nitrogen, " << get<1>(leftovers) << " Phosphorus, " << get<2>(leftovers) << " Potassium" << endl;

        return cropSequence;
    }

    // Floyd Warshall Algorithm
    // Pseudocode and Basic Implementation Guidance from:
    // Geeks for Geeks: Floyd-Warshall Algorithm
    // Michael Sambol (YT Video) "Floyd–Warshall algorithm in 4 minutes" https://www.youtube.com/watch?v=4OQeCuLYj-4
    // modified to support start & end crop
    // TODO: add a min length constraint
    vector<string> floydWarshall(string& startCrop, string& endCrop){
        // initialize the distance matrix with the adj matrix
        vector<vector<tuple<int, int, int>>> dist = adjMatrix;
        // use for path reconstruction, keeps track of the 'intermediate vertices' on the shortest path between each pair of vertices.
        vector<vector<int>> next(numCrops, vector<int>(numCrops, -1));
        vector<vector<bool>> visited(numCrops, vector<bool>(numCrops, false));

        for (int i = 0; i < numCrops; i++) {  // next^^ initialized with the same dimensions as the distance matrix (n x n where n = number of vertices)
            for (int j = 0; j < numCrops; j++) {
                if (i != j && get<0>(dist[i][j]) != INT_MAX) {
                    next[i][j] = j; // as the algo progresses next[i][j] is updated to store the next vertex on the shortest path from i to j.
                }
            }
        }

        // reference Brilliant.org on Floyd Warshall Algorithm general structure

        for (int k = 0; k < numCrops; k++){ // k is the intermediate crop in the path from i to j
            for (int i = 0; i < numCrops; i++){ // i is start crop
                for (int j = 0; j < numCrops; j++){ // j is end crop

                    // first deviation from standard FW algo - accounts for all new nutrient differences (NPK) through individual crop k
                    int newN = get<0>(dist[i][k]) + get<0>(dist[k][j]);
                    int newP = get<1>(dist[i][k]) + get<1>(dist[k][j]);
                    int newK = get<2>(dist[i][k]) + get<2>(dist[k][j]);

                    // calculate the sum of these tuples ^^ for CURRENT path but take absolute values
                    // refer to line 125 : "... add the absolute values of the tuple to get cumulative distance in terms of nutrient values left"
                    int currentSum = abs(get<0>(dist[i][j])) + abs(get<1>(dist[i][j])) + abs(get<2>(dist[i][j]));
                    int newSum = abs(newN) + abs(newP) + abs(newK); // gets sum for the NEW path

                    // reference from Geeks for Geeks "Finding shortest path between any two nodes using Floyd Warshall Algorithm"
                    if (newSum < currentSum) { // checks if NEW path through k has a smaller sum of nutrient differences
                        dist[i][j] = make_tuple(newN, newP, newK); // updates the distance matrix
                        next[i][j] = next[i][k]; // updates next crop in the OPTIMAL path**
                    }

                }
            }
        }

        // reconstructing the path
        vector<string> cropSequence;
        int start = -1;
        int end = -1;
        // finds indexes where start and end are
        for (int i = 0; i < numCrops; i++){
            if (crops[i].name == startCrop){
                start = i;
            }
            if (crops[i].name == endCrop){
                end = i;
            }
        }
        // referenced lines 153-171 for related if/else structure below
        if (start == -1 || end == -1 || next[start][end] == -1) { // check if start and end crops even exist
            cout << "No path exists." << endl;
            return cropSequence; // Return empty sequence if no path exists
        }

        // redone section of code to reconstruct optimal path using the next MATRIX, which stores the next crop in the optimal path for each pair of crops
        unordered_set<int> visitedNodes;
        int current = start;
        cropSequence.push_back(startCrop);
        visitedNodes.insert(current);

        // implemented a check to prevent infinitely running program due to loops/cycles of already visited crops
        while (current != end){
            int nextNode = next[current][end];
            if (visitedNodes.count(nextNode) > 0) {
                cout << "Cycle detected. Finding alternative path." << endl;


                break;
            }

            current = nextNode;
            cropSequence.push_back(crops[current].name);
            visitedNodes.insert(current);
        }

        // after adding those crops stuck in a cycle:
        // if it hasn't reached the end crop, find the best path through remaining crops
        if (current != end) {
            vector<int> remainingCrops;
            for (int i = 0; i < numCrops; i++) {
                if (visitedNodes.count(i) == 0) {
                    remainingCrops.push_back(i);
                }
            }
            // if path reconstruction fails, fallback is a greedy approach
            // sort remaining (dynamically) OPTIMAL crops based on nutrient waste
            sort(remainingCrops.begin(), remainingCrops.end(),
                 [&](int a, int b) {
                     int sumA = abs(get<0>(dist[current][a])) + abs(get<1>(dist[current][a])) + abs(get<2>(dist[current][a]));
                     int sumB = abs(get<0>(dist[current][b])) + abs(get<1>(dist[current][b])) + abs(get<2>(dist[current][b]));
                     return sumA < sumB;
                 });

            // add remaining crops to sequence
            for (int crop : remainingCrops) {
                if (crop != end) {
                    cropSequence.push_back(crops[crop].name);
                    current = crop;
                }
            }

            // ensure user inputted end crop is the last in sequence
            if (cropSequence.back() != endCrop) {
                cropSequence.push_back(endCrop);
            }
        }

        // structure to emulate output from bellman ford algorithm
        cout << "The following crop sequence minimizes overall nutrient waste from " << startCrop << " to " << endCrop << ": " << endl;
        for (int i = 0; i < cropSequence.size(); i++) {
            if (i == cropSequence.size() - 1) {
                cout << cropSequence[i] << endl;
            } else {
                cout << cropSequence[i] << " --> ";
            }
        }
        cout << endl;

        tuple<int, int, int> leftovers = dist[start][end];
        cout << "Nutrients Leftover: " << get<0>(leftovers) << " Nitrogen, " << get<1>(leftovers) << " Phosphorus, " << get<2>(leftovers) << " Potassium" << endl;

        return cropSequence;
    }

};

#endif