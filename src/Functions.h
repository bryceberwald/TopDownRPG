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
#include <sstream>
#include <fstream>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
using namespace std;

enum Boundary {
    NONE,
    VILLAGE_MAIN,
    VILLAGE_NORTH,
    VILLAGE_EAST,
    VILLAGE_SOUTH,
    VILLAGE_WEST,
};

enum GameMenuScreen {
    PreGameScreen,
    InGameScreen,
};

enum InGameMenuOptions {
    INVENTORY,
    QUESTS,
    SKILLS,
    EQUIPPED,
    SETTINGS,
    OTHER,
};

const int ROW_SIZE = 50; // CONST - Used to set the row size of the game map array.
const int COL_SIZE = 50; // CONST - Used to set the column size of the game map array.

const int TOTAL_BLOCKED_LOCATIONS = 50;

// Function Prototypes
int LocationsFromFile(int binaryDigit);

#endif /* Functions_h */
