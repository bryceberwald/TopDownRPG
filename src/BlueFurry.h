/* ************************************************************
 * PROGRAMMER  :    BRYCE BERWALD
 * PROJECT     :    SAVAGES
 * VERSION     :    0.0.1
 * DESCRIPTION :    AN RPG GAME CREATED FOR LEARNING PURPOSES
 **************************************************************/

#ifndef BLUEFURRY_H
#define BLUEFURRY_H

#include "../include/raylib.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>
#include <string>
using namespace std;

/*****************************************************
 * 
*****************************************************/
class BlueFurry {
    public:
        /********************************************
         * 
        *********************************************/
        BlueFurry();

        /********************************************
         * 
        *********************************************/
        ~BlueFurry();

        /********************************************
         * 
        *********************************************/
        void moveBlueFurry(string direction);

        /********************************************
         * 
        *********************************************/
        void updateBlueFurryLocation(float x, float y, float width, float height);

        /********************************************
         * 
        *********************************************/
        void setMovementTimer(int value);

        /********************************************
         * 
        *********************************************/
        void incrementMovementTimer();

        /********************************************
         * 
        *********************************************/
        int getMovementTimerValue()const;

        /********************************************
         * 
        *********************************************/
        int getPositionX()const;

        /********************************************
         * 
        *********************************************/
        int getPositionY()const;

        /********************************************
         * 
        *********************************************/
        Texture2D getCurrentTexture()const;

        /********************************************
         * 
        *********************************************/
        Rectangle getBlueFurryLocation()const;

        /********************************************
         * 
        *********************************************/
        char getBlueFurryDirection()const;

    private:
        Texture2D npc_up;
        Texture2D npc_down;
        Texture2D npc_left;
        Texture2D npc_right;
        Texture2D npc_current;
        int PositionX;
        int PositionY;
        char direction;
        Rectangle Location;
        int movementTimer;
        int totalHealth;
        int currentHealth;
        int level;
};

#endif