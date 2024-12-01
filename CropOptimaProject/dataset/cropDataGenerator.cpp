#include <iostream>
#include <fstream>
#include <vector>
#include <random>

using namespace std;

// function to read crop names from the unique crops CSV data
vector<string> readCrops(const string& filename) {
    vector<string> crops;
    ifstream file(filename);
    string line;
    
    getline(file, line); // skip first line label/descriptor
    
    while (getline(file, line)) {
        crops.push_back(line);
    }
    return crops;
}

// this function uses mersenne_twister_engine (mt) and distribution function w/ random standard library to generate random NPK values (0-50) for each crop I/O
int generateRandomNPK() { 
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> dis(0, 50);
    return dis(gen);
}

// we will run the script only once in CL to write to a csv file for 140k 'points' of generated crop data
int main() {
    vector<string> allCrops = readCrops("UniqueCrops.csv"); // function reads all the 461 unique crops

    // this states vector is already alphabetized, so script will read every state in order, then to create its crops
    vector<string> states = {"Alabama", "Alaska", "Arizona", "Arkansas", "California", "Colorado", "Connecticut", "Delaware",
                            "Florida", "Georgia", "Hawaii", "Idaho", "Illinois", "Indiana", "Iowa", "Kansas", "Kentucky",
                            "Louisiana", "Maine", "Maryland", "Massachusetts", "Michigan", "Minnesota", "Mississippi",
                            "Missouri", "Montana", "Nebraska", "Nevada", "New Hampshire", "New Jersey", "New Mexico",
                            "New York", "North Carolina", "North Dakota", "Ohio", "Oklahoma", "Oregon", "Pennsylvania",
                            "Rhode Island", "South Carolina", "South Dakota", "Tennessee", "Texas", "Utah", "Vermont",
                            "Virginia", "Washington", "West Virginia", "Wisconsin", "Wyoming"};

    ofstream outFile("cropNutrientDataset.csv"); // just setting the name of the output file to write to

    outFile << "US State,Crop Name,Input N,Output N,Input P,Output P,Input K,Output K\n"; // header of the output, with column labels for data

    mt19937 gen(random_device{}()); // use random generator library and random_device function

    for (const auto& state : states) { // for every state (in alphabetical order), resize the random crops it can have from 350-450 per state
        vector<string> stateCrops = allCrops;
        shuffle(stateCrops.begin(), stateCrops.end(), gen);
        stateCrops.resize(400);

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

    // Ran this with 'g++ cropDataGenerator.cpp -o cropDataGenerator -std=c++11' command setup in terminal
    // it created an executable 'cropDataGenerator' file, ran it with ./cropDataGenerator, it created the data set cropNutrientDataset.csv!

    outFile.close();
   cout << "Dataset created successfully with 20,000 rows (50 states x 400 unique crops), each with 7 additional points of name & nutrient info (140,000 points of data)." << endl;

    return 0;
}