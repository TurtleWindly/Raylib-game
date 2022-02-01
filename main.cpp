#include <cmath>
#include "raylib.h"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 960;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Some Game");

    Rectangle player {static_cast<float>(screenWidth/2) - 100, static_cast<float>(screenHeight/2) - 100, 50, 50};
    float playerSpeed {7.0f};

    Vector2 movementInput {};

    int moveHorizontal {};
    int moveVertical {};
    bool shoot {false};

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        moveHorizontal = (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) - (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A));
        moveVertical = (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) - (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) ;

        // Get player Input
        movementInput.x = moveHorizontal;
        movementInput.y = moveVertical;
        shoot = IsMouseButtonDown(MOUSE_BUTTON_LEFT);

        // Normalize the Input Vector2
        if (abs(movementInput.x) + abs(movementInput.y) == 2)
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

    CloseWindow();                  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}