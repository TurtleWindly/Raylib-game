#include <cmath>
#include "raylib.h"
#include "raymath.h"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 960;
    const int screenHeight = 720;

    SetConfigFlags(FLAG_MSAA_4X_HINT);  // NOTE: Try to enable MSAA 4X

    InitWindow(screenWidth, screenHeight, "Some Game");

    InitAudioDevice();                  // Initialize audio device

    Rectangle player {static_cast<float>(screenWidth/2) - 100, static_cast<float>(screenHeight/2) - 100, 50, 50};
    float playerSpeed {7.0f};

    Vector2 movementInput {};

    int moveHorizontal {};
    int moveVertical {};
    bool shoot {false};

    Music music = LoadMusicStream("res/audio/mini1111.xm");
    music.looping = false;

    PlayMusicStream(music);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        UpdateMusicStream(music);

        moveHorizontal = (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) - (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A));
        moveVertical = (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) - (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) ;

        // Get player Input
        movementInput.x = moveHorizontal;
        movementInput.y = moveVertical;
        shoot = IsMouseButtonDown(MOUSE_BUTTON_LEFT);

        // Normalize the Input Vector2
        if (std::abs(movementInput.x) + std::abs(movementInput.y) == 2)
        {
            player.x += movementInput.x * playerSpeed * (3.0f/4.0f);
            player.y += movementInput.y * playerSpeed * (3.0f/4.0f);
        } else {
            // Caculate player position
            player.x += movementInput.x * playerSpeed;
            player.y += movementInput.y * playerSpeed;
        }
        


        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawRectangle(player.x, player.y, player.width, player.height, BLUE);

            // TODO: Create bullet that shoot to mouse direction
            if (shoot)
            {
                DrawCircle(player.x, player.y, 5.0f, RED);
            }


            DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadMusicStream(music);          // Unload music stream buffers from RAM

    CloseAudioDevice();     // Close audio device (music streaming is automatically stopped)

    CloseWindow();                  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}