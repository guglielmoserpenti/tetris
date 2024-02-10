#include "GameFunctions.h"
#include "Tetris.h"
#include "tetrominos.h"
#include "stage.h"
#include "raylib.h"
#include "string.h"

extern int score;
extern int consecutiveLines;
extern int speedMultiplaier;

void drawTetromino(const Color currentColor, const int startOffsetX, const int startOffsetY, const int tetrominoStartX, const int tetrominoStartY, const int *tetromino)
{
    for(int y = 0; y < TETROMINO_SIZE; y++)
    {
        for(int x = 0; x < TETROMINO_SIZE; x++)
        {
            const int offset = y * TETROMINO_SIZE + x;

            if(tetromino[offset] == 1)
            {
                DrawRectangle((x + tetrominoStartX) * TILE_SIZE + startOffsetX, (y + tetrominoStartY) * TILE_SIZE + startOffsetY, TILE_SIZE, TILE_SIZE, currentColor);
            }

        }
    }
}

void ResetLines(int startLineY)
{
    for (int y = startLineY; y >= 0; y--)
    {
        for (int x = 1; x < STAGE_WIDTH - 1; x++)
        {
            const int offset = y * STAGE_WIDTH + x;
            const int offset_below = (y+1) * STAGE_WIDTH + x;

            if (stage[offset_below] == 0 && stage[offset] > 0)
            {
                stage[offset_below] = stage[offset];
                stage[offset] = 0;
            }
        }
    }   
}

void UpdateScore()
{
    switch (consecutiveLines)
    {
    case 1:
        score += 100;
        break;
    case 2:
        score += 200;
        break;
    case 3:
        score += 600;
        break;
    case 4:
        score += 800;
    default:
        
        break;
    }
}

 void difficultieChange()
 {
     switch (score/1000)
     {
        case 0:
         speedMultiplaier = 1;
         break;
        case 1: 
         speedMultiplaier = 2;
         break;
        case 2: 
         speedMultiplaier = 3;
         break;
        case 3:
        speedMultiplaier = 4;
        break;
        case 4:
        speedMultiplaier = 5;
        break;
        case 5:
        speedMultiplaier = 6;
        break;
        case 6:
        speedMultiplaier = 7;
        break;
        case 7:
        speedMultiplaier = 8;
        break;
     default:
         break;
     }
 }
void DeleteLines(Sound clearLine)
{
    
    for (int y = 0; y < STAGE_HEIGHT - 1; y++)
    {
        int checkLine = 1;

        for (int x = 1; x < STAGE_WIDTH - 1; x++)
        {
            const int offset = y * STAGE_WIDTH + x;

            if (stage[offset] == 0)
            {
                checkLine = 0;
                consecutiveLines = 0;
                break;
            }
        }

        if (checkLine)
        {
            const int offset = y * STAGE_WIDTH + 1;
            memset(stage + offset, 0, (STAGE_WIDTH - 2) * sizeof(int));
            
            ResetLines(y);
            
            consecutiveLines++;     
            
            PlaySound(clearLine);  
             
        }
        
        UpdateScore();
        difficultieChange();
    }
    
}

