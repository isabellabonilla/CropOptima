#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
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
    vector<vector<int>> adjMatrix; // adjacency matrix that holds the compatibility values between crops

    int getCompatibility(Crop& crop1, Crop& crop2) { // measures difference in crop2's nutrient requirements and crop1's nutrient output
        int compatibility = 0;
        compatibility += abs(crop1.outputN - crop2.inputN); // difference in nitrogen compatibility
        compatibility += abs(crop1.outputP - crop2.inputP); // difference in phosphorus compatibility
        compatibility += abs(crop1.outputK - crop2.inputK); // difference in potassium compatibility

        return compatibility;
    }

    public:

    Graph(const vector<Crop>& crops){ // graph constructor
        this->crops = crops;
        this->numCrops = crops.size();

        // initialize adjacency matrix with the correct size and compatability values (edge weights) to 0 
        adjMatrix = vector<vector<int>>(numCrops, vector<int>(numCrops, 0)); 
    }

    void populate() { // iterate through the entire matrix generating the compatibility scores between each crop (edge weights)
        for(int i = 0; i < numCrops; i++) {
            for(int j = 0; j < numCrops; j++) {
                if(i != j) { // do not add a compatibility score for a crop to itself
                    int weight = getCompatibility(crops[i], crops[j]);
                    adjMatrix[i][j] = weight;
                }
            }
        }
    }

    void printMatrix() {
        cout << "Adjacency Matrix: Compatibility Scores" << endl;
        cout << "       ";

        for(int i = 0; i < 10; i++) {
            cout << crops[i].name << "        ";
        }
        cout << endl;

        for(int i = 0; i < 10; i++) {
            cout << crops[i].name << "          ";

            for(int j = 0; j < 10; j++) {
                cout << adjMatrix[i][j] << "          ";
            }
            cout << endl;
        }
        cout << endl;
    }
};

#endif                