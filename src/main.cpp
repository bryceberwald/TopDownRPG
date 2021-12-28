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
    Texture2D Map1 = LoadTexture("resources/maps/map1.png");

    // Create a Interchangable Texture2D for the player.
    Texture2D Character = PlayerDown;
   
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

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here

        // Declaration for the Camera2D.
        Camera2D Camera = {{ CameraX, CameraY }, { CameraX, CameraY }, 0.0f, 1.5f };
        
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

    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}