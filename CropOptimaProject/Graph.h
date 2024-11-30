#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include <iomanip>
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
};

#endif