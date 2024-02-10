#include "raylib.h"

void drawTetromino(const Color currentColor, const int startOffsetX, const int startOffsetY, const int tetrominoStartX, const int tetrominoStartY, const int *tetromino);
void ResetLines(int startLineY);
void UpdateScore();
void difficultieChange();
void DeleteLines(Sound clearLine);
