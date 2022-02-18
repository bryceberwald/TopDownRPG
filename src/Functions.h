/* ************************************************************
 * PROGRAMMER  :    BRYCE BERWALD
 * PROJECT     :    SAVAGES
 * VERSION     :    0.0.1
 * DESCRIPTION :    AN RPG GAME CREATED FOR LEARNING PURPOSES
 **************************************************************/

#ifndef Functions_h
#define Functions_h

// Pre-processor directives
#include "../include/raylib.h"
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <math.h>

#include <stdlib.h> // For random numbers to be generated
#include <cctype>   // For toUpper() function to be used

#include <iostream>
#include <iomanip>
#include <string>
#include <ostream>
#include <sstream>
#include <fstream>
#include <vector>

#include <string.h>
#include <unistd.h>
#include <stdio.h>
using std::vector; 
using namespace std;
using namespace std::chrono;

enum Boundary {
    NONE,
    MAP_1,
    MAP_2,
    MAP_3,
};

const int ROW_SIZE = 50; // CONST - Used to set the row size of the game map array.
const int COL_SIZE = 50; // CONST - Used to set the column size of the game map array.

const int TOTAL_BLOCKED_LOCATIONS = 50;

// Function Prototypes

// Prototype description goes here...
int LocationsFromFile(int binaryDigit);

#endif /* Functions_h */
