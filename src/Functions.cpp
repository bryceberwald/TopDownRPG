/* ************************************************************
 * PROGRAMMER  :    BRYCE BERWALD
 * PROJECT     :    SAVAGES
 * VERSION     :    0.0.1
 * DESCRIPTION :    AN RPG GAME CREATED FOR LEARNING PURPOSES
 **************************************************************/

#include "Functions.h"

/* ************************************************************
 * This is a helper function for reading in blocked locations
 * from an input file. The function will check to ensure that 
 * all ones and zeros have been read from the file and nothing
 * else. A negative 1 will be returned otherwise.
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
