#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <sstream>

using namespace std;

// function to read crop names from the 3k crops CSV data
vector<string> readCrops(const string& filename) {
    vector<string> crops;
    ifstream file(filename);
    string line;
    
    getline(file, line); // skip first line label
    
    while (getline(file, line)) {
        crops.push_back(line);
    }
    return crops;
}

// this function uses mersenne_twister_engine and distribution function w/ random standard libray to generate random NPK values (0-50) for each crop I/O
int generateRandomNPK() { 
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> dis(0, 50);
    return dis(gen);
}

// we will run the script only once in the main function to write to a csv file for 100k points of generated crop data
int main() {
    vector<string> allCrops = readCrops("3000_crops_list.csv"); // function reads all the 3k unique crops we have in the 3000_crops_list.csv

    // this states vector is already alphabetized, so script will read every state in order, then to create its crops
    vector<string> states = {"Alabama", "Alaska", "Arizona", "Arkansas", "California", "Colorado", "Connecticut", "Delaware",
                            "Florida", "Georgia", "Hawaii", "Idaho", "Illinois", "Indiana", "Iowa", "Kansas", "Kentucky",
                            "Louisiana", "Maine", "Maryland", "Massachusetts", "Michigan", "Minnesota", "Mississippi",
                            "Missouri", "Montana", "Nebraska", "Nevada", "New Hampshire", "New Jersey", "New Mexico",
                            "New York", "North Carolina", "North Dakota", "Ohio", "Oklahoma", "Oregon", "Pennsylvania",
                            "Rhode Island", "South Carolina", "South Dakota", "Tennessee", "Texas", "Utah", "Vermont",
                            "Virginia", "Washington", "West Virginia", "Wisconsin", "Wyoming"};

    ofstream outFile("crop_nutrient_dataset.csv"); // just setting the name of the output file to write to

    outFile << "US State,Crop Name,Input N,Output N,Input P,Output P,Input K,Output K\n"; // header of the output, with column labels for data

    mt19937 gen(random_device{}()); // use random generator library and random_device function

    for (const auto& state : states) { // for every state (in alphabetical order), resize the random crops it can have from 3k to 2k per state
        vector<string> stateCrops = allCrops;
        shuffle(stateCrops.begin(), stateCrops.end(), gen);
        stateCrops.resize(2000);

        for (const auto& crop : stateCrops) { // for every crop, writes a random I/O for Nitrogen, Phosphorus, and Potassium
            outFile << state << "," // COMMA SEPARATED VALUES (CSV) we will read from
                    << crop << ","
                    << generateRandomNPK() << ","
                    << generateRandomNPK() << ","
                    << generateRandomNPK() << ","
                    << generateRandomNPK() << ","
                    << generateRandomNPK() << ","
                    << generateRandomNPK() << "\n";
        }
    }

    // Try and ran this with 'g++ cropDataGenerator.cpp -o cropDataGenerator' command setup in terminal
    // it created an executable 'cropDataGenerator' file, ran it with ./cropDataGenerator, it created the data set crop_nutrient_dataset.csv!

    outFile.close();
   cout << "Dataset created successfully with 100,000 rows." << endl;

    return 0;
}