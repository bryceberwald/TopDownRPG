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

#include <iostream>
#include <iomanip>
#include <string>
#include <ostream>
#include <sstream>
#include <fstream>

#include <string.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

const int ROW_SIZE = 50; // CONST - Used to set the row size of the game map array.
const int COL_SIZE = 50; // CONST - Used to set the column size of the game map array.

// Function Prototypes

// Prototype description goes here...
int LocationsFromFile(int binaryDigit);


#endif /* Functions_h */
