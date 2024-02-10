#include "raylib.h"
#include <time.h>
#include <string.h>
#include "Tetris.h"
#include <stdio.h>
#include "stage.h"
#include "tetrominos.h"
#include "GameFunctions.h"


const Color bgColor = {55, 62, 64,255};
Color lineColor = {119, 135, 139, 125};


const Color colorTypes[8] =
{
    {234, 20, 28,255},
    {254, 251, 52,255},
    {83, 218, 63,255},
    {1, 237, 250,255},
    {221, 10, 178,255},
    {46, 46, 132,255},
    {120, 37, 111,255},
    {255, 145, 12,255},
};


int score = 0;
int consecutiveLines = 0;
int speedMultiplaier = 1;
int stopDrawing = 0;





int main(int argc, char** argv, char** environ)
{
    const int windowWidth = 600; 
    const int windowHeight = 700; 

    const int startOffsetX = (windowWidth / 2) - ((STAGE_WIDTH * TILE_SIZE) / 2);
    const int startOffsetY = (windowHeight / 2) - ((STAGE_HEIGHT * TILE_SIZE) / 2);

    const int tetrominoStartX = STAGE_WIDTH / 2;
    const int tetrominoStartY = 0;

    int currentTetrominoX = tetrominoStartX;
    int currentTetrominoY = tetrominoStartY;
    
    


    time_t unixTime;

    time(&unixTime);

    SetRandomSeed(unixTime);

    int currentTetrominoType = GetRandomValue(0, 6);
    int currentRotation = 0;

    const float moveTetrominoDownTimer = 1.f;
    float timeToMoveTetrominoDown = moveTetrominoDownTimer;
    int currentColor = GetRandomValue(0, 7);

    TraceLog(LOG_INFO, "Number of arguments : %d", argc);

    for(int i = 0; i < argc; i++)
    {
        TraceLog(LOG_INFO, "Argument : %d = %s", i, argv[i]);
    }

    while(*environ != 0)
    {
        TraceLog(LOG_INFO, "Argument : %s", *environ);
        environ++;
    }

    InitWindow(windowWidth, windowHeight, "Tetris - by GS");
    InitAudioDevice();
    Music soundtrack = LoadMusicStream("tetris_soundtrack.mp3");
    PlayMusicStream(soundtrack);
    SetMusicVolume(soundtrack, 0.01f);
    Sound clearLine = LoadSound("tetris-line-clear-sound.mp3");
    SetSoundVolume(clearLine, 0.01f);
    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        UpdateMusicStream(soundtrack);
        timeToMoveTetrominoDown -= GetFrameTime() * speedMultiplaier;

         if (IsKeyPressed(KEY_SPACE))
        {
            const int lastRotation = currentRotation;
            
            currentRotation++;

            if (currentRotation > 3)
            {
                currentRotation = 0;
            }

            if (CheckCollision(currentTetrominoX,currentTetrominoY,tetrominoTypes[currentTetrominoType][currentRotation]))
            {
                currentRotation = lastRotation;
            }
        }

        if (IsKeyPressed(KEY_RIGHT))
        {
            // No need to check overflow, wall is your protector
            if (!CheckCollision(currentTetrominoX+1,currentTetrominoY,tetrominoTypes[currentTetrominoType][currentRotation]))
            {
                currentTetrominoX++;
            }
        }
        if (IsKeyPressed(KEY_LEFT))
        {
            // No need to check overflow, wall is your protector
            if (!CheckCollision(currentTetrominoX-1,currentTetrominoY,tetrominoTypes[currentTetrominoType][currentRotation]))
            {
                currentTetrominoX--;
            }
        }

        if(timeToMoveTetrominoDown <= 0 || IsKeyPressed(KEY_DOWN))
        {            
            if(!CheckCollision(currentTetrominoX,currentTetrominoY+1,tetrominoTypes[currentTetrominoType][currentRotation]))
            {
                currentTetrominoY++;
                timeToMoveTetrominoDown = moveTetrominoDownTimer;
            }

            else
            {
                

                for(int y = 0; y < TETROMINO_SIZE; y++)
                {
                    for(int x = 0; x < TETROMINO_SIZE; x++)
                    {
                        const int offset = y * TETROMINO_SIZE + x;

                        const int *tetromino = tetrominoTypes[currentTetrominoType][currentRotation];

                        if(tetromino[offset] == 1)
                        {
                            const int offset_stage = (y + currentTetrominoY) * STAGE_WIDTH + (x + currentTetrominoX);

                            stage[offset_stage] = currentColor + 1;
                        }
                        
                    }
                }

                DeleteLines(clearLine);
            
                currentTetrominoX = tetrominoStartX;
                currentTetrominoY = tetrominoStartY;

                currentTetrominoType = GetRandomValue(0, 6);
                currentRotation = 0;
                currentColor = GetRandomValue(0, 7);
            }
            
            
        }
        for (int i = 1; i < 10; i++)
        {
            if(stage[i] == 1)
            {
               
                stopDrawing= 1;
                
            }
        }
        
        
        
        
        BeginDrawing();
        ClearBackground(bgColor);

        char scoreText[20];
        sprintf(scoreText, "Score: %d", score);
        DrawText(scoreText, windowWidth - 150, 10, 20, BLACK);


        for(int y = 0; y < STAGE_HEIGHT; y++)
        {
            for(int x = 0; x < STAGE_WIDTH; x++)
            {
                const int offset = y * STAGE_WIDTH + x;
                const int color = stage[offset];

                if(stage[offset] != 0)
                {
                    DrawRectangle(x * TILE_SIZE + startOffsetX, y * TILE_SIZE + startOffsetY, TILE_SIZE, TILE_SIZE, BLACK);
                }

                DrawRectangleLines(x * TILE_SIZE + startOffsetX, y * TILE_SIZE + startOffsetY, TILE_SIZE, TILE_SIZE, lineColor);
            }
        }
        if(!stopDrawing)
        {

            drawTetromino(colorTypes[currentColor],startOffsetX, startOffsetY, currentTetrominoX, currentTetrominoY, tetrominoTypes[currentTetrominoType][currentRotation]);
            
        }
        else
        {
            ClearBackground(BLACK);
            lineColor = BLACK;
            sprintf(scoreText, "Score: %d", score);
            DrawText(scoreText, ((windowWidth - startOffsetX)*0.5), (windowHeight - startOffsetY)*0.5, 20, WHITE);
            DrawText("GAME OVER", (windowWidth - startOffsetX)*0.5,((windowHeight - startOffsetY)*0.5)-50, 20, WHITE);
        }

        
        
        EndDrawing();
    }
    UnloadMusicStream(soundtrack);
    UnloadSound(clearLine);
    CloseAudioDevice();
    CloseWindow();


    return 0;
}