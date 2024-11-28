#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_map>
#include <string>
#include "Utility.h"
using namespace std;


struct Crop {
    string name;
    int inputN, outputN;
    int inputP, outputP;
    int inputK, outputK;
    Crop(string name, int inputN, int outputN, int inputP, int outputP, int inputK, int ouputK) {
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
    unordered_map<string, int> mapper; // map to store crop names and their associated index in adjacency matrix
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

        for(int i = 0; i < crops.size(); i++) { // directly populating mapper with crop names and their associated index in the adjacency matrix
            mapper[crops[i].name] = i;
        }
    }

    // TENTATIVE BECAUSE IM EDITTING THE GRAPH CONSTRUCTOR
    // void addCrop(Crop& crop) {
    //     mapper[crop.name] = numCrops; // map the new crop name to the next index in the adjacency mmatrix
    //     numCrops++;
    // }

    // void addEdge(string& from, string& to, int compatibility) {
    //     if(mapper.find(from) == mapper.end()) { 
    //         mapper[from] = numCrops++; // assign an index to the new crop
    //     }
    //     if(mapper.find(to) == mapper.end()) {
    //         mapper[to] = numCrops++; // assign an index to the new crop
    //     }

    //     adjMatrix[mapper[from]][mapper[to]] = compatibility; // add a compatbility value between crops in adjacency matrix
    // }

    // void populateGraph() {

    // }





};

#endif                