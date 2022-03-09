/* ************************************************************
 * PROGRAMMER  :    BRYCE BERWALD
 * PROJECT     :    SAVAGES
 * VERSION     :    0.0.1
 * DESCRIPTION :    AN RPG GAME CREATED FOR LEARNING PURPOSES
 **************************************************************/

#include "BlueFurry.h"

/*****************************************************
 * 
*****************************************************/
BlueFurry::BlueFurry(){
    this->npc_up = LoadTexture("resources/NPCs/blue_furry/blue_furry_up.png");
    this->npc_down = LoadTexture("resources/NPCs/blue_furry/blue_furry_down.png");
    this->npc_left = LoadTexture("resources/NPCs/blue_furry/blue_furry_left.png");
    this->npc_right = LoadTexture("resources/NPCs/blue_furry/blue_furry_right.png");
    this->npc_current = this->npc_down;

    this->PositionX = 500 + (rand() % 356);
    this->PositionY = 777 + (rand() % 574);

    this->direction = 'S';

    this->Location = {(float)PositionX, (float)PositionY, 32, 32};

    this->movementTimer = 0;

    this->health = 10.0;
    this->displayHealth = 100.0;

    this->level = 1;
}


/*****************************************************
 * 
*****************************************************/
BlueFurry::~BlueFurry(){
    UnloadTexture(npc_up);
    UnloadTexture(npc_down);
    UnloadTexture(npc_left);
    UnloadTexture(npc_right);
    UnloadTexture(npc_current);
}


/*****************************************************
 * 
*****************************************************/
void BlueFurry::moveBlueFurry(string direction){
    if(direction == "left"){
        this->PositionX -= 5;
        this->direction = 'W';
        this->npc_current = this->npc_left;
    } else if(direction == "right"){
        this->PositionX += 5;
        this->direction = 'E';
        this->npc_current = this->npc_right;
    } else if(direction == "up"){
        this->PositionY -= 5;
        this->direction = 'N';
        this->npc_current = this->npc_up;
    } else if (direction == "down"){
        this->PositionY += 5;
        this->direction = 'S';
        this->npc_current = this->npc_down;
    }

    this->Location = {(float)PositionX, (float)PositionY, 32, 32};
}


/*****************************************************
 * 
*****************************************************/
void BlueFurry::updateBlueFurryLocation(float x, float y, float width, float height){
    this->Location = {x, y, width, height};
}


/*****************************************************
 * 
*****************************************************/
void BlueFurry::updateHP(float damage){
 this->health -= damage;
}


/*****************************************************
 * 
*****************************************************/
void BlueFurry::respawnBlueFurry(){
    this->PositionX = 500 + (rand() % 356);
    this->PositionY = 777 + (rand() % 574);

    this->health = 10;
}


/*****************************************************
 * 
*****************************************************/
void BlueFurry::setMovementTimer(int value){
    this->movementTimer = value;
}


/*****************************************************
 * 
*****************************************************/
void BlueFurry::incrementMovementTimer(){
    this->movementTimer++;
}


/*****************************************************
 * 
*****************************************************/
int BlueFurry::getMovementTimerValue()const{
    return this->movementTimer;
}


/*****************************************************
 * 
*****************************************************/
int BlueFurry::getPositionX()const{
    return this->PositionX;
}


/*****************************************************
 * 
*****************************************************/
int BlueFurry::getPositionY()const{
    return this->PositionY;
}

/*****************************************************
 * 
*****************************************************/
float BlueFurry::getHealth()const{
    return this->health;
}


/*****************************************************
 * 
*****************************************************/
Texture2D BlueFurry::getCurrentTexture()const {
    return this->npc_current;
}


/*****************************************************
 * 
*****************************************************/
Rectangle BlueFurry::getBlueFurryLocation()const{
    return this->Location;
}


/*****************************************************
 * 
*****************************************************/
char BlueFurry::getBlueFurryDirection()const{
    return this->direction;
}


/********************************************
 * 
*********************************************/
float BlueFurry::getDisplayHealth()const{
    return this->health * 3;
}


/********************************************
 * 
*********************************************/
int BlueFurry::getLevel()const{
    return this->level;
}