#include <cmath>
#include "raylib.h"
#include "raymath.h"
#include <array>
#include <cassert>

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
    bool isShoot {false};

    // Bullet
    std::array<Vector2, 10> bulletList {};
    int reload {};
    int bulletSpeed {20};

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
        isShoot = IsKeyDown(KEY_E);

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
        // Clamp in screen
        player.x = Clamp(player.x, 0, static_cast<float>(GetScreenWidth() - player.width));
        player.y = Clamp(player.y, 0, static_cast<float>(GetScreenHeight() - player.height));

        // instance the bullet
        if (isShoot && reload < 10)
        {
            bulletList[reload] = {player.x + player.width / 2, player.y + player.height / 2};
        }

        // Caculate bullet coordinates
        for (int iii {0}; iii < static_cast<int>(bulletList.size()); iii++)
        {
//             if (bulletList[iii].x == 0 && bulletList[iii].y == 0) continue;
            bulletList[iii].x += bulletSpeed;
            if (bulletList[iii].x > static_cast<float>(GetScreenWidth())) bulletList[iii] = {0, 0};
        }

        // Reload the index of bulletList to reuse after 6 second
        if (reload == 360) reload = 0;
        else reload++;

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawRectangle(player.x, player.y, player.width, player.height, BLUE);

            // TODO: make new bullet and add it to bullet group
            // FIXME: I have no idea what the is the bullet doing
            for (int iii {0}; iii < static_cast<int>(bulletList.size()); iii++)
            {
                if ((bulletList[iii].x == 0 && bulletList[iii].y == 0))
                DrawRectangle(bulletList[iii].x, bulletList[iii].y, 100, 50, RED);
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
