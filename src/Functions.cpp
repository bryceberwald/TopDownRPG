/* ************************************************************
 * PROGRAMMER  :    BRYCE BERWALD
 * PROJECT     :    SAVAGES
 * VERSION     :    0.0.1
 * DESCRIPTION :    AN RPG GAME CREATED FOR LEARNING PURPOSES
 **************************************************************/

#include "Functions.h"

/* ************************************************************
 * Function description goes here...
 **************************************************************/
int LocationsFromFile(int binaryDigit){

    int result;

    if(binaryDigit == 0 || binaryDigit == 1){
        result = binaryDigit;
    } else {
        result = -1;
    }

    return result;
}
