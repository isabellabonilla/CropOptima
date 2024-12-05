<H1> COP3530 - Project 3 </h1>

Respective TAs: Matthew DeGuzman (matthewdeguzman@ufl.edu), Ananya Sista (sistaa@ufl.edu)

Crop Optima: README
Welcome to Crop Optima, where the Dirt Detective is on the case! This program helps you determine optimal crop rotations for efficient nutrient usage and soil health.

Features
- Load and analyze crop data for a specific U.S. state.
- Suggest crop sequences based on compatibility and soil health.
- Compare algorithms for finding optimal crop paths:
  - Bellman-Ford
  - Floyd-Warshall

Prerequisites
Ensure you have the following:
- C++ Compiler: A compiler supporting C++11 or newer (e.g., GCC, Clang, MSVC).
- Dataset: The crop nutrient dataset (cropNutrientDataset.csv) placed in the dataset/ directory.

Building and Running the Program
Step 1: Clone the Repository
- git clone https://github.com/your-repo-name/crop-optima.git
- cd crop-optima
Step 2: Compile the Code
- Run the following command to compile the program:
- g++ -std=c++11 main.cpp Graph.cpp Utility.cpp -o crop_optima
Step 3: Run the Program
- Execute the program using:
- ./crop_optima

How to Use
1. Launch the Program: Follow the welcome prompts to get started.
2. Input State: Provide your state to load crop data.
3. Select Crops: Choose a starting and ending crop for rotation.
4. Analyze Results: View crop sequences and execution times for Bellman-Ford and Floyd-Warshall algorithms.
5. Enjoy the ASCII Art: End your session with a friendly farming farewell!

File Structure
- main.cpp: Contains the program logic and user interaction.
- Graph.h/Graph.cpp: Implements graph-based algorithms.
- Utility.h/Utility.cpp: Helper functions for file reading and data handling.
- dataset/cropNutrientDataset.csv: Crop nutrient data for different U.S. states.
