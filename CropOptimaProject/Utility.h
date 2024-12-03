#ifndef UTILITY_H
#define UTILITY_H

#include <fstream>
#include <sstream>
#include "Graph.h"
using namespace std;

vector<Crop> readStateCropData(const string& filename, const string& state) { // read crop data from csv file and create vector of crop objects for the user-specified state
    vector<Crop> crops;

    ifstream file(filename);

    if(!file.is_open()) {
        throw runtime_error("Could not open " + filename);
    }

    string line;
    getline(file, line); // skip header line of csv
    bool stateFound;

    while(getline(file, line)) { // read each crop's data
        stringstream ss(line);
        string cropState, name, strInputN, strOutputN, strInputP, strOutputP, strInputK, strOutputK;

        getline(ss, cropState, ',');
        getline(ss, name, ',');
        getline(ss, strInputN, ',');
        getline(ss, strOutputN, ',');
        getline(ss, strInputP, ',');
        getline(ss, strOutputP, ',');
        getline(ss, strInputK, ',');
        getline(ss, strOutputK); 

        if (cropState == state) {
            // convert inputs and outputs to integers
            stateFound = true;
            int inputN = stoi(strInputN);
            int outputN = stoi(strOutputN);
            int inputP = stoi(strInputP);
            int outputP = stoi(strOutputP);
            int inputK = stoi(strInputK);
            int outputK = stoi(strOutputK);

            // create new crop object and add it to the vector of crops
            Crop crop(name, inputN, outputN, inputP, outputP, inputK, outputK);
            crops.push_back(crop); 
        }

        if(cropState != state && stateFound){ // if the next line has the wrong state, all crops for that state have been collected (csv is in alphabetical order) so break out of the loop
            break;
        }
    }

    file.close();
    return crops;
 }

 #endif