/* ************************************************************
 * PROGRAMMER  :    BRYCE BERWALD
 * PROJECT     :    SAVAGES
 * VERSION     :    0.0.1
 * DESCRIPTION :    AN RPG GAME CREATED FOR LEARNING PURPOSES
 **************************************************************/

#include "Functions.h"

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
    
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(1280, 720, "Savages | Version 0.0.1");

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)

    // Load Textures & Store into Texture2D Variables.
    Texture2D PlayerLeft = LoadTexture("resources/characters/player/player_left.png");
    Texture2D PlayerRight = LoadTexture("resources/characters/player/player_right.png");
    Texture2D PlayerUp = LoadTexture("resources/characters/player/player_up.png");
    Texture2D PlayerDown = LoadTexture("resources/characters/player/player_down.png");

    Texture2D Map1 = LoadTexture("resources/maps/map1/map1.png");
    Texture2D Map2 = LoadTexture("resources/maps/map2/map2.png");
    Texture2D Map3 = LoadTexture("resources/maps/map3/map3.png");

    // Create a Interchangable Texture2D for the player.
    Texture2D Character = PlayerDown;

    // Create a Interchangable Texture2D for the map.
    Texture2D CurrentMap = Map1;

    // Variable holds cardinal direction for the players facing direction.
    char CharacterCardinalDirection = 'S';

    // String used for creating binary locations into an integer array.
    string inputFileName = "resources/maps/map1/map1.txt";

    // Array to hold blocked locations in integer format.
    int GameMapArray[ROW_SIZE][COL_SIZE];  // 50 x 50 for map sizing. (Tiles are 32x32 in size) 1600 x 1600 overall pixel size...

    // Array is used to hold all blocked locations of type rectangle.
    Rectangle BlockedLocations[TOTAL_BLOCKED_LOCATIONS];

    // Rectangle type variables used for to and from teleportation to store 1 on map 1.
    Rectangle ToStoreLocation = {256, 640, 64, 32};
    Rectangle ExitStoreLocation = {256, 736, 64, 32};

    // Variable holds the number of blocked locations in total.
    int totalBlockedLocations = 0;

    // Variable used for reading binary numbers from input file.
    int binaryDigit;
   
    // Variables used for establishing screen dimensions.
    int ScreenWidth  = GetScreenWidth();
    int ScreenHeight = GetScreenHeight();

    // Starting position for the player. (Control player with these variables)
    int PlayerX = ScreenWidth / 2;
    int PlayerY = 200; 

    // Move camera vector with these variables.
    float CameraX = 700;
    float CameraY = 0;

    // Variable controls when to load map properties or new map properties.
    bool LoadMapProperties = true;

    // Variable used to determine which map contents to load and use.
    Boundary CurrentBounds = MAP_1;

    // Set the game FPS.
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
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
            // Display multi-dimensional array to the console for debugging purposes.
            for (int i = 0; i < ROW_SIZE; i++) {
                for (int k = 0; k < COL_SIZE; k++) {
                    cout << GameMapArray[i][k];
                }
                cout << endl;
            }
            // Set to false to only load once.
            LoadMapProperties = false;
        }

        // Declaration for the Camera2D.
        Camera2D Camera = {{ CameraX, CameraY }, { CameraX, CameraY }, 0.0f, 1.5f };

        // Variables used for draw coordinates for rectangles on blocked locations.
        int recPositionX = 0;
        int recPositionY = 0;

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

        BeginMode2D(Camera);

        ClearBackground(BLACK);

        // Draw map to the screen.
        DrawTexture(CurrentMap, 0, 0, WHITE);

        // Draw player to the screen.
        DrawTexture(Character, PlayerX, PlayerY, WHITE);

        // Draw blocked locations for the current map.
        for (int i = 0; i < ROW_SIZE; i++) {
            for (int k = 0; k < COL_SIZE; k++) {
                if(GameMapArray[i][k] == 1){
                    //DrawRectangle(recPositionX, recPositionY, 32, 32, RED);
                    BlockedLocations[totalBlockedLocations] = {(float)recPositionX, (float)recPositionY, 32, 32};
                    totalBlockedLocations++;
                }
                recPositionX += 32;
            }
            recPositionX = 0;
            recPositionY += 32;
        }

        // DrawRectangle(256, 736, 64, 32, GREEN);
        
        EndMode2D();

        // Figure out which movement key was pressed first before moving a player with that key.
        string firstMovementKeyPressed = "NONE";
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN)) {

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

        /* Check Keyboard Input from Player for horizontal movement 
           if the player pressed anything, change the new_PlayerX and new_CameraX variables */
        if (firstMovementKeyPressed == "RIGHT") {
            new_CameraX += 15;
            Character = PlayerRight;
            CharacterCardinalDirection = 'E';
            new_PlayerX += 5;
        } 
        if (firstMovementKeyPressed == "LEFT") {
            new_CameraX -= 15;
            Character = PlayerLeft;
            CharacterCardinalDirection = 'W';
            new_PlayerX -= 5;
        } 

        /* Now change if we are colliding with anything, if yes, go back to the last "safe" position, if not, go there */
        for (int i = 0; i < totalBlockedLocations; i++) {
            if (CheckCollisionRecs(BlockedLocations[i], {(float)new_PlayerX,(float)new_PlayerY,40,55})) {
                new_PlayerX = PlayerX;
                new_CameraX = CameraX;
            }
        }
        PlayerX = new_PlayerX;
        CameraX = new_CameraX;

        /* Check Keyboard Input from Player for vertical movement 
           if the player pressed anything, change the new_PlayerY and new_CameraY variables */
        if(firstMovementKeyPressed == "UP") {
            new_CameraY -= 15;
            Character = PlayerUp;
            CharacterCardinalDirection = 'N';
            new_PlayerY -= 5;
        } 
        if (firstMovementKeyPressed == "DOWN") {
            new_CameraY += 15;
            Character = PlayerDown;
            CharacterCardinalDirection = 'S';
            new_PlayerY += 5;
        }

        /* Now change if we are colliding with anything, if yes, go back to the last "safe" position, if not, go there */
        for (int i = 0; i < totalBlockedLocations; i++) {
            if (CheckCollisionRecs(BlockedLocations[i], {(float)new_PlayerX,(float)new_PlayerY,40,55})) {
                new_PlayerY = PlayerY;
                new_CameraY = CameraY;
            }
        }
        PlayerY = new_PlayerY;
        CameraY = new_CameraY;

        if(CurrentBounds == MAP_1) {

            // LEFT-SIDE Of Map1 collision detection to keep player on map.
            if(PlayerX < 0 && (PlayerY < 680 || PlayerY > 765)) {
                PlayerX = 1;
                CameraX += 15;
            }

            // LEFT-SIDE of Map1 collision detection to send player to a new map location.
            if(PlayerX < 0 && (PlayerY >= 680 && PlayerY <= 765)) {
                // Code to change map here...
                LoadMapProperties = true;
                // change input file name
                inputFileName = "resources/maps/map2/map2.txt";
                // change CurrentMap Texture to new map
                CurrentMap = Map2;
                // Change current bounds to new map.
                CurrentBounds = MAP_2;
                // change player x & y coordinates to location wanted on new map.
                PlayerX += 1575;
                CameraX += 4720;
                // Reset blocked locations to zero.
                totalBlockedLocations = 0;
            }

            // RIGHT-SIDE Of Map1 collision detection to keep player on map.
            if(PlayerX > 1570 && (PlayerY < 680 || PlayerY > 775)) {
                PlayerX = 1569;
                CameraX -= 15;
            }

            // RIGHT-SIDE of Map1 collision detection to send player to a new map location.
            if(PlayerX > 1570 && (PlayerY > 680 && PlayerY < 775)) {
                // Code to change map here...

                // End of code changes...
            }

            // TOP-SIDE - Has no exit points - Only need to ensure player is within bounds.
            if(PlayerY <= -40) {
                PlayerY = -39;
                CameraY += 15;
            }

            // BOTTOM-SIDE Of Map1 collision detection to keep player on map.
            if(PlayerY >= 1550 && (PlayerX < 730 || PlayerX > 835)) {
                PlayerY = 1549;
                CameraY -= 15;
            }

            // BOTTOM-SIDE of Map1 collission detection to send to player to a new map location.
            if(PlayerY >= 1550 && (PlayerX > 730 && PlayerX < 835)) {
                cout << "\nBOTTOM-HIT\n";
                DrawText("[EXIT LOCATION]", 500, 200, 24, WHITE);
                // Code to change map here...

                // End of code changes...
            }

            // GO TO THE STORE!!!
            // Teleport player to the store from building 1 on map 1.
            if(CheckCollisionRecs(ToStoreLocation, {(float)PlayerX, (float)PlayerY, 40, 55 })) {

                // Code to change map here...
                LoadMapProperties = true;

                // change input file name
                inputFileName = "resources/maps/map3/map3.txt";

                // change CurrentMap Texture to new map
                CurrentMap = Map3;

                // Change current bounds to new map.
                CurrentBounds = MAP_3;

                // Reset blocked locations to zero.
                totalBlockedLocations = 0;

            }

        } else if (CurrentBounds == MAP_2) {
            //LEFT
            if(PlayerX < 0) {
                PlayerX = 1;
                CameraX += 15;
            }
            // RIGHT-SIDE Of Map2 collision detection to keep player on map.
            if(PlayerX > 1570 && (PlayerY < 680 || PlayerY > 775)) {
                PlayerX = 1569;
                CameraX -= 15;
            }
            // RIGHT-SIDE of Map2 collision detection to send player to a Map1.
            if(PlayerX > 1570 && (PlayerY >= 680 && PlayerY <= 775)) {
                // Code to change map here...
                LoadMapProperties = true;
                // change input file name
                inputFileName = "resources/maps/map1/map1.txt";
                // change CurrentMap Texture to new map
                CurrentMap = Map1;
                // Change current bounds to new map.
                CurrentBounds = MAP_1;
                // change player x & y coordinates to location wanted on new map.
                PlayerX -= 1575;
                CameraX -= 4720;
                // Reset blocked locations to zero.
                totalBlockedLocations = 0;
            }
            // TOP
            if(PlayerY <= -40) {
                PlayerY = -39;
                CameraY += 15;
            }
            // BOTTOM
            if(PlayerY >= 1550) {
                PlayerY = 1549;
                CameraY -= 15;
            }
        } else if (CurrentBounds == MAP_3) {
            // Store Map Location!

            if(CheckCollisionRecs(ExitStoreLocation, {(float)PlayerX, (float)PlayerY, 40, 55 })) {
                // Code to change map here...
                LoadMapProperties = true;
                // change input file name
                inputFileName = "resources/maps/map1/map1.txt";
                // change CurrentMap Texture to new map
                CurrentMap = Map1;
                // Change current bounds to new map.
                CurrentBounds = MAP_1;
                // Reset blocked locations to zero.
                totalBlockedLocations = 0;
            }

        }

        //Draw Gaming menu template with rectangles around screen edges.
        DrawRectangle(0, 0, ScreenWidth, 50, DARKBROWN);                  // Top
        DrawRectangle(0, ScreenHeight-300, ScreenWidth, 300, DARKBROWN); // Bottom
        DrawRectangle(0, 0, 20, ScreenHeight, DARKBROWN);                 // Left
        DrawRectangle(ScreenWidth-20, 0, 20, ScreenHeight, DARKBROWN);   // Right
        
        // Display mouse and player coordinates to the screen.
        ostringstream ss;
        string mouseValues;
        int mouseX = GetMouseX();
        int mouseY = GetMouseY();
        ss << "Mouse X Value is: " << mouseX << endl;
        ss << "Mouse Y Value is: " << mouseY << endl;
        ss << "Player X is: " << PlayerX << endl;
        ss << "Player Y is: " << PlayerY << endl;
        mouseValues = ss.str();
        DrawText(mouseValues.c_str(), 5, ScreenHeight-250, 24, WHITE);
 
        EndDrawing();
        //----------------------------------------------------------------------------------
        // END DRAWING
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(PlayerLeft);
    UnloadTexture(PlayerRight);
    UnloadTexture(PlayerUp);
    UnloadTexture(PlayerDown);

    UnloadTexture(Character);
    UnloadTexture(Map1);

    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}