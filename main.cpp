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

    // End game
    bool isEnd {false};
    bool isRestart {false};

    // Score
    int scores {0};

    SetConfigFlags(FLAG_MSAA_4X_HINT);  // NOTE: Try to enable MSAA 4X

    InitWindow(screenWidth, screenHeight, "Space Invader");

    InitAudioDevice();                  // Initialize audio device

    // Player
    Rectangle player {static_cast<float>(screenWidth/2) - 100, static_cast<float>(screenHeight/2) - 100, 25, 25};
    float playerSpeed {7.0f};

    Vector2 movementInput {};

    int moveHorizontal {};
    int moveVertical {};
    bool isShoot {false};

    // Bullet
    const int maxBulletList {20};
    std::array<Vector2, maxBulletList> bulletList {};
    int reload {};
    int reloadTime {};
    int bulletSpeed {20};
    // second * FPS
    const int reloadSecond = 3 * 60;

    //asteroid
    const int maxAsteroid {200};
    std::array<Vector2, maxAsteroid> asteroidList {};
    int reloadAsteroid {};
    int asteroidSpeed {5};
    // 60 mean around 1 second
    const int spawnSecond {30};
    int spawnTime {spawnSecond};

    // Music
    Music music = LoadMusicStream("res/audio/mini1111.xm");
    music.looping = true;

    PlayMusicStream(music);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        UpdateMusicStream(music);

    if (!isEnd) {

        // Score
        scores++;

        moveHorizontal = (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) - (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A));
        moveVertical = (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) - (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) ;

        // Get player Input
        movementInput.x = moveHorizontal;
        movementInput.y = moveVertical;
        isShoot = IsKeyPressed(KEY_E) || IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

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
        if (isShoot && reload < maxBulletList)
        {
            bulletList[reload] = {player.x + player.width / 2, player.y + player.height / 2};
            reload++;
        }

        // Caculate bullet coordinates
        for (int iii {0}; iii < static_cast<int>(bulletList.size()); iii++)
        {
            if (bulletList[iii].x == 0 && bulletList[iii].y == 0) continue;
            bulletList[iii].x += bulletSpeed;
            if (bulletList[iii].x > static_cast<float>(GetScreenWidth())) bulletList[iii] = {0, 0};
        }

        // Reload the index of bulletList to reuse
        if (reloadTime == reloadSecond)
        {
            reload = 0;
            reloadTime = 0;
        }
        else reloadTime++;

        // asteroid
        // instance asteroid
        if (spawnTime == 0 && reloadAsteroid <= maxAsteroid)
        {
            spawnTime = spawnSecond;
            for (int iii {0}; iii < 5; iii++)
            {
                asteroidList[reloadAsteroid] = {
                    static_cast<float>(GetRandomValue(screenWidth, screenWidth + 100)),
                    static_cast<float>(GetRandomValue(0, screenHeight - 50))
                };
                reloadAsteroid++;
            }
        } else {
            spawnTime--;
        }
        // Caculate asteroid coordinates
        for (int iii {0}; iii < maxAsteroid; iii++)
        {
            if (asteroidList[iii].x == 0) continue;
            asteroidList[iii].x -= asteroidSpeed;
            if (asteroidList[iii].x < 0) asteroidList[iii] = {0, 0};
        }

        // Reload the index of asteroidList to reuse
        if (reloadAsteroid >= maxAsteroid)
        {
            reloadAsteroid = 0;
        }

        // Check collision of player with asteroids
        for (int iii {0}; iii < maxAsteroid; iii++)
        {
            if (CheckCollisionRecs(player, {asteroidList[iii].x, asteroidList[iii].y, 20, 20}) && asteroidList[iii].x != 0)
            {
                isEnd = true;
            }
        }

        // Check collision of bullet with asteroids
        for (int iii {0}; iii < maxBulletList; iii++)
        {
            for (int uuu {0}; uuu < maxAsteroid; uuu++)
            {
                if (CheckCollisionRecs({bulletList[iii].x, bulletList[iii].y, 20, 10}, {asteroidList[uuu].x, asteroidList[uuu].y, 20, 20}))
                {
                    bulletList[iii] = {0, 0};
                    asteroidList[uuu] = {0, 0};
                }
            }
        }} // isEnd scene
    else {
        isRestart = IsKeyPressed(KEY_R);
        if (isRestart)
        {
            // Clean bullets and asteroids
            for (int iii {0}; iii < maxBulletList; iii++)
            {
                bulletList[iii] = {0, 0};
            }
            for (int iii {0}; iii < maxAsteroid; iii++)
            {
                asteroidList[iii] = {0, 0};
            }
            // Reset player position
            player = {static_cast<float>(screenWidth/2) - 100, static_cast<float>(screenHeight/2) - 100, 25, 25};

            isRestart = false;
            isEnd = false;
            scores = 0;
        }
    }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);

        if (!isEnd) {

            DrawRectangle(player.x, player.y, player.width, player.height, WHITE);

            // Score
            DrawText(TextFormat("Scores: %i", scores), 400, 50, 20, RAYWHITE);

            // Draw bullet
            for (int iii {0}; iii < static_cast<int>(bulletList.size()); iii++)
            {
                if (!(bulletList[iii].x == 0 && bulletList[iii].y == 0))
                DrawRectangle(bulletList[iii].x, bulletList[iii].y, 20, 10, RED);
            }
            // Draw asteroid
            for (int iii {0}; iii < maxAsteroid; iii++)
            {
                if (asteroidList[iii].x == 0) continue;
                DrawRectangle(asteroidList[iii].x, asteroidList[iii].y, 20, 20, YELLOW);
            }
        } // isEndScene

            if (isEnd)
            {
                DrawText("You lose!", 250, 250, 100, RED);
                DrawText("Press R to play again", 200, 350, 50, WHITE);
                DrawText(TextFormat("Your scores are: %i", scores), 250, 450, 40, WHITE);
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
