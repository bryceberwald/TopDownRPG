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
    
    // Variables used for establishing monitor dimensions.
    int MonitorWidth  = GetMonitorWidth(0);
    int MonitorHeight = GetMonitorHeight(0);
    
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(MonitorWidth, MonitorHeight, "Savages | Version 0.0.1");

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)

    // Load Textures & Store into Texture2D Variables.
    Texture2D PlayerLeft = LoadTexture("resources/characters/player/player_left.png");
    Texture2D PlayerRight = LoadTexture("resources/characters/player/player_right.png");
    Texture2D PlayerUp = LoadTexture("resources/characters/player/player_up.png");
    Texture2D PlayerDown = LoadTexture("resources/characters/player/player_down.png");
    Texture2D Map1 = LoadTexture("resources/maps/map1/map1.png");

    // Create a Interchangable Texture2D for the player.
    Texture2D Character = PlayerDown;

    // String used for creating binary locations into an integer array.
    string inputFileName = "resources/maps/map1/map1.txt";

    // Array to hold blocked locations in integer format.
    int GameMapArray[ROW_SIZE][COL_SIZE];  // 50 x 50 for map sizing. (Tiles are 32x32 in size)

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

    // Used to check if player is walking.
    bool walking = true;

    // Set the game FPS.
    SetTargetFPS(60);

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

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here

        // Declaration for the Camera2D.
        Camera2D Camera = {{ CameraX, CameraY }, { CameraX, CameraY }, 0.0f, 1.5f };

        // Variables used for draw coordinates for rectangles on blocked locations.
        int recPositionX = 0;
        int recPositionY = 0;
        
        // NOTE: Texture is scaled twice its size, so it sould be considered on scrolling
        
        //----------------------------------------------------------------------------------
        // BEGIN DRAWING
        //----------------------------------------------------------------------------------
        BeginDrawing();

        BeginMode2D(Camera);

        ClearBackground(BLACK);

        // Draw map to the screen.
        DrawTexture(Map1, 0, 0, WHITE);

        // Draw player to the screen.
        DrawTexture(Character, PlayerX, PlayerY, WHITE);

        // Draw blocked locations for map #1.
        for (int i = 0; i < ROW_SIZE; i++) {
            for (int k = 0; k < COL_SIZE; k++) {
                if(GameMapArray[i][k] == 1){
                    DrawRectangle(recPositionX, recPositionY, 32, 32, RED);
                }
                recPositionX += 32;
            }
            recPositionX = 0;
            recPositionY += 32;
        }

        // Check if player is currently walking or not.
        if (walking){
            if (IsKeyDown(KEY_RIGHT)) {
                CameraX += 15;
                Character = PlayerRight;
                PlayerX += 5;
            } else if (IsKeyDown(KEY_LEFT)) {
                CameraX -= 15;
                Character = PlayerLeft;
                PlayerX -= 5;
            } else if(IsKeyDown(KEY_UP)) {
                CameraY -= 15;
                Character = PlayerUp;
                PlayerY -= 5;
            } else if (IsKeyDown(KEY_DOWN)) {
                CameraY += 15;
                Character = PlayerDown;
                PlayerY += 5;
            }
        } 
        
        EndMode2D();

        // Draw Gaming menu template with rectangles around screen edges.
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