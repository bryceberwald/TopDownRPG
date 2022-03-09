/* ************************************************************
 * PROGRAMMER  :    BRYCE BERWALD
 * PROJECT     :    SAVAGES
 * VERSION     :    0.0.1
 * DESCRIPTION :    AN RPG GAME CREATED FOR LEARNING PURPOSES
 **************************************************************/

#include "Functions.h"
#include "BlueFurry.h"

int main(int argc, char* argv[])
{
    // Change the current directory to the root of the project.
    char path_save[1024];
    char cwd[1024];
    char* p;
    if(!(p = strrchr(argv[0], '/')))
        getcwd(cwd, sizeof(cwd));
    else
    {
        *p = '\0';
        getcwd(path_save, sizeof(path_save));
        chdir(argv[0]);
        getcwd(cwd, sizeof(cwd));
        chdir(path_save);
    }
    string NEW_WORKING_DIR = string(cwd) + "/";
    chdir(NEW_WORKING_DIR.c_str());

    /* initialize random seed: */
    srand (time(NULL));
    
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(1280, 720, "Savages | Version 0.0.1");

    int ScreenWidth  = GetScreenWidth();
    int ScreenHeight = GetScreenHeight();

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
    
    // GUI layout textures.
    Texture2D PreGameBackground = LoadTexture("resources/GUI/start-menu.png");
    Texture2D InGameBackground = LoadTexture("resources/GUI/background.png");

    GameMenuScreen CurrentScreenDisplay = PreGameScreen;  // enum type

    Texture2D BigButtonPressed = LoadTexture("resources/GUI/buttons/big_button_pressed.png");
    Texture2D BigButtonUnpressed = LoadTexture("resources/GUI/buttons/big_button_unpressed.png");
    Texture2D CurrentButtonTexture = BigButtonUnpressed;

    InGameMenuOptions CurrentMenuOptions = INVENTORY; // enum type

    // Player Textures
    Texture2D HumanSpriteSheet = LoadTexture("resources/characters/human_spritesheet.png");
    Texture2D DemonSpriteSheet = LoadTexture("resources/characters/demon_spritesheet.png");
    Texture2D CurrentSpriteSheet = DemonSpriteSheet;

    // Array of 25 Blue Furry Monsters on Village North Map.
    const int MAX_BLUE_FURRYS = 25;
    BlueFurry BlueFurryArray[MAX_BLUE_FURRYS];
    Vector2 BlueFurryCoordinatesVectorArray[MAX_BLUE_FURRYS];
    int attackedFurryIndex= 0;

    // Variable determines which direction player is facing.
    char CharacterDirection = 'S';

    // Control Player X & Y coordinates with these variables.
    int PlayerX = ScreenWidth / 2;
    int PlayerY = 200; 

    /*  Variables to control Player-Animation */
    float FrameX = (float)CurrentSpriteSheet.width/8;
    float FrameY = (float)CurrentSpriteSheet.height/8;

    // Variable holds the current frame of the spritesheet.
    Rectangle frameRec = { 0.0f, 0.0f, FrameX, FrameY };

    // Variables used for player animation support
    bool attackMode = false;
    int attackAnimationTimer = 0;
    int walkingAnimationTimer = 0;

    // Map Textures
    Texture2D VillageMain = LoadTexture("resources/maps/map1/main_village.png");
    Texture2D VillageWest = LoadTexture("resources/maps/map2/main_village_west.png");
    Texture2D VillageEast = LoadTexture("resources/maps/map3/main_village_east.png");
    Texture2D VillageNorth = LoadTexture("resources/maps/map4/main_village_north.png");
    Texture2D VillageSouth = LoadTexture("resources/maps/map5/main_village_south.png");
    Texture2D CurrentMap = VillageMain;

    // String used for creating binary locations into an integer array.
    string inputFileName = "resources/maps/map1/main_village.txt";

    // Variable used for reading binary numbers from input file.
    int binaryDigit;

    // Controls which map the player is currently on.
    Boundary CurrentBounds = VILLAGE_MAIN;

    // Array to hold blocked locations in integer format.
    int GameMapArray[ROW_SIZE][COL_SIZE];  // 50 x 50 for map sizing. (Tiles are 32x32 in size) 1600 x 1600 overall pixel size...

    // Array is used to hold all blocked locations of type rectangle.
    Rectangle BlockedLocations[TOTAL_BLOCKED_LOCATIONS];

    // Variable holds the number of blocked locations in total.
    int totalBlockedLocations = 0;

    // Variable controls when to load map properties or new map properties.
    bool LoadMapProperties = true;

    // Move camera vector with these variables.
    float CameraX = 670;
    float CameraY = 190;

    SetTargetFPS(60);       // Set the game FPS.
    SetMousePosition(0,0);  // Set initial mouse postion to (0,0).

    bool ContinuePlayingGame = true;

    // Main game loop
    while (!WindowShouldClose() && ContinuePlayingGame)    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here

        if(LoadMapProperties) {
            // Save Binary Locations to integer array to be used later for creating blocked locations of rec type.
            ifstream inFile;
            inFile.open(inputFileName);
            int rowCounter = 0;
            if (inFile.is_open()) {
                while (inFile) {
                    for(int i = 0; i < COL_SIZE; i++){
                        inFile >> binaryDigit;
                        GameMapArray[rowCounter][i] = LocationsFromFile(binaryDigit);
                    }
                    rowCounter++;
                }
            }
            LoadMapProperties = false;
        }

        // Variable holds the coordinates & zoom levels of the game camera.
        Camera2D Camera = {{ CameraX, CameraY }, { CameraX, CameraY }, 0.0f, 2.5f };

        // Variable holds X & Y coordinates of the player.
        Vector2 position = { (float)PlayerX, (float)PlayerY };

        // Variables used for draw coordinates for rectangles on blocked locations.
        int recPositionX = 0;
        int recPositionY = 0;

        // Variables used to determine where mouse coordinates are currently at.
        int mouseX = GetMouseX();
        int mouseY = GetMouseY();

        // Variables used to help support collsion detection against player.
        int new_PlayerX = PlayerX;
        int new_PlayerY = PlayerY;
        int new_CameraX = CameraX;
        int new_CameraY = CameraY;

        // NOTE: Texture is scaled twice its size, so it sould be considered on scrolling
        
        //----------------------------------------------------------------------------------
        // BEGIN DRAWING
        //----------------------------------------------------------------------------------
        BeginDrawing();

        switch(CurrentScreenDisplay){
            case PreGameScreen:
                DrawTexture(PreGameBackground, 0, 0, WHITE);

                if(mouseX > 535 && mouseX < 745 && mouseY > 291 && mouseY < 342){
                    DrawLine(535, 290, 745, 290, WHITE);
                    DrawLine(535, 343, 745, 343, WHITE);

                    // Set and/or reset all the players data values to starting values.
                    // --------------------------------------------------------------//
                    // End of reseting player data in text files.

                    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        CurrentScreenDisplay = InGameScreen;
                    }

                } else if (mouseX > 535 && mouseX < 745 && mouseY > 359 && mouseY < 409){
                    DrawLine(535, 358, 745, 358, WHITE);
                    DrawLine(535, 410, 745, 410, WHITE);

                    // Load all saved data from text files or the default starting data if nothing to begin with.
                    // --------------------------------------------------------------//
                    // End of Loading saved data and/or default data values for new users.
                    
                    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        CurrentScreenDisplay = InGameScreen;
                    }

                } else if(mouseX > 535 && mouseX < 745 && mouseY > 429 && mouseY < 478){
                    DrawLine(535, 428, 745, 428, WHITE);
                    DrawLine(535, 479, 745, 479, WHITE);
                } else if (mouseX > 535 && mouseX < 745 && mouseY > 496 && mouseY < 546){
                    DrawLine(535, 495, 745, 495, WHITE);
                    DrawLine(535, 547, 745, 547, WHITE);

                    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        ContinuePlayingGame = false;  // Close Application. (EXIT button)
                    }
                }
      
                break;
            case InGameScreen:
                BeginMode2D(Camera);

                ClearBackground(BLACK);

                DrawTexture(CurrentMap, 0, 0, WHITE);                  // Draw map to the screen.

                Rectangle CharacterSurroundingRec = {(float)PlayerX+24, (float)PlayerY+16, 14, 28};
                //DrawRectanglePro(CharacterSurroundingRec, {0,0}, 0.0f, Fade(RED, 0.5f));

                if(CurrentBounds == VILLAGE_NORTH){
                    for(int i = 0; i < MAX_BLUE_FURRYS; i++){

                        DrawTexture(BlueFurryArray[i].getCurrentTexture(), BlueFurryArray[i].getPositionX(), BlueFurryArray[i].getPositionY(), WHITE);

                        // Assign array values of type Vector2 with the coordinates of Blue Furry's on map.
                        BlueFurryCoordinatesVectorArray[i] = {(float)BlueFurryArray[i].getPositionX()+16, (float)BlueFurryArray[i].getPositionY()+10};
                        
                        BlueFurryArray[i].incrementMovementTimer();

                        if(BlueFurryArray[i].getMovementTimerValue() > 5){

                            int randomNumber = 1 + (rand() % 40);

                            float distance = pow( pow(BlueFurryCoordinatesVectorArray[i].x - (PlayerX+32), 2) + pow(BlueFurryCoordinatesVectorArray[i].y - (PlayerY+32), 2), 0.5f);
                            
                            // Check if any blue furrys are too close to the player, if so start randomly moving them in the other direction.
                            if(distance < 100){
                                if(PlayerX < BlueFurryArray[i].getPositionX() && PlayerY < BlueFurryArray[i].getPositionY()){
                                    // NW or Quadrant #2 from Blue Furry's postion.
                                    switch(randomNumber){
                                        case 1:
                                            BlueFurryArray[i].moveBlueFurry("right");
                                            break;
                                        case 2:
                                            BlueFurryArray[i].moveBlueFurry("right");
                                            break;
                                        case 3:
                                            BlueFurryArray[i].moveBlueFurry("down");
                                            break;
                                        case 4:
                                            BlueFurryArray[i].moveBlueFurry("down");
                                            break;
                                    }
                                // NE or Quadrant #1 from Blue Furry's postion.
                                } else if(PlayerX > BlueFurryArray[i].getPositionX() && PlayerY < BlueFurryArray[i].getPositionY()){
                                    switch(randomNumber){
                                        case 1:
                                            BlueFurryArray[i].moveBlueFurry("left");
                                            break;
                                        case 2:
                                            BlueFurryArray[i].moveBlueFurry("left");
                                            break;
                                        case 3:
                                            BlueFurryArray[i].moveBlueFurry("down");
                                            break;
                                        case 4:
                                            BlueFurryArray[i].moveBlueFurry("down");
                                            break;
                                    }
                                // SW or Quadrant #3 from Blue Furry's postion.
                                } else if(PlayerX < BlueFurryArray[i].getPositionX() && PlayerY > BlueFurryArray[i].getPositionY()){
                                    switch(randomNumber){
                                        case 1:
                                            BlueFurryArray[i].moveBlueFurry("right");
                                            break;
                                        case 2:
                                            BlueFurryArray[i].moveBlueFurry("right");
                                            break;
                                        case 3:
                                            BlueFurryArray[i].moveBlueFurry("up");
                                            break;
                                        case 4:
                                            BlueFurryArray[i].moveBlueFurry("up");
                                            break;
                                    }
                                // SE or Quadrant #4 from Blue Furry's postion.
                                } else if(PlayerX > BlueFurryArray[i].getPositionX() && PlayerY > BlueFurryArray[i].getPositionY()){
                                    switch(randomNumber){
                                        case 1:
                                            BlueFurryArray[i].moveBlueFurry("left");
                                            break;
                                        case 2:
                                            BlueFurryArray[i].moveBlueFurry("left");
                                            break;
                                        case 3:
                                            BlueFurryArray[i].moveBlueFurry("up");
                                            break;
                                        case 4:
                                            BlueFurryArray[i].moveBlueFurry("up");
                                            break;
                                    }
                                }
                            } else {
                                switch(randomNumber){
                                    case 1:
                                        BlueFurryArray[i].moveBlueFurry("left");
                                        break;
                                    case 2:
                                        BlueFurryArray[i].moveBlueFurry("right");
                                        break;
                                    case 3:
                                        BlueFurryArray[i].moveBlueFurry("up");
                                        break;
                                    case 4:
                                        BlueFurryArray[i].moveBlueFurry("down");
                                        break;
                                }
                            }
                            BlueFurryArray[i].setMovementTimer(0);
                        }
                    }
                }

                DrawTextureRec(CurrentSpriteSheet, frameRec, position, WHITE);  // Draw player to the screen.
                
                // Draw Blue Furry health bar and level after player spritesheet to fix the overlapping issue.
                for(int i = 0; i < MAX_BLUE_FURRYS; i++){
                    DrawText("Level 1", BlueFurryArray[i].getPositionX(),BlueFurryArray[i].getPositionY()-25,6, WHITE);
                    DrawRectangle(BlueFurryArray[i].getPositionX(), BlueFurryArray[i].getPositionY()-10, 30, 5, Fade(BLACK, 0.5f));
                    DrawRectangle(BlueFurryArray[i].getPositionX(), BlueFurryArray[i].getPositionY()-10, (int)BlueFurryArray[i].getDisplayHealth(), 5, RED);
                    DrawRectangleLines(BlueFurryArray[i].getPositionX(), BlueFurryArray[i].getPositionY()-10, 30, 5, BLACK);
                }

                // Draw blocked locations for the current map.
                for (int i = 0; i < ROW_SIZE; i++) {
                    for (int k = 0; k < COL_SIZE; k++) {
                        if(GameMapArray[i][k] == 1){
                            BlockedLocations[totalBlockedLocations] = {(float)recPositionX, (float)recPositionY, 32, 32};
                            totalBlockedLocations++;
                        }
                        recPositionX += 32;
                    }
                    recPositionX = 0;
                    recPositionY += 32;
                }

                EndMode2D();

                if(!IsKeyPressed(KEY_LEFT) && !IsKeyPressed(KEY_RIGHT) && !IsKeyPressed(KEY_UP) && !IsKeyPressed(KEY_DOWN) 
                    && !IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_UP) && !IsKeyDown(KEY_DOWN)){
                    if(IsKeyPressed(KEY_Z)){
                        if(CharacterDirection == 'S') {
                            frameRec.y = 0;
                            frameRec.x = 64;
                            attackMode = true;
                        } else if(CharacterDirection == 'N') {
                            frameRec.y = 64;
                            frameRec.x = 64;
                            attackMode = true;
                        } else if(CharacterDirection == 'E'){
                            frameRec.y = 128;
                            frameRec.x = 64;
                            attackMode = true;
                        }else if(CharacterDirection == 'W'){
                            frameRec.y = 192;
                            frameRec.x = 64;
                            attackMode = true;
                        }
                    }

                    if(IsKeyDown(KEY_Z)){
                        if(CharacterDirection == 'S') {
                            frameRec.y = 0;
                            frameRec.x = 64;
                            attackMode = true;
                        } else if(CharacterDirection == 'N') {
                            frameRec.y = 64;
                            frameRec.x = 64;
                            attackMode = true;
                        } else if(CharacterDirection == 'E'){
                            frameRec.y = 128;
                            frameRec.x = 64;
                            attackMode = true;
                        }else if(CharacterDirection == 'W'){
                            frameRec.y = 192;
                            frameRec.x = 64;
                            attackMode = true;
                        }
                    }
                }

                if(attackMode){
                    attackAnimationTimer++;

                    if(attackAnimationTimer >= 15){
                        frameRec.x = 0;
                        attackMode = false;
                        attackAnimationTimer = 0;
                    }
                }

                // Figure out which movement key was pressed first before moving a player with that key.
                string firstMovementKeyPressed = "NONE";
                
                if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN)) {

                    FrameX = (float)CurrentSpriteSheet.width/8;
                    FrameY = (float)CurrentSpriteSheet.height/4;

                    if(IsKeyDown(KEY_LEFT)){
                        CharacterDirection = 'W';
                    } else if(IsKeyDown(KEY_RIGHT)){
                        CharacterDirection = 'E';
                    } else if(IsKeyDown(KEY_UP)){
                        CharacterDirection = 'N';
                    } else if(IsKeyDown(KEY_DOWN)){
                        CharacterDirection = 'S';
                    }

                    if (firstMovementKeyPressed != "NONE") {

                        if (firstMovementKeyPressed == "RIGHT" && !IsKeyDown(KEY_RIGHT)) {
                            firstMovementKeyPressed = "NONE";
                        }

                        if (firstMovementKeyPressed == "LEFT" && !IsKeyDown(KEY_LEFT)) {
                            firstMovementKeyPressed = "NONE";
                        }
                            
                        if (firstMovementKeyPressed == "UP" && !IsKeyDown(KEY_UP)) {
                            firstMovementKeyPressed = "NONE";
                        }
                            
                        if (firstMovementKeyPressed == "DOWN" && !IsKeyDown(KEY_DOWN)) {
                            firstMovementKeyPressed = "NONE";
                        }   
                    } else {
                        if (IsKeyDown(KEY_RIGHT)) {
                            firstMovementKeyPressed = "RIGHT";
                        } else if (IsKeyDown(KEY_LEFT)) {
                            firstMovementKeyPressed = "LEFT";
                        } else if (IsKeyDown(KEY_UP)) {
                            firstMovementKeyPressed = "UP";
                        } else if (IsKeyDown(KEY_DOWN)) {
                            firstMovementKeyPressed = "DOWN";
                        }
                    }
                }

                walkingAnimationTimer++;
                if(walkingAnimationTimer > 5){

                    if(firstMovementKeyPressed == "LEFT"){
                        frameRec.x += 64;
                        frameRec.y = 448;
                    } else if(firstMovementKeyPressed == "RIGHT"){
                        frameRec.x += 64;
                        frameRec.y = 384;
                    } else if(firstMovementKeyPressed == "UP"){
                        frameRec.x += 64;
                        frameRec.y = 320;
                    } else if(firstMovementKeyPressed == "DOWN"){
                        frameRec.x += 64;
                        frameRec.y = 256;
                    }

                    if(frameRec.x >= 320){
                        frameRec.x = 0;
                    }
                    walkingAnimationTimer = 0;

                }

                if(IsKeyReleased(KEY_LEFT)){
                    frameRec.x = 0;
                    frameRec.y = 192;
                } else if(IsKeyReleased(KEY_RIGHT)){
                    frameRec.x = 0;
                    frameRec.y = 128;
                } else if(IsKeyReleased(KEY_UP)){
                    frameRec.x = 0;
                    frameRec.y = 64;
                } else if(IsKeyReleased(KEY_DOWN)){
                    frameRec.x = 0;
                    frameRec.y = 0;
                }

                /* Check Keyboard Input from Player for horizontal movement 
                if the player pressed anything, change the new_PlayerX and new_CameraX variables */
                if (firstMovementKeyPressed == "RIGHT") {
                    new_CameraX += 5;
                    CharacterDirection = 'E';
                    new_PlayerX += 3;
                } 
                if (firstMovementKeyPressed == "LEFT") {
                    new_CameraX -= 5;
                    CharacterDirection = 'W';
                    new_PlayerX -= 3;
                } 

                if(CharacterDirection == 'E'){
                    /* Now change if we are colliding with anything, if yes, go back to the last "safe" position, if not, go there */
                    for (int i = 0; i < totalBlockedLocations; i++) {
                        if (CheckCollisionRecs(BlockedLocations[i], {(float)new_PlayerX,(float)new_PlayerY,40,55})) {
                            new_PlayerX = PlayerX;
                            new_CameraX = CameraX;
                        }
                    }
                    PlayerX = new_PlayerX;
                    CameraX = new_CameraX;
                } else if (CharacterDirection == 'W'){
                    /* Now change if we are colliding with anything, if yes, go back to the last "safe" position, if not, go there */
                    for (int i = 0; i < totalBlockedLocations; i++) {
                        if (CheckCollisionRecs(BlockedLocations[i], {(float)new_PlayerX-5,(float)new_PlayerY,40,55})) {
                            new_PlayerX = PlayerX;
                            new_CameraX = CameraX;
                        }
                    }
                    PlayerX = new_PlayerX;
                    CameraX = new_CameraX;
                }

                /* Check Keyboard Input from Player for vertical movement 
                if the player pressed anything, change the new_PlayerY and new_CameraY variables */
                if(firstMovementKeyPressed == "UP") {
                    new_CameraY -= 5;
                    CharacterDirection = 'N';
                    new_PlayerY -= 3;
                } 
                if (firstMovementKeyPressed == "DOWN") {
                    new_CameraY += 5;
                    CharacterDirection = 'S';
                    new_PlayerY += 3;
                }

                if(CharacterDirection == 'N'){
                    /* Now change if we are colliding with anything, if yes, go back to the last "safe" position, if not, go there */
                    for (int i = 0; i < totalBlockedLocations; i++) {
                        if (CheckCollisionRecs(BlockedLocations[i], {(float)new_PlayerX,(float)new_PlayerY-10,40,55})) {
                            new_PlayerY = PlayerY;
                            new_CameraY = CameraY;
                        }
                    }
                    PlayerY = new_PlayerY;
                    CameraY = new_CameraY;
                } else if (CharacterDirection == 'S'){
                    /* Now change if we are colliding with anything, if yes, go back to the last "safe" position, if not, go there */
                    for (int i = 0; i < totalBlockedLocations; i++) {
                        if (CheckCollisionRecs(BlockedLocations[i], {(float)new_PlayerX,(float)new_PlayerY,40,55})) {
                            new_PlayerY = PlayerY;
                            new_CameraY = CameraY;
                        }
                    }
                    PlayerY = new_PlayerY;
                    CameraY = new_CameraY;
                }

                if(CurrentBounds == VILLAGE_MAIN) {

                    // LEFT-SIDE Of Map1 collision detection to keep player on map.
                    if(PlayerX < -25 && (PlayerY >= 452 || PlayerY <= 530)) {
                        PlayerX = -24;
                        CameraX += 5;
                    }

                    // LEFT-SIDE of Map1 -GO TO: Village West Location
                    if(PlayerX <= -24 && (PlayerY >= 452 && PlayerY <= 530)) {
                        // Code to change map here...
                        LoadMapProperties = true;
                        // change input file name
                        inputFileName = "resources/maps/map2/main_village_west.txt";
                        // change CurrentMap Texture to new map
                        CurrentMap = VillageWest;
                        // Change current bounds to new map.
                        CurrentBounds = VILLAGE_WEST;
                        // change player x & y coordinates to location wanted on new map.
                        PlayerX += 1575;
                        CameraX += 2625;
                        // Reset blocked locations to zero.
                        totalBlockedLocations = 0;
                    }

                    // RIGHT-SIDE Of Map1 collision detection to keep player on map.
                    if(PlayerX > 1562 && (PlayerY >= 428 || PlayerY <= 515)) {
                        PlayerX = 1561;
                        CameraX -= 5;
                    }
                    // RIGHT-SIDE of Map1 -GO TO: Village East Location
                    if(PlayerX >= 1561 && (PlayerY >= 428 && PlayerY <= 515)) {
                        // Code to change map here...
                        LoadMapProperties = true;
                        // change input file name
                        inputFileName = "resources/maps/map3/main_village_east.txt";
                        // change CurrentMap Texture to new map
                        CurrentMap = VillageEast;
                        // Change current bounds to new map.
                        CurrentBounds = VILLAGE_EAST;
                        // change player x & y coordinates to location wanted on new map.
                        PlayerX -= 1575;
                        CameraX -= 2625;
                        // Reset blocked locations to zero.
                        totalBlockedLocations = 0;
                    }

                    // TOP-SIDE Of Map 1 collision detection to keep player on map.
                    if(PlayerY <= -40 && (PlayerX >= 598 || PlayerX <= 875)) {
                        PlayerY = -39;
                        CameraY += 5;
                    }
                    // TOP-SIDE Of Map 1... GO TO: VILLAGE NORTH
                    if(PlayerY <= -39 && (PlayerX >= 598 && PlayerX <= 875)) {
                        // Code to change map here...
                        LoadMapProperties = true;
                        // change input file name
                        inputFileName = "resources/maps/map4/main_village_north.txt";
                        // change CurrentMap Texture to new map
                        CurrentMap = VillageNorth;
                        // Change current bounds to new map.
                        CurrentBounds = VILLAGE_NORTH;
                        // change player x & y coordinates to location wanted on new map.
                        PlayerY += 1575;
                        CameraY += 2625;
                        // Reset blocked locations to zero.
                        totalBlockedLocations = 0;
                    }

                    // BOTTOM-SIDE Of Map 1 collision detection to keep player on map.
                    if(PlayerY >= 1555 && (PlayerX >= 598 || PlayerX <= 875)) {
                        PlayerY = 1554;
                        CameraY -= 5;
                    }
                    
                    // BOTTOM-SIDE Of Map 1... GO TO: Village South!
                    if(PlayerY >= 1554 && (PlayerX >= 598 && PlayerX <= 875)) {
                        // Code to change map here...
                        LoadMapProperties = true;
                        // change input file name
                        inputFileName = "resources/maps/map5/main_village_south.txt";
                        // change CurrentMap Texture to new map
                        CurrentMap = VillageSouth;
                        // Change current bounds to new map.
                        CurrentBounds = VILLAGE_SOUTH;
                        // change player x & y coordinates to location wanted on new map.
                        PlayerY -= 1575;
                        CameraY -= 2625;
                        // Reset blocked locations to zero.
                        totalBlockedLocations = 0;
                    }

                } else if (CurrentBounds == VILLAGE_WEST) {
                    //LEFT
                    if(PlayerX < -25) {
                        PlayerX = -24;
                        CameraX += 5;
                    }
                    // RIGHT-SIDE Of Map2 collision detection to keep player on map.
                    if(PlayerX > 1562 && (PlayerY >= 452 || PlayerY <= 530)) {
                        PlayerX = 1561;
                        CameraX -= 5;
                    }
                    // RIGHT-SIDE of Map2 collision detection to send player to a Map1.
                    if(PlayerX >= 1561 && (PlayerY >= 452 && PlayerY <= 530)) {
                        // Code to change map here...
                        LoadMapProperties = true;
                        // change input file name
                        inputFileName = "resources/maps/map1/main_village.txt";
                        // change CurrentMap Texture to new map
                        CurrentMap = VillageMain;
                        // Change current bounds to new map.
                        CurrentBounds = VILLAGE_MAIN;
                        // change player x & y coordinates to location wanted on new map.
                        PlayerX -= 1575;
                        CameraX -= 2625;
                        // Reset blocked locations to zero.
                        totalBlockedLocations = 0;
                    }
                    // TOP
                    if(PlayerY <= -40) {
                        PlayerY = -39;
                        CameraY += 5;
                    }
                    // BOTTOM
                    if(PlayerY >= 1555) {
                        PlayerY = 1554;
                        CameraY -= 5;
                    }
                } else if (CurrentBounds == VILLAGE_EAST) {
                    
                    // LEFT
                    if(PlayerX < -25 && (PlayerY >= 428 || PlayerY <= 515)) {
                        PlayerX = -24;
                        CameraX += 5;
                    }

                    // LEFT
                    if(PlayerX <= -24 && (PlayerY >= 428 && PlayerY <= 515)){
                        // Code to change map here...
                        LoadMapProperties = true;
                        // change input file name
                        inputFileName = "resources/maps/map1/main_village.txt";
                        // change CurrentMap Texture to new map
                        CurrentMap = VillageMain;
                        // Change current bounds to new map.
                        CurrentBounds = VILLAGE_MAIN;
                        // change player x & y coordinates to location wanted on new map.
                        PlayerX += 1575;
                        CameraX += 2625;
                        // Reset blocked locations to zero.
                        totalBlockedLocations = 0;
                    }

                    // RIGHT-SIDE Of Map2 collision detection to keep player on map.
                    if(PlayerX > 1562) {
                        PlayerX = 1561;
                        CameraX -= 5;
                    }

                    // TOP
                    if(PlayerY <= -40) {
                        PlayerY = -39;
                        CameraY += 5;
                    }
                    // BOTTOM
                    if(PlayerY >= 1555) {
                        PlayerY = 1554;
                        CameraY -= 5;
                    }

                } else if(CurrentBounds == VILLAGE_NORTH){
                    // LEFT
                    if(PlayerX < -25) {
                        PlayerX = -24;
                        CameraX += 5;
                    }

                    // RIGHT
                    if(PlayerX > 1562) {
                        PlayerX = 1561;
                        CameraX -= 5;
                    }

                    // TOP
                    if(PlayerY <= -40) {
                        PlayerY = -39;
                        CameraY += 5;
                    }

                    // BOTTOM
                    if(PlayerY > 1556 && (PlayerX >= 598 || PlayerX <= 875)) {
                        PlayerY = 1555;
                        CameraY -= 5;
                    }

                    if(PlayerY >= 1555 && (PlayerX >= 598 && PlayerX <= 875)){
                        // Code to change map here...
                        LoadMapProperties = true;
                        // change input file name
                        inputFileName = "resources/maps/map1/main_village.txt";
                        // change CurrentMap Texture to new map
                        CurrentMap = VillageMain;
                        // Change current bounds to new map.
                        CurrentBounds = VILLAGE_MAIN;
                        // change player x & y coordinates to location wanted on new map.
                        PlayerY -= 1575;
                        CameraY -= 2625;
                        // Reset blocked locations to zero.
                        totalBlockedLocations = 0;
                    }

                    // Check for Player Collision against Blue Furry's on North Map.
                    for(int i = 0; i < MAX_BLUE_FURRYS; i++){
                        if(CheckCollisionCircleRec(BlueFurryCoordinatesVectorArray[i], 9, CharacterSurroundingRec)){
                            attackedFurryIndex = i;
                            if(CharacterDirection == 'E'){
                                PlayerX -= 3;
                                CameraX -= 5;
                            } else if(CharacterDirection == 'W'){
                                PlayerX += 3;
                                CameraX += 5;
                            } else if(CharacterDirection == 'N'){
                                PlayerY += 3;
                                CameraY += 5;
                            } else if(CharacterDirection == 'S'){
                                PlayerY -= 3;
                                CameraY -= 5;
                            }
                        }
                    }

                    float PlayerToFurryDistance = pow( pow(BlueFurryCoordinatesVectorArray[attackedFurryIndex].x - (PlayerX+32), 2) + pow(BlueFurryCoordinatesVectorArray[attackedFurryIndex].y - (PlayerY+32), 2), 0.5f);
                    int randomAttack = 1 + (rand() % 5);

                    // Check if player is attacking a blue furry within distance.
                    if((CharacterDirection == 'S') && PlayerToFurryDistance < 28 && IsKeyPressed(KEY_Z)){
                        if(randomAttack == 1){
                            BlueFurryArray[attackedFurryIndex].updateHP(0.5);
                        }
                    } else if ((CharacterDirection == 'E') && PlayerToFurryDistance < 26 && (IsKeyDown(KEY_Z) || IsKeyPressed(KEY_Z))){
                        if(randomAttack == 1){
                            BlueFurryArray[attackedFurryIndex].updateHP(0.5);
                        }
                    } else if ((CharacterDirection == 'W') && PlayerToFurryDistance < 26 && (IsKeyDown(KEY_Z) || IsKeyPressed(KEY_Z))){
                        if(randomAttack == 1){
                            BlueFurryArray[attackedFurryIndex].updateHP(0.5);
                        }
                    } else if ((CharacterDirection == 'N') && PlayerToFurryDistance < 28 && (IsKeyDown(KEY_Z) || IsKeyPressed(KEY_Z))){
                        if(!(PlayerX < BlueFurryArray[attackedFurryIndex].getPositionX() && PlayerY < BlueFurryArray[attackedFurryIndex].getPositionY()) && !(PlayerX > BlueFurryArray[attackedFurryIndex].getPositionX() && PlayerY < BlueFurryArray[attackedFurryIndex].getPositionY())){
                            if(randomAttack == 1){
                                BlueFurryArray[attackedFurryIndex].updateHP(0.5);
                            }
                        }
                    }

                    // Check if Furry being attacked has died yet... health = zero check!
                    if(BlueFurryArray[attackedFurryIndex].getHealth() <= 0){
                        BlueFurryArray[attackedFurryIndex].respawnBlueFurry();
                    }

                } else if(CurrentBounds == VILLAGE_SOUTH) {
                    // LEFT
                    if(PlayerX < -25) {
                        PlayerX = -24;
                        CameraX += 5;
                    }

                    // RIGHT
                    if(PlayerX > 1562) {
                        PlayerX = 1561;
                        CameraX -= 5;
                    }

                    // TOP
                    if(PlayerY <= -40  && (PlayerX >= 598 || PlayerX <= 875)) {
                        PlayerY = -39;
                        CameraY += 5;
                    }

                    // TOP... GO TO: Main Village!
                    if(PlayerY <= -39  && (PlayerX >= 598 && PlayerX <= 875)) {
                        // Code to change map here...
                        LoadMapProperties = true;
                        // change input file name
                        inputFileName = "resources/maps/map1/main_village.txt";
                        // change CurrentMap Texture to new map
                        CurrentMap = VillageMain;
                        // Change current bounds to new map.
                        CurrentBounds = VILLAGE_MAIN;
                        // change player x & y coordinates to location wanted on new map.
                        PlayerY += 1575;
                        CameraY += 2625;
                        // Reset blocked locations to zero.
                        totalBlockedLocations = 0;
                    }

                    // BOTTOM
                    if(PlayerY >= 1556) {
                        PlayerY = 1555;
                        CameraY -= 5;
                    }
                }

                DrawTexture(InGameBackground, 0, 0, WHITE);

                // Right-Top Big Button!
                if(mouseX >= 105 && mouseX <= 334 && mouseY >= 485 && mouseY <= 532){
                    CurrentButtonTexture = BigButtonPressed;
                    DrawTexture(CurrentButtonTexture, 75, 480, WHITE);
                    DrawText("Inventory", 155, 500, 24, WHITE);
                    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
                        CurrentMenuOptions = INVENTORY;
                    }
                } else {
                    CurrentButtonTexture = BigButtonUnpressed;
                    DrawTexture(CurrentButtonTexture, 75, 480, WHITE);
                    DrawText("Inventory", 155, 495, 24, WHITE);
                }
                // Right-Middle Big Button!
                if(mouseX >= 105 && mouseX <= 334 && mouseY >= 550 && mouseY <= 598){
                    CurrentButtonTexture = BigButtonPressed;
                    DrawTexture(CurrentButtonTexture, 75, 545, WHITE);
                    DrawText("Quests", 175, 565, 24, WHITE);
                } else {
                    CurrentButtonTexture = BigButtonUnpressed;
                    DrawTexture(CurrentButtonTexture, 75, 545, WHITE);
                    DrawText("Quests", 175, 560, 24, WHITE);
                }
                // Right-Bottom Big Button!
                if(mouseX >= 105 && mouseX <= 334 && mouseY >= 615 && mouseY <= 662){
                    CurrentButtonTexture = BigButtonPressed;
                    DrawTexture(CurrentButtonTexture, 75, 610, WHITE);
                    DrawText("Skills", 185, 630, 24, WHITE);
                } else {
                    CurrentButtonTexture = BigButtonUnpressed;
                    DrawTexture(CurrentButtonTexture, 75, 610, WHITE);
                    DrawText("Skills", 185, 625, 24, WHITE);
                }
                
                // Left-Top Big Button!
                if(mouseX >= 960 && mouseX <= 1185 && mouseY >= 485 && mouseY <= 532){
                    CurrentButtonTexture = BigButtonPressed;
                    DrawTexture(CurrentButtonTexture, 930, 480, WHITE);
                    DrawText("Equipped", 1019, 500, 24, WHITE);
                } else {
                    CurrentButtonTexture = BigButtonUnpressed;
                    DrawTexture(CurrentButtonTexture, 930, 480, WHITE);
                    DrawText("Equipped", 1019, 495, 24, WHITE);
                }
                // Left-Middle Big Button!
                if(mouseX >= 960 && mouseX <= 1185 && mouseY >= 550 && mouseY <= 598){
                    CurrentButtonTexture = BigButtonPressed;
                    DrawTexture(CurrentButtonTexture, 930, 545, WHITE);
                    DrawText("Settings", 1020, 565, 24, WHITE);
                } else {
                    CurrentButtonTexture = BigButtonUnpressed;
                    DrawTexture(CurrentButtonTexture, 930, 545, WHITE);
                    DrawText("Settings", 1020, 560, 24, WHITE);
                }
                // Left-Bottom Big Button!
                if(mouseX >= 960 && mouseX <= 1185 && mouseY >= 615 && mouseY <= 662){
                    CurrentButtonTexture = BigButtonPressed;
                    DrawTexture(CurrentButtonTexture, 930, 610, WHITE);
                    DrawText("Other", 1038, 630, 24, WHITE);
                    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
                        CurrentMenuOptions = OTHER;
                    }
                } else {
                    CurrentButtonTexture = BigButtonUnpressed;
                    DrawTexture(CurrentButtonTexture, 930, 610, WHITE);
                    DrawText("Other", 1038, 625, 24, WHITE);
                }

                if(CurrentMenuOptions == INVENTORY){

                } else if (CurrentMenuOptions == OTHER){
                    // Display mouse and player coordinates to the screen.
                    ostringstream ss;
                    string mouseValues;
                    ss << "Mouse X Value is: " << mouseX << endl;
                    ss << "Mouse Y Value is: " << mouseY << endl;
                    ss << "Player X is: " << PlayerX << endl;
                    ss << "Player Y is: " << PlayerY << endl;
                    mouseValues = ss.str();
                    DrawText(mouseValues.c_str(), 550, ScreenHeight-200, 20, WHITE);
                }

                // current date and time on the current system
                time_t now = time(0);
                // convert now to string form
                char* date_time = ctime(&now);
                string dateStr;
                stringstream dateSS;
                dateSS << date_time;
                dateStr = dateSS.str();
                DrawText(dateStr.c_str(), 1031, ScreenHeight-30, 18, WHITE);
                break;
        }
 
        EndDrawing();
        //----------------------------------------------------------------------------------
        // END DRAWING
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    
    // Unload Spritesheets
    UnloadTexture(HumanSpriteSheet);
    UnloadTexture(CurrentSpriteSheet);

    // Unload map images
    UnloadTexture(VillageMain);
    UnloadTexture(VillageNorth);
    UnloadTexture(VillageSouth);
    UnloadTexture(VillageEast);
    UnloadTexture(VillageWest);
    UnloadTexture(CurrentMap);

    // Unload GUI content
    UnloadTexture(PreGameBackground);
    UnloadTexture(InGameBackground);
    UnloadTexture(BigButtonPressed);
    UnloadTexture(BigButtonUnpressed);

    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}