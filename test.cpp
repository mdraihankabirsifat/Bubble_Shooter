#include "iGraphics.h"
#include "iFont.h"
#include "iSound.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <cstdio>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

#define ROWS 17
#define COLS 20
#define GAME_OVER_ROW 15
#define BUBBLE_RADIUS 20
#define ACTUAL_HEIGHT 800
#define GAME_WINDOW_WIDTH 800
#define GAME_WINDOW_HEIGHT 750

int Rows = 10;
int Columns = 20;
int grid[ROWS][COLS] = {0};
int shootingBallColorNow, shootingBallColorNext;
double shooter_x = GAME_WINDOW_WIDTH / 2;
double shooter_y = 80;
double ball_x, ball_y;
double velocity = 20;
double angle = 0;
double mouseX = GAME_WINDOW_WIDTH / 2;
double mouseY = GAME_WINDOW_HEIGHT / 2;
double rowHeight = BUBBLE_RADIUS * sqrt(3);
double vx = 0;
double vy = 0;
bool isBallMoving = false;
int ballBounceCount = 0;
bool swapKeyPressed = false;
int Score = 0;
int ScoreIncrement = 0;
int ScoreIncrementingInstant = 0;
int ShowIncrement = false;
int fallingBubbleCount = 0;
int PoppedBubbleColor;
double X, Y;
int PoppingInstant;
int ShowPopping = 0;
int currentLevel = 1;
int unlockedLevels = 1;
int currentLevelData[12][20];
int levels[10][12][20] = {
    // level 1
    {
        {1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2},  // Hidden row 1 (easiest - big groups)
        {2, 2, 2, 2, 2, 1, 1, 1, 0, 1, 2, 2, 2, 2, 2, 1, 1, 1, 0, 1},  // Hidden row 2 (easiest - big groups)
        {1, 1, 1, 2, 2, 2, 1, 1, 0, 0, 2, 2, 1, 1, 1, 2, 2, 0, 1, 1},  // Hidden row 3 (medium groups)
        {2, 2, 1, 1, 1, 2, 2, 0, 0, 1, 2, 2, 1, 1, 1, 2, 0, 1, 1, 2},  // Hidden row 4 (medium groups)
        {1, 1, 2, 2, 1, 1, 2, 2, 1, 0, 2, 2, 1, 1, 2, 2, 0, 1, 2, 2},  // Hidden row 5 (pairs)
        {2, 1, 1, 2, 2, 1, 1, 2, 2, 1, 0, 2, 2, 1, 1, 2, 2, 1, 1, 2},  // Hidden row 6 (pairs)
        {1, 2, 1, 2, 1, 0, 0, 2, 1, 2, 1, 0, 1, 2, 1, 2, 1, 2, 0, 2},  // Hidden row 7 (alternating)
        {2, 1, 2, 1, 2, 0, 0, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1},  // Hidden row 8 (alternating)
        {1, 1, 1, 2, 2, 1, 1, 1, 0, 2, 1, 1, 1, 2, 2, 1, 1, 1, 0, 2},  // Initially visible row 1
        {2, 2, 1, 1, 2, 2, 0, 1, 2, 0, 1, 1, 2, 2, 1, 1, 2, 2, 1, 1},  // Initially visible row 2
        {1, 2, 2, 1, 1, 2, 2, 1, 1, 2, 0, 1, 1, 2, 2, 1, 1, 0, 2, 1},  // Initially visible row 3
        {2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1}}, // Initially visible row 4
    // Level 2 - Easy: 3 colors, more random grouped pattern
    {
        {1, 1, 1, 2, 2, 2, 3, 3, 3, 1, 1, 0, 2, 2, 2, 3, 3, 3, 1, 1},  // Hidden row 1 (big clusters)
        {2, 0, 2, 3, 3, 3, 3, 1, 3, 2, 2, 0, 2, 3, 2, 1, 1, 1, 0, 2},  // Hidden row 2 (offset big clusters)
        {1, 1, 2, 0, 2, 2, 3, 1, 2, 3, 0, 0, 1, 1, 2, 2, 3, 0, 0, 1},  // Hidden row 3 (medium clusters)
        {3, 3, 1, 0, 0, 2, 3, 3, 1, 0, 0, 2, 3, 3, 1, 0, 2, 2, 3, 3},  // Hidden row 4 (medium clusters)
        {1, 2, 1, 0, 1, 0, 2, 3, 1, 0, 1, 1, 1, 1, 2, 0, 1, 2, 1, 2},  // Hidden row 5 (mixed pairs)
        {2, 3, 2, 1, 1, 2, 3, 1, 2, 1, 1, 1, 1, 1, 0, 3, 2, 3, 0, 3},  // Hidden row 6 (mixed pairs)
        {1, 3, 2, 1, 3, 1, 1, 1, 1, 1, 3, 2, 1, 0, 2, 1, 3, 2, 0, 3},  // Hidden row 7 (alternating triplets)
        {2, 1, 3, 0, 1, 3, 2, 1, 1, 2, 1, 3, 2, 1, 3, 2, 1, 3, 2, 1},  // Hidden row 8 (alternating triplets)
        {3, 3, 1, 0, 2, 2, 3, 3, 1, 1, 1, 2, 3, 3, 1, 1, 2, 2, 3, 3},  // Visible row 1
        {1, 1, 0, 0, 3, 3, 1, 1, 2, 2, 1, 3, 1, 1, 2, 0, 0, 3, 1, 1},  // Visible row 2
        {2, 2, 0, 3, 1, 1, 2, 2, 3, 3, 1, 1, 2, 2, 3, 3, 0, 0, 2, 2},  // Visible row 3
        {3, 3, 1, 1, 2, 2, 3, 3, 1, 1, 2, 2, 3, 3, 1, 1, 2, 2, 3, 3}}, // Initially visible row 4
    // Level 3 - Easy: 3 colors, random scattered pattern
    {
        {1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 1, 1},
        {1, 0, 1, 1, 1, 1, 2, 2, 0, 2, 0, 2, 0, 3, 3, 3, 0, 0, 1, 1},
        {2, 2, 0, 2, 2, 2, 3, 3, 0, 3, 0, 3, 0, 1, 1, 1, 1, 0, 0, 2},
        {2, 2, 2, 0, 2, 2, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 2, 2},
        {3, 3, 3, 3, 3, 3, 1, 1, 0, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3},
        {3, 3, 3, 0, 3, 3, 1, 1, 1, 1, 1, 1, 2, 2, 0, 0, 2, 2, 3, 3},
        {1, 1, 1, 0, 2, 0, 3, 0, 3, 1, 1, 0, 2, 2, 0, 0, 3, 3, 1, 1},
        {2, 2, 2, 3, 3, 3, 1, 0, 1, 2, 2, 0, 0, 3, 3, 1, 1, 1, 2, 2},
        {1, 0, 1, 1, 2, 2, 2, 3, 3, 3, 1, 1, 0, 2, 2, 2, 3, 3, 0, 1},
        {2, 2, 0, 2, 3, 0, 3, 1, 0, 1, 2, 2, 2, 0, 0, 3, 1, 1, 0, 2},
        {3, 3, 0, 0, 1, 0, 1, 2, 2, 2, 3, 3, 3, 0, 0, 1, 2, 2, 0, 3},
        {1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 1, 1, 1, 2, 2, 2, 3, 3, 3, 1}}, // Initially visible row 4
    // Level 4 - Easy-Medium: 4 colors, random pattern
    {
        {1, 1, 1, 2, 2, 2, 3, 1, 0, 0, 4, 4, 1, 1, 2, 2, 3, 3, 4, 4},  // Hidden row 1
        {2, 0, 0, 3, 3, 3, 4, 1, 4, 1, 4, 1, 2, 2, 3, 3, 4, 4, 0, 0},  // Hidden row 2
        {3, 4, 0, 0, 2, 2, 1, 4, 0, 1, 0, 1, 2, 0, 0, 2, 1, 1, 4, 2},  // Hidden row 3
        {4, 4, 2, 0, 3, 3, 1, 4, 2, 2, 4, 1, 1, 1, 1, 2, 2, 0, 0, 3},  // Hidden row 4
        {1, 1, 3, 3, 4, 4, 4, 2, 0, 3, 1, 1, 4, 4, 0, 2, 3, 3, 1, 1},  // Hidden row 5
        {2, 2, 4, 0, 1, 4, 3, 3, 4, 4, 2, 2, 1, 1, 3, 3, 4, 0, 2, 2},  // Hidden row 6
        {1, 1, 2, 2, 4, 3, 4, 4, 3, 3, 2, 0, 3, 3, 4, 4, 1, 1, 2, 2},  // Hidden row 7
        {3, 3, 4, 0, 1, 0, 3, 2, 0, 3, 3, 3, 1, 1, 0, 2, 3, 2, 2, 4},  // Hidden row 8
        {1, 4, 1, 2, 2, 0, 3, 4, 0, 3, 1, 0, 0, 3, 0, 0, 4, 2, 1, 4},  // Visible row 1
        {2, 2, 3, 3, 4, 4, 1, 0, 2, 2, 3, 0, 0, 4, 1, 1, 0, 2, 4, 1},  // Visible row 2
        {4, 4, 0, 0, 4, 2, 3, 3, 1, 1, 3, 4, 2, 2, 3, 3, 0, 1, 4, 0},  // Visible row 3
        {3, 3, 2, 4, 4, 0, 0, 1, 3, 3, 2, 2, 4, 4, 1, 1, 3, 0, 2, 2}}, // Initially visible row 4
    // Level 5 - Medium: 4 colors, longer V-shape pattern
    {
        {1, 1, 2, 2, 3, 3, 4, 4, 1, 1, 2, 2, 3, 3, 4, 4, 1, 1, 2, 2},  // Hidden row 1
        {3, 0, 4, 4, 1, 1, 2, 2, 2, 0, 4, 4, 1, 1, 2, 2, 3, 3, 4, 4},  // Hidden row 2
        {2, 2, 0, 1, 4, 4, 3, 3, 2, 2, 1, 1, 0, 0, 3, 3, 2, 2, 1, 1},  // Hidden row 3
        {4, 4, 3, 0, 2, 2, 1, 1, 0, 0, 3, 3, 2, 0, 1, 1, 4, 4, 3, 3},  // Hidden row 4
        {1, 1, 2, 2, 1, 1, 3, 3, 0, 1, 2, 2, 3, 3, 1, 1, 2, 2, 4, 4},  // Hidden row 5
        {3, 3, 1, 0, 0, 4, 2, 2, 1, 1, 3, 3, 2, 2, 0, 4, 1, 1, 3, 3},  // Hidden row 6
        {2, 2, 3, 0, 0, 1, 4, 4, 2, 0, 0, 3, 1, 1, 0, 0, 2, 2, 3, 0},  // Hidden row 7
        {4, 4, 1, 1, 2, 2, 3, 3, 4, 0, 0, 1, 2, 2, 3, 3, 4, 4, 1, 0},  // Hidden row 8
        {1, 1, 1, 2, 0, 3, 3, 4, 4, 1, 1, 2, 2, 3, 3, 4, 4, 1, 1, 0},  // Visible row 1
        {0, 2, 3, 3, 0, 0, 0, 2, 2, 2, 3, 3, 4, 4, 1, 0, 0, 2, 3, 3},  // Visible row 2
        {0, 4, 1, 1, 0, 2, 3, 0, 1, 0, 4, 4, 2, 2, 3, 3, 0, 1, 0, 4},  // Visible row 3
        {3, 3, 2, 2, 4, 4, 1, 1, 3, 3, 2, 2, 4, 4, 1, 1, 3, 3, 0, 2}}, // Initially visible row 4
    // Level 6 - Medium: 5 colors, pyramid pattern
    {
        {1, 0, 3, 0, 0, 1, 5, 5, 0, 0, 0, 5, 4, 0, 0, 0, 5, 2, 2, 4},  // Hidden row 1
        {3, 2, 5, 1, 4, 3, 2, 5, 1, 0, 3, 5, 5, 0, 4, 3, 2, 5, 1, 4},  // Hidden row 2
        {4, 1, 0, 0, 3, 4, 1, 2, 5, 0, 4, 1, 5, 5, 3, 0, 1, 2, 4, 4},  // Hidden row 3
        {2, 4, 0, 0, 5, 2, 4, 1, 3, 5, 2, 4, 1, 3, 5, 2, 4, 0, 0, 5},  // Hidden row 4
        {5, 3, 4, 2, 0, 5, 3, 4, 0, 0, 5, 3, 4, 2, 1, 0, 3, 0, 0, 1},  // Hidden row 5
        {1, 5, 2, 4, 3, 1, 5, 2, 0, 0, 1, 5, 2, 4, 0, 0, 5, 2, 4, 3},  // Hidden row 6
        {3, 1, 5, 2, 4, 3, 1, 5, 2, 4, 3, 0, 0, 2, 0, 3, 1, 5, 2, 4},  // Hidden row 7
        {4, 2, 3, 1, 5, 4, 2, 3, 1, 5, 4, 0, 0, 1, 5, 4, 2, 3, 1, 5},  // Hidden row 8
        {2, 4, 0, 0, 3, 1, 1, 1, 0, 3, 2, 4, 1, 5, 0, 2, 4, 1, 5, 3},  // Visible row 1
        {5, 3, 0, 0, 1, 1, 3, 1, 0, 0, 5, 0, 2, 4, 0, 0, 0, 2, 4, 1},  // Visible row 2
        {1, 5, 4, 3, 2, 1, 5, 2, 2, 2, 1, 0, 0, 3, 2, 0, 0, 4, 3, 2},  // Visible row 3 (changed a 1 to 0)
        {3, 2, 0, 5, 4, 3, 2, 1, 0, 1, 3, 2, 0, 5, 4, 3, 2, 0, 5, 4}}, // Initially visible row 4
    // Level 7 - Medium-Hard: 5 colors, circle pattern
    {
        {1, 0, 5, 2, 4, 1, 1, 1, 1, 4, 1, 3, 5, 2, 4, 1, 3, 0, 2, 4},  // Hidden row 1
        {4, 0, 0, 5, 3, 4, 1, 1, 5, 4, 4, 2, 1, 5, 3, 4, 2, 0, 5, 3},  // Hidden row 2
        {3, 5, 4, 1, 2, 3, 5, 1, 1, 0, 0, 5, 4, 1, 2, 3, 5, 4, 0, 2},  // Hidden row 3
        {2, 1, 0, 0, 5, 2, 1, 3, 4, 0, 0, 1, 3, 0, 0, 2, 1, 3, 4, 5},  // Hidden row 4
        {5, 4, 0, 0, 1, 5, 4, 2, 3, 2, 5, 4, 2, 0, 0, 5, 4, 2, 3, 1},  // Hidden row 5
        {1, 2, 5, 3, 4, 1, 2, 2, 2, 2, 1, 2, 5, 3, 4, 1, 2, 5, 3, 4},  // Hidden row 6
        {3, 4, 1, 0, 0, 3, 4, 2, 5, 2, 3, 4, 1, 5, 2, 3, 4, 1, 5, 2},  // Hidden row 7
        {2, 5, 3, 0, 0, 2, 5, 2, 0, 0, 2, 5, 3, 1, 4, 2, 5, 3, 1, 4},  // Hidden row 8
        {0, 3, 1, 5, 1, 4, 5, 1, 0, 0, 4, 3, 1, 0, 0, 4, 3, 1, 5, 2},  // Visible row 1
        {0, 2, 1, 1, 1, 5, 5, 4, 3, 1, 5, 2, 4, 3, 0, 5, 2, 0, 3, 1},  // Visible row 2
        {1, 0, 1, 0, 0, 1, 5, 2, 4, 3, 1, 5, 2, 4, 3, 1, 5, 0, 0, 3},  // Visible row 3
        {3, 1, 1, 0, 4, 1, 1, 5, 2, 4, 3, 1, 5, 2, 4, 3, 1, 5, 0, 4}}, // Initially visible row 4
    // Level 8 - Hard: 5 colors, reverse V pattern
    {
        {3, 1, 5, 0, 0, 3, 1, 5, 2, 4, 0, 1, 5, 0, 0, 3, 1, 0, 0, 4},  // Hidden row 1
        {4, 2, 1, 5, 3, 4, 2, 1, 5, 3, 4, 0, 0, 5, 3, 0, 2, 0, 5, 3},  // Hidden row 2
        {5, 3, 4, 1, 2, 5, 3, 4, 1, 2, 5, 3, 0, 1, 2, 0, 3, 4, 1, 2},  // Hidden row 3
        {2, 5, 3, 4, 1, 2, 5, 3, 4, 1, 2, 5, 3, 4, 0, 5, 5, 3, 4, 1},  // Hidden row 4
        {1, 4, 2, 3, 0, 1, 4, 2, 3, 5, 1, 0, 2, 5, 5, 1, 5, 2, 3, 5},  // Hidden row 5
        {3, 1, 5, 2, 0, 3, 1, 5, 2, 4, 3, 0, 0, 2, 4, 3, 1, 0, 2, 4},  // Hidden row 6
        {4, 2, 1, 5, 3, 4, 2, 1, 5, 3, 4, 0, 0, 5, 0, 4, 2, 0, 0, 3},  // Hidden row 7
        {5, 3, 4, 0, 2, 1, 1, 4, 1, 2, 5, 3, 4, 1, 0, 5, 3, 4, 5, 2},  // Hidden row 8
        {2, 4, 5, 0, 3, 1, 4, 5, 1, 3, 2, 4, 5, 1, 3, 2, 4, 5, 1, 5},  // Visible row 1
        {1, 5, 2, 4, 1, 1, 5, 2, 4, 2, 2, 2, 2, 4, 3, 1, 5, 0, 0, 3},  // Visible row 2
        {3, 1, 4, 1, 0, 0, 1, 2, 2, 2, 3, 2, 4, 0, 0, 3, 0, 3, 3, 2},  // Visible row 3
        {5, 2, 3, 1, 4, 0, 2, 2, 1, 4, 5, 2, 2, 1, 0, 5, 0, 3, 1, 4}}, // Initially visible row 4
    // Level 9 - Hard: 5 colors, diamond outline pattern
    {
        {1, 0, 0, 0, 1, 1, 5, 5, 4, 4, 5, 5, 4, 4, 4, 3, 5, 2, 2, 4},  // Hidden row 1
        {3, 2, 5, 1, 4, 3, 2, 5, 1, 4, 3, 5, 5, 0, 0, 3, 2, 5, 1, 4},  // Hidden row 2
        {4, 1, 0, 0, 3, 4, 1, 2, 5, 4, 4, 1, 5, 0, 0, 3, 1, 2, 4, 4},  // Hidden row 3
        {2, 4, 0, 0, 5, 2, 4, 1, 3, 5, 2, 4, 1, 3, 5, 2, 4, 0, 0, 5},  // Hidden row 4
        {5, 3, 4, 2, 0, 5, 3, 4, 0, 0, 5, 3, 4, 2, 0, 0, 3, 0, 0, 1},  // Hidden row 5
        {1, 5, 2, 4, 3, 1, 5, 2, 0, 0, 1, 5, 2, 4, 0, 1, 5, 2, 4, 3},  // Hidden row 6
        {3, 1, 5, 0, 0, 0, 1, 5, 2, 4, 3, 0, 0, 2, 0, 3, 1, 0, 2, 4},  // Hidden row 7
        {4, 2, 3, 1, 5, 4, 2, 3, 0, 5, 4, 0, 0, 1, 5, 4, 2, 3, 1, 5},  // Hidden row 8
        {2, 4, 0, 0, 3, 1, 1, 1, 5, 3, 2, 4, 1, 5, 3, 2, 4, 1, 5, 3},  // Visible row 1
        {5, 3, 0, 0, 1, 0, 0, 1, 1, 0, 0, 3, 2, 4, 1, 0, 0, 2, 0, 1},  // Visible row 2
        {1, 5, 4, 3, 2, 1, 5, 4, 1, 0, 0, 5, 4, 3, 2, 0, 0, 4, 3, 2},  // Visible row 3
        {3, 2, 1, 5, 4, 3, 2, 1, 1, 1, 3, 2, 1, 5, 4, 3, 2, 1, 5, 4}}, // Initially visible row 4
    // Level 10 - Very Hard: 5 colors, complex mixed pattern
    {
        {3, 3, 1, 4, 2, 2, 5, 1, 4, 5, 2, 2, 3, 1, 4, 4, 1, 3, 5, 5},   // Hidden row 1
        {2, 2, 4, 3, 1, 1, 5, 3, 4, 5, 1, 1, 2, 3, 4, 4, 3, 1, 2, 2},   // Hidden row 2
        {1, 1, 5, 2, 4, 0, 0, 1, 3, 3, 2, 2, 1, 0, 0, 5, 2, 2, 3, 3},   // Hidden row 3
        {0, 0, 3, 1, 5, 5, 4, 4, 2, 2, 3, 3, 5, 4, 0, 0, 1, 1, 2, 2},   // Hidden row 4
        {5, 5, 0, 4, 1, 1, 3, 3, 2, 2, 1, 1, 0, 0, 5, 5, 2, 2, 4, 4},   // Hidden row 5
        {4, 4, 2, 0, 3, 3, 5, 5, 1, 1, 0, 0, 3, 3, 1, 1, 2, 2, 4, 4},   // Hidden row 6
        {3, 3, 5, 5, 1, 1, 4, 4, 2, 2, 4, 4, 5, 5, 1, 1, 3, 3, 2, 2},   // Hidden row 7
        {2, 2, 1, 1, 5, 5, 3, 3, 4, 4, 3, 3, 2, 2, 1, 1, 5, 5, 4, 4},   // Hidden row 8
        {1, 1, 5, 2, 4, 0, 0, 1, 3, 3, 2, 2, 1, 0, 0, 5, 2, 2, 3, 3},   // Visible row 1
        {0, 0, 3, 1, 5, 5, 4, 4, 2, 2, 3, 3, 5, 4, 0, 0, 1, 1, 2, 2},   // Visible row 2
        {5, 5, 0, 4, 1, 1, 3, 3, 2, 2, 1, 1, 0, 0, 5, 5, 2, 2, 4, 4},   // Visible row 3
        {4, 4, 2, 0, 3, 3, 5, 5, 1, 1, 0, 0, 3, 3, 1, 1, 2, 2, 4, 4}}}; // Initially visible row 4// Ball limitation system
int ballsRemaining = 30;                                                // Balls left for current level
int maxBallsForLevel = 30;                                              // Maximum balls for current level
bool inHomepage = true;
bool inLevelSelect = false;
bool inGame = false;
bool inSettings = false;
bool inTribute = false;
bool inInstruction = false;
bool inPauseMenu = false;
bool inGameOver = false;
bool inLevelComplete = false;
bool inHighScore = false;
bool inNameInput = false;
bool highScoreSaved = false; // Flag to prevent duplicate high score saves
int soundVolume = 70;
int musicVolume = 50;
bool soundEnabled = true;

// Bomb system
int bombCount = 0;
int lastBombScore = 0;           // Track score for bomb rewards
bool bombSelected = false;       // Track if bomb is currently selected
const int POINTS_PER_BOMB = 700; // Points needed for each bomb

// High Score System
typedef struct
{
    char name[20];
    int score;
    int level;
} HighScore;

HighScore highScores[10]; // Top 10 high scores
int highScoreCount = 0;
char playerName[20] = "";
int nameInputPos = 0;

typedef struct
{
    double x, y;
    double vy;
    int color;
    int isFalling;
} FallingBubble;
FallingBubble fallingBubbles[100];

int iGetTimer()
{
    return (int)(clock() * 1000 / CLOCKS_PER_SEC);
}

bool checkGameOver()
{
    for (int r = GAME_OVER_ROW; r < ROWS; r++)
    {
        int maxCols = (r % 2 == 0) ? COLS : (COLS - 1);
        for (int c = 0; c < maxCols; c++)
        {
            if (grid[r][c] != 0)
            {
                return true;
            }
        }
    }
    return false;
}

int movesCount = 0;
int movesPerRowDrop = 5; // Drop 2 rows every 5 moves in level 1
int hiddenRows[3][COLS];
int maxHiddenRows = 3;
int currentHiddenRowsUsed = 0;

// Velocity-based row drop animation system
bool isAnimatingRowDrop = false;
double gridVerticalOffset = 0.0;      // Current vertical offset for the entire grid
const double ROW_DROP_VELOCITY = 3.0; // Pixels per frame for smooth animation
double targetVerticalOffset = 0.0;    // Target offset (2 rows height)

// Pop animation system
struct PopAnimation
{
    int row, col;
    int color;
    int startTime;
    bool isActive;
};

PopAnimation poppingBubbles[50]; // Max bubbles that can pop at once
int poppingBubbleCount = 0;
const int POP_ANIMATION_DURATION = 200; // 200ms per bubble pop (much faster)
const int POP_STAGGER_DELAY = 50;       // 50ms delay between each bubble (faster chain reaction)

bool moveCounted = false;          // Flag to track if current move has been counted
int level1DisplayOffset = 8;       // Start showing from row 8 (0-indexed), so initially show rows 8-11 (4 rows)
int currentLevelDisplayOffset = 8; // Universal display offset for all levels
bool musicEnabled = true;
bool isDraggingVolume = false;
int backgroundMusicChannel = -1;

void loadLevel(int levelNumber);
int getRandomLevelColor();
bool checkLevelComplete();
void detectCluster();
void checkForRowDrop();
void finalizeRowDrop();
void loadHighScores();
void saveHighScores();
void addHighScore(char *name, int score, int level);
void drawHighScorePage();
void drawNameInputPage();
void handleLevelComplete();
void unlockNextLevel();
void saveLevelProgress();
void loadLevelProgress();
void drawBombIcon(double x, double y, bool isSelected);
void explodeBomb(int centerRow, int centerCol);

// Function to calculate maximum balls for a level
int getMaxBallsForLevel(int levelNumber)
{
    return 25 + (levelNumber * 5);
}

// Function to load level data from hardcoded array
void loadLevelFromFile(int levelNumber, int levelGrid[12][20])
{
    // Use hardcoded levels instead of reading from files
    if (levelNumber >= 1 && levelNumber <= 10)
    {
        // Copy from hardcoded levels array
        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < 20; j++)
            {
                levelGrid[i][j] = levels[levelNumber - 1][i][j];
            }
        }
    }
    else
    {
        // If level doesn't exist, create empty level
        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < 20; j++)
            {
                levelGrid[i][j] = 0;
            }
        }
    }
}

void startNewGame()
{
    Score = 0;
    movesCount = 0;
    bombCount = 0;
    currentHiddenRowsUsed = 0;
    isBallMoving = false;
    ballBounceCount = 0;
    fallingBubbleCount = 0;
    isAnimatingRowDrop = false;
    gridVerticalOffset = 0.0;
    poppingBubbleCount = 0;
    ShowIncrement = false;
    ShowPopping = false;
    inGameOver = false;
    inLevelComplete = false;
    // Reset ball count for current level
    ballsRemaining = maxBallsForLevel;
    // Clear the entire grid first
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            grid[r][c] = 0;
        }
    }
    loadLevel(currentLevel);
    shootingBallColorNow = getRandomLevelColor();
    shootingBallColorNext = getRandomLevelColor();
}

void goToNextLevel()
{
    if (currentLevel < 10)
    {
        currentLevel++;
        Score = 0;
        movesCount = 0;
        currentHiddenRowsUsed = 0;
        isBallMoving = false;
        ballBounceCount = 0;
        fallingBubbleCount = 0;
        isAnimatingRowDrop = false;
        gridVerticalOffset = 0.0;
        poppingBubbleCount = 0; // Reset pop animations
        ShowIncrement = false;
        ShowPopping = false;
        inGameOver = false;
        inLevelComplete = false;
        // Clear the entire grid first
        for (int r = 0; r < ROWS; r++)
        {
            for (int c = 0; c < COLS; c++)
            {
                grid[r][c] = 0;
            }
        }
        loadLevel(currentLevel);
        shootingBallColorNow = getRandomLevelColor();
        shootingBallColorNext = getRandomLevelColor();
    }
    else
    {
        // All levels completed, go back to level select or homepage
        inGame = false;
        inLevelSelect = true;
    }
}
// Helper function to get text width (improved approximation)
int getTextWidth(const char *text, int fontSize)
{
    return strlen(text) * fontSize * 0.55; // More accurate character width for better centering
}
// Helper function to center text in a rectangle with improved accuracy
void drawCenteredText(int rectX, int rectY, int rectWidth, int rectHeight, const char *text, const char *font, int fontSize)
{
    // Calculate approximate center position
    int x = rectX + rectWidth / 2 - (strlen(text) * 6); // Approximate text width
    int y = rectY + rectHeight / 2;
    iText(x, y, text, GLUT_BITMAP_HELVETICA_18);
}

// Forward declaration
bool isPointInRect(int px, int py, int rx, int ry, int width, int height);
void dropNewRow(); // Forward declaration for dropNewRow function
// Helper function to get available colors from the current level
int getRandomLevelColor()
{
    // Collect unique colors from the current playing grid and remaining hidden rows
    bool colorsAvailable[6] = {false}; // indices 1-5 for colors 1-5
    int colorCount = 0;
    // Scan the current playing grid for available colors
    for (int i = 0; i < ROWS; i++)
    {
        int colsInRow = (i % 2 == 0) ? COLS : (COLS - 1);
        for (int j = 0; j < colsInRow; j++)
        {
            int color = grid[i][j];
            if (color >= 1 && color <= 5 && !colorsAvailable[color])
            {
                colorsAvailable[color] = true;
                colorCount++;
            }
        }
    }
    // For progressive system, also scan remaining hidden rows from predefined level data
    if (currentLevelDisplayOffset > 1) // Only when we can still drop rows
    {
        for (int i = 0; i < currentLevelDisplayOffset; i++) // Only scan remaining hidden rows
        {
            int colsInRow = (i % 2 == 0) ? COLS : (COLS - 1);
            for (int j = 0; j < colsInRow; j++)
            {
                int color = currentLevelData[i][j]; // Get from current level data
                if (color >= 1 && color <= 5 && !colorsAvailable[color])
                {
                    colorsAvailable[color] = true;
                    colorCount++;
                }
            }
        }
    }
    // If no colors found, use default colors for the level
    if (colorCount == 0)
    {
        if (currentLevel <= 3)
        {
            // Levels 1-3 use colors 1, 2, 3
            return rand() % 3 + 1;
        }
        else if (currentLevel <= 6)
        {
            // Levels 4-6 use colors 1, 2, 3, 4
            return rand() % 4 + 1;
        }
        else
        {
            // Levels 7-10 use colors 1, 2, 3, 4, 5
            return rand() % 5 + 1;
        }
    }
    // Create array of available colors
    int availableColors[5];
    int index = 0;
    for (int i = 1; i <= 5; i++)
    {
        if (colorsAvailable[i])
        {
            availableColors[index++] = i;
        }
    }
    // Return random color from available ones
    return availableColors[rand() % colorCount];
}

bool checkLevelComplete()
{
    // Don't check level completion while pop animations are running
    if (poppingBubbleCount > 0)
    {
        return false; // Wait for animations to finish
    }
    // Check if all bubbles are cleared from the grid
    int bubbleCount = 0;
    for (int r = 0; r < ROWS; r++)
    {
        int colsInRow = (r % 2 == 0) ? COLS : (COLS - 1); // Respect hexagonal grid structure
        for (int c = 0; c < colsInRow; c++)
        {
            if (grid[r][c] != 0)
            {
                bubbleCount++;
            }
        }
    }

    if (bubbleCount == 0)
    {
        return true; // All bubbles cleared, level complete
    }

    return false; // Still have bubbles
}

void generateRandomGrid()
{
    for (int i = 0; i < Rows; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            grid[i][j] = levels[currentLevel - 1][i][j];
        }
    }
}

void loadLevel(int levelNumber)
{
    // Reset high score save flag for new level
    highScoreSaved = false;
    if (levelNumber >= 1 && levelNumber <= 10)
    {
        currentLevel = levelNumber;
        // Load level data from hardcoded array into global variable
        loadLevelFromFile(levelNumber, currentLevelData);
        maxBallsForLevel = getMaxBallsForLevel(levelNumber);
        ballsRemaining = maxBallsForLevel;
        // Initialize the grid with empty rows
        for (int i = 0; i < Rows; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                grid[i][j] = 0;
            }
        }
        // Use the same progressive system as Level 1 for all levels
        int initialVisibleRows = 4; // All levels start with 4 visible rows
        int startOffset = 8;
        // Copy only the currently visible rows from the loaded level data
        for (int i = 0; i < initialVisibleRows; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                grid[i][j] = currentLevelData[startOffset + i][j]; // Copy from offset position
            }
        }
        // Reset progressive system variables for all levels
        currentHiddenRowsUsed = 0;
        currentLevelDisplayOffset = startOffset; // Universal offset for all levels
        movesCount = 0;
        isAnimatingRowDrop = false;
        gridVerticalOffset = 0.0;
        // Setup hidden rows for all levels - use the first rows from the loaded data
        for (int i = 0; i < maxHiddenRows && i < startOffset; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                hiddenRows[i][j] = currentLevelData[i][j]; // Copy from loaded level data
            }
        }
        inLevelSelect = false;
        inGame = true;
        Score = 0;
        bombCount = 0;          // Reset bomb counter at the start of each level
        highScoreSaved = false; // Reset high score save flag for new game
        isBallMoving = false;
        fallingBubbleCount = 0;
        shootingBallColorNow = getRandomLevelColor();
        shootingBallColorNext = getRandomLevelColor();
    }
}
// Function to check if we need to drop a new row after a move
void checkForRowDrop()
{
    // Don't drop rows if pop animations are active or if already animating row drop
    bool popAnimationsActive = false;
    for (int i = 0; i < poppingBubbleCount; i++)
    {
        if (poppingBubbles[i].isActive)
        {
            popAnimationsActive = true;
            break;
        }
    }
    if (currentLevelDisplayOffset > 1 && !moveCounted && !popAnimationsActive && !isAnimatingRowDrop) // Need at least 2 rows to drop
    {
        moveCounted = true; // Mark this move as counted
        movesCount++;
        if (movesCount >= movesPerRowDrop)
        {
            dropNewRow();
            movesCount = 0; // Reset move counter
        }
    }
}
// Helper function to check if there are any balls in the top row
bool hasTopRowBalls()
{
    for (int c = 0; c < COLS; c++)
    {
        if (grid[0][c] != 0)
            return true;
    }
    return false;
}
// Helper function to check if there are hidden rows remaining
bool hasHiddenRows()
{
    return (currentLevelDisplayOffset > 1); // Need at least 2 rows to drop
}
// Update animation offset for smooth row dropping
void updateRowDropAnimation()
{
    if (isAnimatingRowDrop)
    {
        // Move the grid down by velocity each frame
        gridVerticalOffset += ROW_DROP_VELOCITY;
        // Check if we've reached the target (2 rows worth of movement)
        if (gridVerticalOffset >= targetVerticalOffset)
        {
            // Animation complete - finalize the grid positions
            gridVerticalOffset = 0.0;
            isAnimatingRowDrop = false;
            finalizeRowDrop();
        }
    }
}

// Pop animation functions
void startPopAnimation(int matchedBubbles[][2], int count)
{
    poppingBubbleCount = count;
    int currentTime = iGetTimer();
    // Each bubble pops individually with increasing delay
    for (int i = 0; i < count; i++)
    {
        poppingBubbles[i].row = matchedBubbles[i][0];
        poppingBubbles[i].col = matchedBubbles[i][1];
        poppingBubbles[i].color = grid[matchedBubbles[i][0]][matchedBubbles[i][1]];
        poppingBubbles[i].startTime = currentTime + (i * POP_STAGGER_DELAY);
        poppingBubbles[i].isActive = true;
    }
}

void updatePopAnimations()
{
    int currentTime = iGetTimer();
    bool anyBubbleJustFinished = false;

    for (int i = 0; i < poppingBubbleCount; i++)
    {
        if (poppingBubbles[i].isActive)
        {
            int elapsed = currentTime - poppingBubbles[i].startTime;
            // Check if this bubble's animation should start
            if (elapsed >= 0)
            {
                // Check if animation is complete
                if (elapsed >= POP_ANIMATION_DURATION)
                {
                    // Remove the bubble from grid when animation completes
                    grid[poppingBubbles[i].row][poppingBubbles[i].col] = 0;
                    poppingBubbles[i].isActive = false;
                    anyBubbleJustFinished = true;
                }
            }
        }
    }

    // Check if all animations are complete
    bool allComplete = true;
    for (int i = 0; i < poppingBubbleCount; i++)
    {
        if (poppingBubbles[i].isActive)
        {
            allComplete = false;
            break;
        }
    }

    if (allComplete && poppingBubbleCount > 0)
    {
        poppingBubbleCount = 0; // Reset the system
        // After all pop animations complete, check for floating bubbles and level completion
        detectCluster(); // This will handle floating bubbles
        // Check if level is completed after all bubbles are cleared
        if (checkLevelComplete())
        {
            handleLevelComplete();
        }
        else
        {
            // If level not complete, check if we need to drop rows (that might have been delayed)
            checkForRowDrop();
        }
    }
}

double getPopScale(int bubbleIndex)
{
    if (!poppingBubbles[bubbleIndex].isActive)
        return 1.0;
    int currentTime = iGetTimer();
    int elapsed = currentTime - poppingBubbles[bubbleIndex].startTime;
    if (elapsed < 0)
        return 1.0; // Animation hasn't started yet
    if (elapsed >= POP_ANIMATION_DURATION)
        return 0.0; // Animation complete
    double progress = (double)elapsed / POP_ANIMATION_DURATION;

    if (progress < 0.2)
    {
        // First 20%: quickly grow larger (1.0 to 1.6)
        return 1.0 + (progress / 0.2) * 0.6;
    }
    else
    {
        // Last 80%: quickly shrink to 0 (1.6 to 0.0)
        double shrinkProgress = (progress - 0.2) / 0.8;
        return 1.6 * (1.0 - shrinkProgress * shrinkProgress); // Quadratic for faster shrink
    }
}

// Finalize the row drop by updating the grid data structure
void finalizeRowDrop()
{
    // Shift all existing rows down by 2 positions to maintain hexagonal pattern
    for (int r = ROWS - 1; r >= 2; r--)
    {
        int colsInRow = (r % 2 == 0) ? COLS : (COLS - 1);
        for (int c = 0; c < colsInRow; c++)
        {
            grid[r][c] = grid[r - 2][c];
        }
    }
    // Add first new row (will become row 1 - odd)
    currentLevelDisplayOffset--;
    int row1Cols = COLS - 1; // Row 1 is odd, so it has COLS-1 columns
    for (int c = 0; c < row1Cols; c++)
    {
        grid[1][c] = currentLevelData[currentLevelDisplayOffset][c];
    }
    // Add second new row (will become row 0 - even)
    currentLevelDisplayOffset--;
    int row0Cols = COLS; // Row 0 is even, so it has COLS columns
    for (int c = 0; c < row0Cols; c++)
    {
        grid[0][c] = currentLevelData[currentLevelDisplayOffset][c];
    }
    // After row drop, detect and handle floating bubbles
    detectCluster();
}

// High Score System Functions
void loadHighScores()
{
    FILE *file = fopen("saves/highscores.txt", "r");
    if (file == NULL)
    {
        highScoreCount = 0;
        return;
    }

    highScoreCount = 0;
    while (highScoreCount < 10 && fscanf(file, "%s %d %d", highScores[highScoreCount].name, &highScores[highScoreCount].score, &highScores[highScoreCount].level) == 3)
    {
        highScoreCount++;
    }
    fclose(file);
}
void saveHighScores()
{
    FILE *file = fopen("saves/highscores.txt", "w");
    if (file == NULL)
        return;

    for (int i = 0; i < highScoreCount; i++)
    {
        fprintf(file, "%s %d %d\n", highScores[i].name, highScores[i].score, highScores[i].level);
    }
    fclose(file);
}
void addHighScore(char *name, int score, int level)
{
    // Find position to insert new score
    int insertPos = highScoreCount;
    for (int i = 0; i < highScoreCount; i++)
    {
        if (score > highScores[i].score)
        {
            insertPos = i;
            break;
        }
    }
    // Shift scores down
    for (int i = (highScoreCount < 10 ? highScoreCount : 9); i > insertPos; i--)
    {
        highScores[i] = highScores[i - 1];
    }
    // Insert new score
    strcpy(highScores[insertPos].name, name);
    highScores[insertPos].score = score;
    highScores[insertPos].level = level;
    if (highScoreCount < 10)
        highScoreCount++;
    saveHighScores();
}

void drawHighScorePage()
{
    iClear();
    iShowImage(0, 0, "assets/images/bouncing ball/setttings.png");
    iSetColor(255, 215, 0); // Gold
    iText(GAME_WINDOW_WIDTH / 2 - 80, 650, "HIGH SCORES", GLUT_BITMAP_HELVETICA_18);
    // Headers
    iSetColor(200, 200, 200);
    iText(150, 580, "Rank", GLUT_BITMAP_HELVETICA_18);
    iText(230, 580, "Name", GLUT_BITMAP_HELVETICA_18);
    iText(430, 580, "Score", GLUT_BITMAP_HELVETICA_18);
    iText(580, 580, "Level", GLUT_BITMAP_HELVETICA_18);

    for (int i = 0; i < highScoreCount; i++)
    {
        int y = 540 - i * 40;
        if (i == 0)
            iSetColor(255, 215, 0); // Gold
        else if (i == 1)
            iSetColor(192, 192, 192); // Silver
        else if (i == 2)
            iSetColor(205, 127, 50); // Bronze
        else
            iSetColor(255, 255, 255); // White

        char rankText[10], scoreText[20], levelText[10];
        sprintf(rankText, "%d", i + 1);
        sprintf(scoreText, "%d", highScores[i].score);
        sprintf(levelText, "%d", highScores[i].level);

        iText(160, y, rankText, GLUT_BITMAP_HELVETICA_18);
        iText(230, y, highScores[i].name, GLUT_BITMAP_HELVETICA_18);
        iText(430, y, scoreText, GLUT_BITMAP_HELVETICA_18);
        iText(590, y, levelText, GLUT_BITMAP_HELVETICA_18);
    }

    // Back button
    int buttonX = 50, buttonY = 50, buttonW = 120, buttonH = 50;
    iSetColor(70, 130, 180);
    iFilledRectangle(buttonX, buttonY, buttonW, buttonH);
    iSetColor(255, 255, 255);
    iRectangle(buttonX, buttonY, buttonW, buttonH);
    iText(buttonX + 35, buttonY + 15, "Back", GLUT_BITMAP_HELVETICA_18);
}

void drawNameInputPage()
{
    iClear();
    // Display settings background image
    iShowImage(0, 0, "assets/images/bouncing ball/setttings.png");
    iSetColor(255, 215, 0);
    iText(GAME_WINDOW_WIDTH / 2 - 70, 470, "Enter Your Name", GLUT_BITMAP_TIMES_ROMAN_24);
    // Name input box
    int boxX = GAME_WINDOW_WIDTH / 2 - 170, boxY = 380, boxW = 360, boxH = 50;
    iSetColor(50, 50, 50);
    iFilledRectangle(boxX, boxY, boxW, boxH);
    iSetColor(255, 255, 255);
    iRectangle(boxX, boxY, boxW, boxH);
    iSetColor(255, 255, 255);
    if (strlen(playerName) > 0)
    {
        iText(boxX + 10, boxY + 25, playerName, GLUT_BITMAP_HELVETICA_18);
    }
    iText(boxX + 10 + strlen(playerName) * 10, boxY + 25, "_", GLUT_BITMAP_HELVETICA_18);
    if (strlen(playerName) > 0)
    {
        int buttonX = GAME_WINDOW_WIDTH / 2 - 60, buttonY = 250, buttonW = 150, buttonH = 50;
        // Check if hovering over continue button
        bool isHoveringContinue = isPointInRect(mouseX, mouseY, buttonX, buttonY, buttonW, buttonH);
        if (isHoveringContinue)
        {
            iSetColor(100, 150, 200); // Lighter blue when hovering
        }
        else
        {
            iSetColor(70, 130, 180); // Normal blue
        }
        iFilledRectangle(buttonX, buttonY, buttonW, buttonH);
        iSetColor(255, 255, 255);
        iRectangle(buttonX, buttonY, buttonW, buttonH);
        if (isHoveringContinue)
        {
            iSetColor(255, 255, 0); // Yellow text when hovering
        }
        else
        {
            iSetColor(255, 255, 255); // White text normally
        }
        iText(buttonX + 30, buttonY + 20, "Continue", GLUT_BITMAP_HELVETICA_18);
    }
    // Back button - transparent with hover effect
    bool isHoveringBack = isPointInRect(mouseX, mouseY, 50, 50, 100, 40);
    if (isHoveringBack)
    {
        // Light colored rectangle around button when hovering
        iSetColor(100, 100, 100); // Light gray background
        iFilledRectangle(45, 48, 110, 44);
        iSetColor(255, 255, 0); // Yellow text when hovering
    }
    else
    {
        iSetColor(255, 255, 255); // White text normally
    }
    iText(75, 70, "Back", GLUT_BITMAP_HELVETICA_18);
}

void handleLevelComplete()
{
    inLevelComplete = true;
    // Save high score only once per level completion
    if (strlen(playerName) > 0 && !highScoreSaved)
    {
        addHighScore(playerName, Score, currentLevel);
        highScoreSaved = true; // Mark as saved to prevent duplicates
    }
    unlockNextLevel();
}

// Level progression functions
void unlockNextLevel()
{
    if (currentLevel < 10 && unlockedLevels == currentLevel)
    {
        unlockedLevels = currentLevel + 1;
        saveLevelProgress();
    }
}

void saveLevelProgress()
{
// Create saves directory if it doesn't exist
#ifdef _WIN32
    CreateDirectoryA("saves", NULL);
#else
    mkdir("saves", 0777);
#endif
    FILE *file = fopen("saves/level_progress.txt", "w");
    if (file != NULL)
    {
        fprintf(file, "%d\n", unlockedLevels);
        fclose(file);
    }
}

void loadLevelProgress()
{
    FILE *file = fopen("saves/level_progress.txt", "r");
    if (file != NULL)
    {
        fscanf(file, "%d", &unlockedLevels);
        fclose(file);
        // Ensure at least level 1 is unlocked
        if (unlockedLevels < 1)
            unlockedLevels = 1;
        if (unlockedLevels > 10)
            unlockedLevels = 10;
    }
    else
    {
        unlockedLevels = 1; // Default: only level 1 unlocked
    }
}

void drawLevelSelect()
{
    iClear();
    // Display background image
    iShowImage(0, 0, "assets/images/bouncing ball/setttings.png");
    // Title
    iSetColor(0, 191, 255); // Ocean blue
    iText(GAME_WINDOW_WIDTH / 2 - 80, 650, "SELECT LEVEL", GLUT_BITMAP_HELVETICA_18);
    iText(GAME_WINDOW_WIDTH / 2 - 180, 250, "press space button to swap the bubble", GLUT_BITMAP_HELVETICA_18);
    iSetColor(255, 255, 255);
    iText(GAME_WINDOW_WIDTH / 2 - 180, 50, "press '>' to unlock all levels & press '<' to set previous", GLUT_BITMAP_HELVETICA_12);
    // Draw level buttons in a 2x5 grid with enhanced styling and hover effects
    for (int i = 0; i < 10; i++)
    {
        int row = i / 5;
        int col = i % 5;
        int baseX = 100 + col * 120;
        int baseY = 450 - row * 100;
        // Check if hovering for size increase effect
        bool isHovering = isPointInRect(mouseX, mouseY, baseX, baseY, 80, 60);
        // Calculate button dimensions with hover effect
        int buttonWidth = isHovering ? 88 : 80;  // 8 pixels wider when hovering
        int buttonHeight = isHovering ? 66 : 60; // 6 pixels taller when hovering
        int x = isHovering ? baseX - 4 : baseX;  // Center the larger button
        int y = isHovering ? baseY - 3 : baseY;  // Center the larger button
        // Enhanced button styling
        bool isLevelUnlocked = (i + 1) <= unlockedLevels;
        if (!isLevelUnlocked)
        {
            // Locked level - gray styling
            iSetColor(50, 50, 50); // Dark gray
            iFilledRectangle(x, y, buttonWidth, buttonHeight);
            // Darker inner area
            iSetColor(30, 30, 30);
            iFilledRectangle(x + 2, y + 2, buttonWidth - 4, buttonHeight - 4);
        }
        else if (i + 1 == currentLevel)
        {
            // Current level - bright ocean blue with glow effect
            iSetColor(0, 150, 255); // Bright ocean blue
            iFilledRectangle(x, y, buttonWidth, buttonHeight);
            // Outer glow effect (adjusted for size)
            iSetColor(100, 200, 255);
            iRectangle(x - 2, y - 2, buttonWidth + 4, buttonHeight + 4);
            iSetColor(50, 175, 255);
            iRectangle(x - 1, y - 1, buttonWidth + 2, buttonHeight + 2);
        }
        else
        {
            // Normal unlocked levels - navy theme with gradient effect
            iSetColor(30, 40, 80); // Dark navy
            iFilledRectangle(x, y, buttonWidth, buttonHeight);
            // Inner gradient effect (adjusted for size)
            iSetColor(40, 60, 100);
            iFilledRectangle(x + 2, y + 2, buttonWidth - 4, buttonHeight - 4);
            iSetColor(50, 70, 120);
            iFilledRectangle(x + 4, y + 4, buttonWidth - 8, buttonHeight - 8);
        }
        // Button border with ocean blue theme (gray for locked levels)
        if (!isLevelUnlocked)
        {
            iSetColor(80, 80, 80); // Gray border for locked levels
        }
        else
        {
            iSetColor(135, 206, 250); // Light sky blue border for unlocked levels
        }
        iRectangle(x, y, buttonWidth, buttonHeight);
        // Inner border for depth
        if (!isLevelUnlocked)
        {
            iSetColor(60, 60, 60); // Darker gray inner border
        }
        else
        {
            iSetColor(100, 180, 230);
        }
        iRectangle(x + 1, y + 1, buttonWidth - 2, buttonHeight - 2);
        // Level number with enhanced styling and hover effects
        char levelText[10];
        sprintf(levelText, "%d", i + 1);
        // Check for hover effect on level buttons
        if (!isLevelUnlocked)
        {
            iSetColor(100, 100, 100); // Gray text for locked levels
        }
        else if (isHovering)
        {
            iSetColor(204, 204, 0); // Bright red when hovering over any level button
        }
        else if (i + 1 == currentLevel)
        {
            iSetColor(0, 255, 0); // Bright green for current level
        }
        else
        {
            iSetColor(255, 255, 255); // White for other levels
        }
        // Center the text within the button
        iText(x + 22, y + 25, levelText, GLUT_BITMAP_HELVETICA_18);
    }
    // Simple Back to Home text (no box) with hover effect
    if (isPointInRect(mouseX, mouseY, 50, 110, 130, 25))
    {
        iSetColor(255, 255, 0); // Yellow text when hovering
    }
    else
    {
        iSetColor(255, 255, 255); // White text normally
    }
    iText(50, 115, "Back to Home", GLUT_BITMAP_HELVETICA_18);
}

bool isPointInRect(int px, int py, int rx, int ry, int width, int height)
{
    return (px >= rx && px <= rx + width && py >= ry && py <= ry + height);
}

void drawHomepage()
{
    iClear();
    // Display home background image
    iShowImage(0, 0, "assets/images/bouncing ball/home.png");

    // Menu buttons
    int buttonWidth = 200;
    int buttonHeight = 60;
    int buttonSpacing = 80;
    int startY = 450;
    // Enter Game button
    bool isHoveringEnter = isPointInRect(mouseX, mouseY, 300, startY, buttonWidth, buttonHeight);
    int enterWidth = isHoveringEnter ? buttonWidth + 10 : buttonWidth;
    int enterHeight = isHoveringEnter ? buttonHeight + 6 : buttonHeight;
    int enterX = isHoveringEnter ? 295 : 300;
    int enterY = isHoveringEnter ? startY - 3 : startY;
    iSetColor(0, 51, 102);
    iFilledRectangle(enterX, enterY, enterWidth, enterHeight);
    iSetColor(255, 255, 255);
    iRectangle(enterX, enterY, enterWidth, enterHeight);

    if (isHoveringEnter)
    {
        iSetColor(255, 255, 0); // Yellow text when hovering
    }
    else
    {
        iSetColor(0, 0, 0); // White text normally
    }
    drawCenteredText(enterX + 14, enterY, enterWidth, enterHeight, "Enter Game", "assets/fonts/arial.ttf", 24);
    // Settings button
    bool isHoveringSettings = isPointInRect(mouseX, mouseY, 300, startY - buttonSpacing, buttonWidth, buttonHeight);
    int settingsWidth = isHoveringSettings ? buttonWidth + 10 : buttonWidth;
    int settingsHeight = isHoveringSettings ? buttonHeight + 6 : buttonHeight;
    int settingsX = isHoveringSettings ? 295 : 300;
    int settingsY = isHoveringSettings ? (startY - buttonSpacing - 3) : (startY - buttonSpacing);

    iSetColor(0, 76, 153);
    iFilledRectangle(settingsX, settingsY, settingsWidth, settingsHeight);
    iSetColor(255, 255, 255);
    iRectangle(settingsX, settingsY, settingsWidth, settingsHeight);
    if (isHoveringSettings)
    {
        iSetColor(255, 255, 0); // Yellow text when hovering
    }
    else
    {
        iSetColor(0, 0, 0); // White text normally
    }
    drawCenteredText(settingsX + 12, settingsY, settingsWidth, settingsHeight, "Settings", "assets/fonts/arial.ttf", 24);
    // Tribute button
    bool isHoveringTribute = isPointInRect(mouseX, mouseY, 300, startY - 2 * buttonSpacing, buttonWidth, buttonHeight);
    int tributeWidth = isHoveringTribute ? buttonWidth + 10 : buttonWidth;
    int tributeHeight = isHoveringTribute ? buttonHeight + 6 : buttonHeight;
    int tributeX = isHoveringTribute ? 295 : 300;
    int tributeY = isHoveringTribute ? (startY - 2 * buttonSpacing - 3) : (startY - 2 * buttonSpacing);
    iSetColor(0, 102, 206);
    iFilledRectangle(tributeX, tributeY, tributeWidth, tributeHeight);
    iSetColor(255, 255, 255);
    iRectangle(tributeX, tributeY, tributeWidth, tributeHeight);
    if (isHoveringTribute)
    {
        iSetColor(255, 255, 0); // Yellow text when hovering
    }
    else
    {
        iSetColor(0, 0, 0); // White text normally
    }
    drawCenteredText(tributeX + 12, tributeY, tributeWidth, tributeHeight, "Tribute", "assets/fonts/arial.ttf", 24);
    // High Score button
    bool isHoveringHighScore = isPointInRect(mouseX, mouseY, 300, startY - 3 * buttonSpacing, buttonWidth, buttonHeight);
    int highScoreWidth = isHoveringHighScore ? buttonWidth + 10 : buttonWidth;
    int highScoreHeight = isHoveringHighScore ? buttonHeight + 6 : buttonHeight;
    int highScoreX = isHoveringHighScore ? 295 : 300;
    int highScoreY = isHoveringHighScore ? (startY - 3 * buttonSpacing - 3) : (startY - 3 * buttonSpacing);
    iSetColor(0, 128, 255); // Purple color for high score
    iFilledRectangle(highScoreX, highScoreY, highScoreWidth, highScoreHeight);
    iSetColor(255, 255, 255);
    iRectangle(highScoreX, highScoreY, highScoreWidth, highScoreHeight);
    if (isHoveringHighScore)
    {
        iSetColor(255, 255, 0); // Yellow text when hovering
    }
    else
    {
        iSetColor(0, 0, 0); // White text normally
    }
    drawCenteredText(highScoreX + 15, highScoreY, highScoreWidth, highScoreHeight, "High Score", "assets/fonts/arial.ttf", 24);
    // Exit button
    bool isHoveringExit = isPointInRect(mouseX, mouseY, 300, startY - 4 * buttonSpacing, buttonWidth, buttonHeight);
    int exitWidth = isHoveringExit ? buttonWidth + 10 : buttonWidth;
    int exitHeight = isHoveringExit ? buttonHeight + 6 : buttonHeight;
    int exitX = isHoveringExit ? 295 : 300;
    int exitY = isHoveringExit ? (startY - 4 * buttonSpacing - 3) : (startY - 4 * buttonSpacing);
    iSetColor(51, 153, 255); // Red color for exit
    iFilledRectangle(exitX, exitY, exitWidth, exitHeight);
    iSetColor(255, 255, 255);
    iRectangle(exitX, exitY, exitWidth, exitHeight);
    if (isHoveringExit)
    {
        iSetColor(255, 255, 0); // Yellow text when hovering
    }
    else
    {
        iSetColor(0, 0, 0); // White text normally
    }
    drawCenteredText(exitX + 9, exitY, exitWidth, exitHeight, "Exit", "assets/fonts/arial.ttf", 24);
    // instruction button
    bool isHoveringhelp = isPointInRect(mouseX, mouseY, 300, startY - 5 * buttonSpacing, buttonWidth, buttonHeight);
    int helpWidth = isHoveringhelp ? buttonWidth + 10 : buttonWidth;
    int helpHeight = isHoveringhelp ? buttonHeight + 6 : buttonHeight;
    int helpX = isHoveringhelp ? 295 : 300;
    int helpY = isHoveringhelp ? (startY - 5 * buttonSpacing - 3) : (startY - 5 * buttonSpacing);
    iSetColor(102, 178, 255); // Red color for Instruction button rect
    iFilledRectangle(helpX, helpY, helpWidth, helpHeight);
    iSetColor(255, 255, 255);
    iRectangle(helpX, helpY, helpWidth, helpHeight);
    if (isHoveringhelp)
    {
        iSetColor(255, 255, 0); // Yellow text when hovering
    }
    else
    {
        iSetColor(0, 0, 0); // White text normally
    }
    drawCenteredText(helpX + 20, helpY, helpWidth, helpHeight, "Instructions", "assets/fonts/arial.ttf", 24);
}

void drawInstruction()
{
    iClear();
    // Display instruction image (make sure this image contains the instructions visually)
    iShowImage(0, 0, "assets/images/bouncing ball/instructions.png");
    // Back button in bottom left corner with hover effect
    bool isHoveringBack = isPointInRect(mouseX, mouseY, 50, 50, 100, 40);
    // Light colored rectangle when hovering
    if (isHoveringBack)
    {
        iSetColor(255, 255, 255); // White background when hovering
        iFilledRectangle(45, 45, 110, 50);
    }
    // Button background
    int backWidth = isHoveringBack ? 110 : 100;
    int backHeight = isHoveringBack ? 50 : 40;
    int backX = isHoveringBack ? 45 : 50;
    int backY = isHoveringBack ? 45 : 50;
    iSetColor(70, 70, 70);
    iFilledRectangle(backX, backY, backWidth, backHeight);
    iSetColor(255, 255, 255);
    iRectangle(backX, backY, backWidth, backHeight);
    // Button text
    if (isHoveringBack)
    {
        iSetColor(255, 255, 0); // Yellow text when hovering
    }
    else
    {
        iSetColor(255, 255, 255); // White text normally
    }
    iText(backX + 30, backY + 20, "Back", GLUT_BITMAP_HELVETICA_12);
}

void drawSettings()
{
    iClear();
    // Display settings background image
    iShowImage(0, 0, "assets/images/bouncing ball/setttings.png");
    // Title
    iSetColor(0, 191, 255); // Ocean blue
    drawCenteredText(0, 650, GAME_WINDOW_WIDTH, 48, "SETTINGS", "assets/fonts/arial.ttf", 48);
    // Sound Settings
    iSetColor(135, 206, 250); // Light sky blue
    iText(200, 560, "Sound Settings", GLUT_BITMAP_HELVETICA_18);
    // Sound Enable/Disable with hover size effect
    bool isSoundHovering = isPointInRect(mouseX, mouseY, 200, 500, 150, 40);
    int soundWidth = isSoundHovering ? 158 : 150;
    int soundHeight = isSoundHovering ? 44 : 40;
    int soundX = isSoundHovering ? 196 : 200;
    int soundY = isSoundHovering ? 498 : 500;
    iSetColor(soundEnabled ? 0 : 160, soundEnabled ? 102 : 160, soundEnabled ? 204 : 160);
    iFilledRectangle(soundX, soundY, soundWidth, soundHeight);
    iSetColor(255, 255, 255);
    iRectangle(soundX, soundY, soundWidth, soundHeight);
    if (isSoundHovering)
    {
        iSetColor(255, 255, 0); // Yellow text when hovering
    }
    else
    {
        iSetColor(0, 0, 0); // White text normally
    }
    drawCenteredText(soundX, soundY, soundWidth, soundHeight, soundEnabled ? "Sound: ON" : "Sound: OFF", "assets/fonts/arial.ttf", 20);
    // Sound Volume
    iSetColor(255, 255, 255); // Light sky blue
    iText(210, 470, "Sound Volume:", GLUT_BITMAP_HELVETICA_18);
    iSetColor(100, 100, 100);
    iFilledRectangle(380, 465, 200, 20);
    iSetColor(102, 0, 204);
    iFilledRectangle(380, 465, (soundVolume * 200) / 100, 20);
    iSetColor(255, 255, 255);
    iRectangle(380, 465, 200, 20);
    char volumeText[10];
    sprintf(volumeText, "%d%%", soundVolume);
    iText(590, 470, volumeText, GLUT_BITMAP_HELVETICA_12);
    // Music Settings
    iSetColor(135, 206, 250); // Light sky blue
    iText(200, 410, "Music Settings", GLUT_BITMAP_HELVETICA_18);
    // Music Enable/Disable with hover size effect
    bool isMusicHovering = isPointInRect(mouseX, mouseY, 200, 350, 150, 40);
    int musicWidth = isMusicHovering ? 158 : 150;
    int musicHeight = isMusicHovering ? 44 : 40;
    int musicX = isMusicHovering ? 196 : 200;
    int musicY = isMusicHovering ? 348 : 350;
    iSetColor(musicEnabled ? 0 : 160, musicEnabled ? 102 : 160, musicEnabled ? 204 : 160);
    iFilledRectangle(musicX, musicY, musicWidth, musicHeight);
    iSetColor(255, 255, 255);
    iRectangle(musicX, musicY, musicWidth, musicHeight);
    if (isMusicHovering)
    {
        iSetColor(255, 255, 0); // Yellow text when hovering
    }
    else
    {
        iSetColor(0, 0, 0); // White text normally
    }
    drawCenteredText(musicX, musicY, musicWidth, musicHeight, musicEnabled ? "Music: ON" : "Music: OFF", "assets/fonts/arial.ttf", 24);
    // Music Volume
    iSetColor(255, 255, 255);
    iText(210, 315, "Music Volume:", GLUT_BITMAP_HELVETICA_18);
    iSetColor(100, 100, 100);
    iFilledRectangle(380, 315, 200, 20);
    iSetColor(102, 0, 204);
    iFilledRectangle(380, 315, (musicVolume * 200) / 100, 20);
    iSetColor(255, 255, 255);
    iRectangle(380, 315, 200, 20);
    sprintf(volumeText, "%d%%", musicVolume);
    iText(590, 320, volumeText, GLUT_BITMAP_HELVETICA_12);
    // Back button with hover size effect
    bool isBackHovering = isPointInRect(mouseX, mouseY, 50, 50, 100, 40);
    int backWidth = isBackHovering ? 108 : 100;
    int backHeight = isBackHovering ? 44 : 40;
    int backX = isBackHovering ? 46 : 50;
    int backY = isBackHovering ? 48 : 50;
    iSetColor(100, 100, 100);
    iFilledRectangle(backX, backY, backWidth, backHeight);
    iSetColor(255, 255, 255);
    iRectangle(backX, backY, backWidth, backHeight);
    if (isBackHovering)
    {
        iSetColor(255, 255, 0); // Yellow text when hovering
    }
    else
    {
        iSetColor(255, 255, 255); // White text normally
    }
    drawCenteredText(backX, backY, backWidth, backHeight, "Back", "assets/fonts/arial.ttf", 20);
}

void drawTribute()
{
    iClear();
    // Display tribute image
    iShowImage(0, 0, "assets/images/bouncing ball/tribute.png");
    bool isHoveringBack = isPointInRect(mouseX, mouseY, 50, 50, 100, 40);
    // Light colored rectangle when hovering
    if (isHoveringBack)
    {
        iSetColor(255, 255, 255); // White background when hovering
        iFilledRectangle(45, 45, 110, 50);
    }
    // Button background
    int backWidth = isHoveringBack ? 110 : 100;
    int backHeight = isHoveringBack ? 50 : 40;
    int backX = isHoveringBack ? 45 : 50;
    int backY = isHoveringBack ? 45 : 50;
    iSetColor(70, 70, 70);
    iFilledRectangle(backX, backY, backWidth, backHeight);
    iSetColor(255, 255, 255);
    iRectangle(backX, backY, backWidth, backHeight);
    // Button text
    if (isHoveringBack)
    {
        iSetColor(255, 255, 0); // Yellow text when hovering
    }
    else
    {
        iSetColor(255, 255, 255); // White text normally
    }
    iText(backX + 30, backY + 20, "Back", GLUT_BITMAP_HELVETICA_12);
}

void drawPauseMenu()
{
    iClear();
    // Display background image
    iShowImage(0, 0, "assets/images/bouncing ball/setttings.png");
    // Menu background with navy theme
    iSetColor(20, 30, 60);
    iFilledRectangle(200, 200, 400, 400);
    iSetColor(100, 150, 200);
    iRectangle(200, 200, 400, 400);
    // Inner border for elegance
    iSetColor(150, 180, 220);
    iRectangle(210, 210, 380, 380);
    // Title with navy theme - Ocean blue color (perfectly centered)
    iSetColor(0, 191, 255); // Deep sky blue
    drawCenteredText(200, 530, 400, 50, "GAME PAUSED", "assets/fonts/arial.ttf", 36);
    // Menu buttons (3 buttons now: Resume, Sound, Exit) - all centered
    int buttonWidth = 250;
    int buttonHeight = 60;
    int buttonSpacing = 80;
    int startY = 440;
    int centerX = 275; // This centers the buttons horizontally
    // Resume button (green like main menu)
    iSetColor(102, 0, 204);
    iFilledRectangle(centerX, startY, buttonWidth, buttonHeight);
    iSetColor(135, 206, 250); // Light sky blue text
    iRectangle(centerX, startY, buttonWidth, buttonHeight);
    // Inner highlight
    iSetColor(80, 180, 80);
    iRectangle(centerX + 5, startY + 5, buttonWidth - 10, buttonHeight - 10);
    iSetColor(0, 0, 0); // White text for contrast - centered
    drawCenteredText(centerX, startY, buttonWidth, buttonHeight, "Resume", "assets/fonts/arial.ttf", 28);
    // Sound button (dynamic color based on state)
    if (soundEnabled)
    {
        iSetColor(127, 0, 255); // Blue when ON
    }
    else
    {
        iSetColor(204, 153, 255); // Dark red when OFF
    }
    iFilledRectangle(centerX, startY - buttonSpacing, buttonWidth, buttonHeight);
    iSetColor(135, 206, 250); // Light sky blue border
    iRectangle(centerX, startY - buttonSpacing, buttonWidth, buttonHeight);
    // Inner highlight
    if (soundEnabled)
    {
        iSetColor(80, 150, 210);
    }
    else
    {
        iSetColor(150, 90, 90);
    }
    iRectangle(centerX + 5, startY - buttonSpacing + 5, buttonWidth - 10, buttonHeight - 10);
    iSetColor(0, 0, 0); // Ocean blue text - centered
    drawCenteredText(centerX, startY - buttonSpacing, buttonWidth, buttonHeight,
                     soundEnabled ? "Sound: ON" : "Sound: OFF", "assets/fonts/arial.ttf", 24);

    // Exit button (red like main menu)
    iSetColor(153, 51, 255);
    iFilledRectangle(centerX, startY - 2 * buttonSpacing, buttonWidth, buttonHeight);
    iSetColor(135, 206, 250); // Light sky blue border
    iRectangle(centerX, startY - 2 * buttonSpacing, buttonWidth, buttonHeight);
    // Inner highlight
    iSetColor(180, 80, 80);
    iRectangle(centerX + 5, startY - 2 * buttonSpacing + 5, buttonWidth - 10, buttonHeight - 10);
    iSetColor(0, 0, 0); // White text for contrast - centered
    drawCenteredText(centerX + 30, startY - 2 * buttonSpacing, buttonWidth, buttonHeight, "Exit to Levels", "assets/fonts/arial.ttf", 24);
}

void drawGameOver()
{
    // iClear();
    // // Navy gradient background with stars - extended to cover full window
    // for (int i = 0; i < GAME_WINDOW_HEIGHT + 100; i += 4)
    // {
    //     // Deep navy gradient: dark navy blue to midnight blue
    //     int blueComponent = 15 + (i * 35) / (GAME_WINDOW_HEIGHT + 100); // 15 to 50 (navy blue)
    //     int greenComponent = 5 + (i * 15) / (GAME_WINDOW_HEIGHT + 100); // 5 to 20 (slight green tint)
    //     int redComponent = 2 + (i * 8) / (GAME_WINDOW_HEIGHT + 100);    // 2 to 10 (minimal red)
    //     iSetColor(redComponent, greenComponent, blueComponent);
    //     // Draw 4 lines at once to fill gaps
    //     iLine(0, i, GAME_WINDOW_WIDTH, i);
    //     if (i + 1 < GAME_WINDOW_HEIGHT + 100)
    //         iLine(0, i + 1, GAME_WINDOW_WIDTH, i + 1);
    //     if (i + 2 < GAME_WINDOW_HEIGHT + 100)
    //         iLine(0, i + 2, GAME_WINDOW_WIDTH, i + 2);
    //     if (i + 3 < GAME_WINDOW_HEIGHT + 100)
    //         iLine(0, i + 3, GAME_WINDOW_WIDTH, i + 3);
    // }
    // // Beautiful stars
    // for (int i = 0; i < 15; i++)
    // {
    //     int x = (i * 127 + 31) % GAME_WINDOW_WIDTH;
    //     int y = (i * 89 + 53) % (GAME_WINDOW_HEIGHT + 100);

    //     int starBrightness = i % 3;
    //     if (starBrightness == 0)
    //     {
    //         iSetColor(160, 170, 200);
    //         iFilledCircle(x, y, 1);
    //     }
    //     else if (starBrightness == 1)
    //     {
    //         iSetColor(120, 130, 160);
    //         iFilledCircle(x, y, 1);
    //     }
    //     else
    //     {
    //         iSetColor(80, 90, 120);
    //         iFilledCircle(x, y, 1);
    //     }
    // }
    iClear();
    iShowImage(0, 0, "assets/images/bouncing ball/setttings.png");
    // Game Over background
    iSetColor(40, 20, 20); // Dark red background
    iFilledRectangle(150, 200, 500, 400);
    iSetColor(200, 100, 100); // Light red border
    iRectangle(150, 200, 500, 400);
    // Inner border for elegance
    iSetColor(220, 150, 150);
    iRectangle(160, 210, 480, 380);
    // Title - red color for game over
    iSetColor(255, 100, 100); // Light red
    drawCenteredText(160, 530, 500, 50, "LEVEL FAILED", "assets/fonts/arial.ttf", 42);
    // Subtitle - check if failed due to balls or bottom limit
    iSetColor(255, 200, 200);
    if (ballsRemaining <= 0)
    {
        char ballMessage[100];
        sprintf(ballMessage, "Out of balls! You had %d balls for this level.", maxBallsForLevel);
        drawCenteredText(250, 480, 500, 30, ballMessage, "assets/fonts/arial.ttf", 18); // changed 10:12 AM 150 -- > 250
    }
    else
    {
        drawCenteredText(220, 480, 500, 30, "Balls reached the bottom limit!", "assets/fonts/arial.ttf", 20);
    }
    // Menu buttons - centered
    int buttonWidth = 200;
    int buttonHeight = 50;
    int buttonSpacing = 70;
    int startY = 420;
    int centerX = 300; // This centers the buttons horizontally
    // New Game button (green)
    bool isNewGameHovering = isPointInRect(mouseX, mouseY, centerX, startY, buttonWidth, buttonHeight);
    int newGameWidth = isNewGameHovering ? buttonWidth + 8 : buttonWidth;
    int newGameHeight = isNewGameHovering ? buttonHeight + 4 : buttonHeight;
    int newGameX = isNewGameHovering ? centerX - 4 : centerX;
    int newGameY = isNewGameHovering ? startY - 2 : startY;
    iSetColor(0, 0, 153);
    iFilledRectangle(newGameX, newGameY, newGameWidth, newGameHeight);
    iSetColor(0, 255, 255);
    iRectangle(newGameX, newGameY, newGameWidth, newGameHeight);

    if (isNewGameHovering)
    {
        iSetColor(204, 102, 0); // Yellow text when hovering
    }
    else
    {
        iSetColor(0, 0, 0); // White text normally
    }
    drawCenteredText(newGameX, newGameY, newGameWidth, newGameHeight, "New Game", "assets/fonts/arial.ttf", 24);

    // Back button (blue)
    bool isBackHovering = isPointInRect(mouseX, mouseY, centerX, startY - buttonSpacing, buttonWidth, buttonHeight);
    int backWidth = isBackHovering ? buttonWidth + 8 : buttonWidth;
    int backHeight = isBackHovering ? buttonHeight + 4 : buttonHeight;
    int backX = isBackHovering ? centerX - 4 : centerX;
    int backY = isBackHovering ? (startY - buttonSpacing - 2) : (startY - buttonSpacing);
    iSetColor(0, 0, 204);
    iFilledRectangle(backX, backY, backWidth, backHeight);
    iSetColor(0, 255, 255);
    iRectangle(backX, backY, backWidth, backHeight);
    if (isBackHovering)
    {
        iSetColor(204, 102, 0); // Yellow text when hovering
    }
    else
    {
        iSetColor(0, 0, 0); // White text normally
    }
    drawCenteredText(backX, backY, backWidth, backHeight, "Back", "assets/fonts/arial.ttf", 24);
}

void drawLevelComplete()
{
    iClear();
    // Display background image
    iShowImage(0, 0, "assets/images/bouncing ball/setttings.png");
    // Level Complete background
    iSetColor(0, 51, 102); // Dark green background
    iFilledRectangle(150, 200, 500, 400);
    iSetColor(51, 153, 255); // Light green border
    iRectangle(150, 200, 500, 400);
    // Inner border
    iSetColor(51, 153, 255);
    iRectangle(160, 210, 480, 380);

    // Title - green color for success
    iSetColor(204, 204, 0);
    drawCenteredText(150 + 10, 530, 500, 50, "LEVEL COMPLETED!", "assets/fonts/arial.ttf", 42);
    // Subtitle
    iSetColor(255, 255, 255);
    char levelText[50];
    sprintf(levelText, "Level %d completed successfully!", currentLevel);
    drawCenteredText(150 + 54, 480, 500, 30, levelText, "assets/fonts/arial.ttf", 20);
    // Menu buttons - centered
    int buttonWidth = 200;
    int buttonHeight = 50;
    int buttonSpacing = 70;
    int startY = 420;
    int centerX = 300;
    // Next Level button (gold)
    bool isNextLevelHovering = isPointInRect(mouseX, mouseY, centerX, startY, buttonWidth, buttonHeight);
    int nextLevelWidth = isNextLevelHovering ? buttonWidth + 8 : buttonWidth;
    int nextLevelHeight = isNextLevelHovering ? buttonHeight + 4 : buttonHeight;
    int nextLevelX = isNextLevelHovering ? centerX - 4 : centerX;
    int nextLevelY = isNextLevelHovering ? startY - 2 : startY;
    if (currentLevel < 20)
    {
        iSetColor(76, 0, 153); // Gold background
        iFilledRectangle(nextLevelX, nextLevelY, nextLevelWidth, nextLevelHeight);
        iSetColor(178, 102, 255); // Gold border
        iRectangle(nextLevelX, nextLevelY, nextLevelWidth, nextLevelHeight);

        if (isNextLevelHovering)
        {
            iSetColor(255, 255, 0); // Yellow text when hovering
        }
        else
        {
            iSetColor(0, 0, 0); // White text normally
        }
        drawCenteredText(nextLevelX + 15, nextLevelY, nextLevelWidth, nextLevelHeight, "Next Level", "assets/fonts/arial.ttf", 24);
    }
    // Back button (blue)
    bool isBackHovering = isPointInRect(mouseX, mouseY, centerX, startY - buttonSpacing, buttonWidth, buttonHeight);
    int backWidth = isBackHovering ? buttonWidth + 8 : buttonWidth;
    int backHeight = isBackHovering ? buttonHeight + 4 : buttonHeight;
    int backX = isBackHovering ? centerX - 4 : centerX;
    int backY = isBackHovering ? (startY - buttonSpacing - 2) : (startY - buttonSpacing);
    iSetColor(102, 0, 204);
    iFilledRectangle(backX, backY, backWidth, backHeight);
    iSetColor(178, 102, 255);
    iRectangle(backX, backY, backWidth, backHeight);
    if (isBackHovering)
    {
        iSetColor(255, 255, 0);
    }
    else
    {
        iSetColor(0, 0, 0);
    }
    drawCenteredText(backX + 24, backY, backWidth, backHeight, "Back to Levels", "assets/fonts/arial.ttf", 24);
}
void setColorByNumber(int color)
{
    switch (color)
    {
    case 1:
        iSetColor(255, 0, 0);
        break; // Red
    case 2:
        iSetColor(0, 255, 0);
        break; // Green
    case 3:
        iSetColor(255, 255, 0);
        break; // Yellow
    case 4:
        iSetColor(0, 0, 255);
        break; // Blue
    case 5:
        iSetColor(255, 0, 255);
        break; // Pink
    default:
        iSetColor(0, 0, 0);
        break; // Black
    }
}

void drawBombIcon(double x, double y, bool isSelected)
{
    // Draw the bomb base (black circle)
    iSetColor(30, 30, 30);
    iFilledCircle(x, y, BUBBLE_RADIUS);

    // Draw bomb fuse
    iSetColor(150, 75, 0);
    iFilledRectangle(x - 2, y + BUBBLE_RADIUS - 2, 4, 10);

    // Draw fuse spark
    if (isSelected)
    {
        // Animated spark when selected
        iSetColor(255, 200, 0);
    }
    else
    {
        // Static spark otherwise
        iSetColor(255, 120, 0);
    }
    iFilledCircle(x, y + BUBBLE_RADIUS + 10, 4);

    // Highlight reflection
    iSetTransparentColor(255, 255, 255, 0.3);
    iFilledEllipse(x - 5, y + 5, 8, 4);
}

void DrawShootedBall(double x, double y, int color)
{
    if (color == -1)
    {
        drawBombIcon(x, y, true);
        return;
    }
    // Step 1: Main base color
    int r = 0, g = 0, b = 0;
    switch (color)
    {
    case 1:
        r = 204;
        g = 0;
        b = 0;
        break; // Red
    case 2:
        r = 0;
        g = 255;
        b = 0;
        break; // Green
    case 3:
        r = 255;
        g = 255;
        b = 0;
        break; // Yellow
    case 4:
        r = 0;
        g = 0;
        b = 210;
        break; // Blue
    case 5:
        r = 255;
        g = 0;
        b = 127;
        break; // Pink
    default:
        r = 0;
        g = 0;
        b = 0;
        break; // black
    }

    if (r != 0 || g != 0 || b != 0)
    {
        iSetColor(r, g, b);
        iFilledCircle(x, y, BUBBLE_RADIUS);
        // Step 2: Simulated radial shading (darker inner rings)
        for (int rad = BUBBLE_RADIUS - 6; rad > BUBBLE_RADIUS / 2; rad -= 2)
        {
            int factor = (BUBBLE_RADIUS - rad) * 10; // shadow intensity
            int dr = max(0, r - factor);
            int dg = max(0, g - factor);
            int db = max(0, b - factor);
            iSetColor(dr, dg, db);
            iFilledCircle(x, y, rad);
        }
        // Step 3: Highlight glare (ellipse + dot)
        iSetTransparentColor(240, 250, 255, 0.63); // Soft white ellipse
        iFilledEllipse(x - BUBBLE_RADIUS / 3, y + BUBBLE_RADIUS / 3, BUBBLE_RADIUS / 4, BUBBLE_RADIUS / 5);

        iSetTransparentColor(255, 255, 255, 0.79); // Solid white dot
        iFilledCircle(x - BUBBLE_RADIUS / 5, y + BUBBLE_RADIUS / 2, BUBBLE_RADIUS / 8);
    }
}
void drawStyledBall(double x, double y, int color)
{
    iScale(x, y, 1.0, 1.0);
    // Step 1: Main base color
    int r = 0, g = 0, b = 0;
    switch (color)
    {
    case 1:
        r = 204;
        g = 0;
        b = 0;
        break; // Red
    case 2:
        r = 0;
        g = 255;
        b = 0;
        break; // Green
    case 3:
        r = 255;
        g = 255;
        b = 0;
        break; // Yellow
    case 4:
        r = 0;
        g = 0;
        b = 210;
        break; // Blue
    case 5:
        r = 255;
        g = 0;
        b = 127;
        break; // Pink
    default:
        r = 0;
        g = 0;
        b = 0;
        break; // black
    }
    if (r != 0 || g != 0 || b != 0)
    {
        iSetColor(r, g, b);
        iFilledCircle(x, y, BUBBLE_RADIUS);
        // Step 2: Simulated radial shading (darker inner rings)
        for (int rad = BUBBLE_RADIUS - 2; rad > BUBBLE_RADIUS / 2; rad -= 2)
        {
            int factor = (BUBBLE_RADIUS - rad) * 10; // shadow intensity

            int dr = max(0, r - factor);
            int dg = max(0, g - factor);
            int db = max(0, b - factor);
            iSetColor(dr, dg, db);
            iFilledCircle(x, y, rad);
        }
        // Step 3: Highlight glare (ellipse + dot)
        iSetTransparentColor(240, 250, 255, 0.63); // Soft white ellipse
        iFilledEllipse(x - BUBBLE_RADIUS / 3, y + BUBBLE_RADIUS / 3, BUBBLE_RADIUS / 4, BUBBLE_RADIUS / 5);
        iSetTransparentColor(255, 255, 255, 0.79); // Solid white dot
        iFilledCircle(x - BUBBLE_RADIUS / 5, y + BUBBLE_RADIUS / 2, BUBBLE_RADIUS / 8);
    }
    iUnScale();
}

void drawStyledBallWithScale(double x, double y, int color, double scale)
{
    // Step 1: Main base color
    int r = 0, g = 0, b = 0;
    switch (color)
    {
    case 1:
        r = 204;
        g = 0;
        b = 0;
        break; // Red
    case 2:
        r = 0;
        g = 255;
        b = 0;
        break; // Green
    case 3:
        r = 255;
        g = 255;
        b = 0;
        break; // Yellow
    case 4:
        r = 0;
        g = 0;
        b = 210;
        break; // Blue
    case 5:
        r = 255;
        g = 0;
        b = 127;
        break; // Pink
    default:
        r = 0;
        g = 0;
        b = 0;
        break; // black
    }
    if (r != 0 || g != 0 || b != 0)
    {
        // Apply scale to bubble radius
        int scaledRadius = (int)(BUBBLE_RADIUS * scale);

        iSetColor(r, g, b);
        iFilledCircle(x, y, scaledRadius);

        // Step 2: Simulated radial shading (darker inner rings)
        for (int rad = scaledRadius - 2; rad > scaledRadius / 2; rad -= 2)
        {
            int factor = (scaledRadius - rad) * 10; // shadow intensity
            int dr = max(0, r - factor);
            int dg = max(0, g - factor);
            int db = max(0, b - factor);
            iSetColor(dr, dg, db);
            iFilledCircle(x, y, rad);
        }
        // Step 3: Highlight glare (ellipse + dot) - also scaled
        iSetTransparentColor(240, 250, 255, 0.63); // Soft white ellipse
        iFilledEllipse(x - scaledRadius / 3, y + scaledRadius / 3,
                       (int)(scaledRadius / 4 * scale), (int)(scaledRadius / 5 * scale));
        iSetTransparentColor(255, 255, 255, 0.79); // Solid white dot
        iFilledCircle(x - scaledRadius / 5, y + scaledRadius / 2, (int)(scaledRadius / 8 * scale));
    }
}

void getGridBallCenter(int r, int c, double *x, double *y)
{
    double xShift = (r % 2 == 0) ? 0 : BUBBLE_RADIUS;
    *x = c * 2 * BUBBLE_RADIUS + BUBBLE_RADIUS + xShift;
    *y = GAME_WINDOW_HEIGHT - r * rowHeight - BUBBLE_RADIUS;
    // Apply velocity-based animation offset during row drop
    if (isAnimatingRowDrop)
    {
        *y -= gridVerticalOffset;
    }
}
void getGridRowColumn(double x, double y, int *r, int *c)
{
    *r = (int)((GAME_WINDOW_HEIGHT - y) / rowHeight);
    double xShift = (*r % 2 == 0) ? 0 : BUBBLE_RADIUS;
    *c = (int)((x - xShift) / (2 * BUBBLE_RADIUS));
}
int checkCollision(double x1, double y1, double x2, double y2)
{
    double dist_sq = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
    return dist_sq <= (2 * BUBBLE_RADIUS - 2) * (2 * BUBBLE_RADIUS - 2);
}
void placeBallOnGrid(double x, double y, int color)
{
    int r, c;
    getGridRowColumn(x, y, &r, &c);
    if (r < 0)
        r = 0;
    if (r >= ROWS)
        r = ROWS - 1; // Don't expand beyond ROWS limit
    int maxCols = (r % 2 == 0) ? COLS : (COLS - 1);
    if (c < 0)
        c = 0;
    if (c >= maxCols)
        c = maxCols - 1;
    // Extra bounds check to prevent array overflow
    if (r >= 0 && r < ROWS && c >= 0 && c < COLS)
    {
        if (color == -1)
        { // Bomb
            explodeBomb(r, c);
        }
        else if (grid[r][c] == 0)
        {
            grid[r][c] = color;

            // Check for game over after placing the ball
            if (checkGameOver())
            {
                inGameOver = true;
            }
        }
    }
}
int Marked[ROWS][COLS] = {0};
int matched[ROWS * COLS][2] = {0};
int BallMatchCount = 0;
void resetMarked()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            Marked[i][j] = 0;
        }
    }
}
// 6 directions --> row / column incrementing or decrementing
// Hexagonal grid neighbors: Even rows (not offset), Odd rows (offset right by 0.5)
int AddRowEven[6] = {-1, -1, 0, 0, 1, 1};
int AddColumnEven[6] = {-1, 0, -1, 1, -1, 0};
int AddRowOdd[6] = {-1, -1, 0, 0, 1, 1};
int AddColumnOdd[6] = {0, 1, -1, 1, 0, 1};
void search6directionsFrom(int r, int c, int color)
{
    // 3 base cases for recursion to end
    // reaching the ceiling or the bottom
    if (r < 0 || r >= ROWS)
        return;
    // reaching the left or the right wall - fix for hexagonal grid
    int maxCols = (r % 2 == 0) ? COLS : (COLS - 1);
    if (c < 0 || c >= maxCols)
        return;
    // Check if already marked
    if (Marked[r][c])
        return;
    // Check if empty cell
    if (grid[r][c] == 0)
        return;
    // Check if color doesn't match
    if (grid[r][c] != color)
        return;
    // Mark this cell and add to matches
    Marked[r][c] = 1;
    matched[BallMatchCount][0] = r;
    matched[BallMatchCount][1] = c;
    BallMatchCount++;
    // Get the appropriate neighbor offsets for this row
    int *AddToRow = (r % 2 == 0) ? AddRowEven : AddRowOdd;
    int *AddToColumn = (r % 2 == 0) ? AddColumnEven : AddColumnOdd;
    // Check all 6 neighbors
    for (int i = 0; i < 6; i++)
    {
        int newR = r + AddToRow[i];
        int newC = c + AddToColumn[i];
        // Only recursively search if the new position is valid
        if (newR >= 0 && newR < ROWS)
        {
            int newMaxCols = (newR % 2 == 0) ? COLS : (COLS - 1);
            if (newC >= 0 && newC < newMaxCols)
            {
                search6directionsFrom(newR, newC, color);
            }
        }
    }
}

void PopBubblesInside(int r, int c)
{
    // Bounds check first
    if (r < 0 || r >= ROWS || c < 0 || c >= COLS)
        return;
    resetMarked();
    BallMatchCount = 0;
    int ShootedBallColor = grid[r][c];
    // Debug: Check if the ball was placed correctly
    if (ShootedBallColor == 0)
    {
        return; // No ball at this position
    }
    search6directionsFrom(r, c, ShootedBallColor);
    if (BallMatchCount >= 3)
    {
        ScoreIncrement = (BallMatchCount * 10);
        int oldScore = Score;
        Score += ScoreIncrement;
        ScoreIncrementingInstant = iGetTimer();
        ShowIncrement = 1;

        // Start the pop animation instead of immediately removing bubbles
        startPopAnimation(matched, BallMatchCount);

        // Check if we crossed a bomb threshold (every 200 points)
        int oldBombThreshold = oldScore / POINTS_PER_BOMB;
        int newBombThreshold = Score / POINTS_PER_BOMB;
        if (newBombThreshold > oldBombThreshold)
        {
            bombCount++; // Award a new bomb
            if (soundEnabled)
            {
                iPlaySound("assets/sounds/bomb2.wav", false, soundVolume); // Special sound for getting a bomb
            }
        }

        // Play pop sound when bubbles are popped (once per group)
        if (soundEnabled && BallMatchCount > 0)
        {
            iPlaySound("assets/sounds/pop.mp3", false, soundVolume);
        }
        // Level completion will be checked after animations complete
    }
}
void ShowScore()
{
    // Ultra-simple top bar for maximum performance
    iSetColor(15, 25, 45);
    iFilledRectangle(0, GAME_WINDOW_HEIGHT, GAME_WINDOW_WIDTH, 50);
    // Score text (no background panels)
    iSetColor(255, 255, 255);
    iText(15, GAME_WINDOW_HEIGHT + 14, "SCORE : ", GLUT_BITMAP_HELVETICA_18);
    char scoreText[30];
    sprintf(scoreText, "%d", Score);
    iSetColor(255, 255, 100);
    iText(150, GAME_WINDOW_HEIGHT + 14, scoreText, GLUT_BITMAP_HELVETICA_18);
    // Ball count display
    iSetColor(255, 255, 255);
    iText(300, GAME_WINDOW_HEIGHT + 14, "BALLS : ", GLUT_BITMAP_HELVETICA_18);
    char ballText[30];
    sprintf(ballText, "%d", ballsRemaining);
    // Change color based on remaining balls
    if (ballsRemaining <= 5)
        iSetColor(255, 100, 100); // Red when low
    else if (ballsRemaining <= 10)
        iSetColor(255, 255, 100); // Yellow when medium
    else
        iSetColor(100, 255, 100); // Green when plenty
    iText(420, GAME_WINDOW_HEIGHT + 14, ballText, GLUT_BITMAP_HELVETICA_18);
} // <Cluster Detection and Fallings>
int visited[ROWS][COLS] = {0};
void addToFloatingBubblesArray(int r, int c, int color)
{
    if (fallingBubbleCount >= 100)
        return;
    double x, y;
    getGridBallCenter(r, c, &x, &y);
    fallingBubbles[fallingBubbleCount].x = x;
    fallingBubbles[fallingBubbleCount].y = y;
    fallingBubbles[fallingBubbleCount].vy = 0;
    fallingBubbles[fallingBubbleCount].color = color;
    fallingBubbles[fallingBubbleCount].isFalling = 1;
    fallingBubbleCount++;
}
void BFS_from_top_row(int row, int col)
{
    if (row < 0 || row >= ROWS)
        return;
    int maxCols = (row % 2 == 0) ? COLS : (COLS - 1);
    if (col < 0 || col >= maxCols)
        return;
    if (grid[row][col] == 0 || visited[row][col])
        return;
    visited[row][col] = 1;
    int *AddToRow = (row % 2 == 0) ? (AddRowEven) : (AddRowOdd);
    int *AddToColumn = (row % 2 == 0) ? (AddColumnEven) : (AddColumnOdd);
    for (int i = 0; i < 6; i++)
    {
        int R = row + *(AddToRow + i);
        int C = col + *(AddToColumn + i);
        BFS_from_top_row(R, C);
    }
}

void visitConnectedGraphFromTop()
{
    memset(visited, 0, sizeof(visited));
    for (int i = 0; i < COLS; i++)
    {
        if (grid[0][i] && !visited[0][i])
            BFS_from_top_row(0, i);
    }
}

void explodeBomb(int centerRow, int centerCol)
{
    // Play explosion sound effect if sound is enabled
    if (soundEnabled)
    {
        iPlaySound("assets/sounds/bomb.wav", false, soundVolume);
    }

    // Define a 3x3 area to explode
    for (int dr = -2; dr <= 2; dr++)
    {
        for (int dc = -2; dc <= 2; dc++)
        {
            int newRow = centerRow + dr;
            int newCol = centerCol + dc;

            // Check if the position is within grid bounds
            if (newRow >= 0 && newRow < ROWS)
            {
                int maxCols = (newRow % 2 == 0) ? COLS : (COLS - 1);
                if (newCol >= 0 && newCol < maxCols)
                {
                    // Start pop animation for bubble if it exists
                    if (grid[newRow][newCol] != 0)
                    {
                        // Add this bubble to popping bubbles array
                        if (poppingBubbleCount < 50)
                        { // Make sure we don't exceed array bounds
                            poppingBubbles[poppingBubbleCount].row = newRow;
                            poppingBubbles[poppingBubbleCount].col = newCol;
                            poppingBubbles[poppingBubbleCount].color = grid[newRow][newCol];
                            poppingBubbles[poppingBubbleCount].startTime = iGetTimer() + (poppingBubbleCount * POP_STAGGER_DELAY);
                            poppingBubbles[poppingBubbleCount].isActive = true;
                            poppingBubbleCount++;
                        }
                        grid[newRow][newCol] = 0;
                    }
                }
            }
        }
    }

    // After explosion, detect any floating bubbles
    detectCluster();
}

void detectCluster()
{
    visitConnectedGraphFromTop();
    for (int i = 0; i < ROWS; i++)
    {
        int colsInRow = (i % 2 == 0) ? COLS : (COLS - 1);
        for (int j = 0; j < colsInRow; j++)
        {
            if (grid[i][j] && !visited[i][j])
            { // a single floating bubble detected
                addToFloatingBubblesArray(i, j, grid[i][j]);
                grid[i][j] = 0;
                Score += 10;
            }
        }
    }
    // Check if level is completed after removing floating bubbles
    if (checkLevelComplete())
    {
        handleLevelComplete();
    }
}
// </Cluster Detection and Falling>
void ballMovement()
{
    for (int i = 0; i < fallingBubbleCount; i++)
    {
        if (fallingBubbles[i].isFalling)
        {
            fallingBubbles[i].y -= fallingBubbles[i].vy;
            fallingBubbles[i].vy += 0.8; // Increased gravity for faster falling
            if (fallingBubbles[i].y >= 800)
                fallingBubbles[i].isFalling = 0;
        }
    }
    if (!isBallMoving)
        return;
    ball_x += vx;
    ball_y += vy;
    if (ball_x <= BUBBLE_RADIUS)
    {
        ball_x = BUBBLE_RADIUS;
        vx = -vx;
    }
    else if (ball_x >= GAME_WINDOW_WIDTH - BUBBLE_RADIUS)
    {
        ball_x = GAME_WINDOW_WIDTH - BUBBLE_RADIUS;
        vx = -vx;
    }
    // Smart top wall collision logic
    if (ball_y >= GAME_WINDOW_HEIGHT - BUBBLE_RADIUS)
    {
        ball_y = GAME_WINDOW_HEIGHT - BUBBLE_RADIUS;
        // Check if all hidden rows are used (all rows visible)
        if (currentLevelDisplayOffset <= 1) // All rows visible when <= 1 (need 2 to drop)
        {
            // All rows are visible - ball should stick to top wall
            isBallMoving = false;
            ballBounceCount = 0;
            placeBallOnGrid(ball_x, ball_y, shootingBallColorNow);
            shootingBallColorNow = shootingBallColorNext;
            shootingBallColorNext = getRandomLevelColor();
            // Debug: Ensure valid colors
            if (shootingBallColorNow == 0)
                shootingBallColorNow = 1;
            if (shootingBallColorNext == 0)
                shootingBallColorNext = 2;

            int row, col;
            getGridRowColumn(ball_x, ball_y, &row, &col);
            PopBubblesInside(row, col);
            detectCluster();
            // Don't call checkForRowDrop() here since ball sticking to top wall should not trigger row drop
            if (checkLevelComplete())
            {
                handleLevelComplete();
            }
        }
        else
        {
            // Hidden rows still available - ball bounces back down
            vy = -vy; // Reverse Y velocity to bounce down
            // Update ball colors for the next shot since this ball is still moving
            shootingBallColorNext = getRandomLevelColor();
            // Debug: Ensure valid colors
            if (shootingBallColorNext == 0)
                shootingBallColorNext = 2;
            // Don't call checkForRowDrop() here - wait until ball actually stops moving
        }
        return;
    }
    // Check if ball falls below the bottom of the screen
    if (ball_y <= -BUBBLE_RADIUS)
    {
        // Ball fell off the bottom - end movement and prepare new ball
        isBallMoving = false;
        ballBounceCount = 0;
        shootingBallColorNow = shootingBallColorNext;
        shootingBallColorNext = getRandomLevelColor();
        // Debug: Ensure valid colors
        if (shootingBallColorNow == 0)
            shootingBallColorNow = 1;
        if (shootingBallColorNext == 0)
            shootingBallColorNext = 2;
        checkForRowDrop();
        return;
    }
    for (int r = ROWS - 1; r >= 0; r--)
    {
        int colsInRow = (r % 2 == 0) ? COLS : (COLS - 1);
        for (int c = 0; c < colsInRow; c++)
        {
            if (grid[r][c] != 0)
            {
                double cx, cy;
                getGridBallCenter(r, c, &cx, &cy);
                if (checkCollision(ball_x, ball_y, cx, cy))
                {
                    isBallMoving = false;
                    ballBounceCount = 0; // Reset bounce counter on successful attachment
                    placeBallOnGrid(ball_x, ball_y, shootingBallColorNow);
                    shootingBallColorNow = shootingBallColorNext;
                    shootingBallColorNext = getRandomLevelColor();
                    // Debug: Ensure valid colors
                    if (shootingBallColorNow == 0)
                        shootingBallColorNow = 1;
                    if (shootingBallColorNext == 0)
                        shootingBallColorNext = 2;
                    int row, col;
                    getGridRowColumn(ball_x, ball_y, &row, &col);
                    PopBubblesInside(row, col);
                    // check floating bubbles
                    detectCluster();
                    // Check if we need to drop a new row after this move
                    checkForRowDrop();
                    // Check if level is completed
                    if (checkLevelComplete())
                    {
                        handleLevelComplete();
                    }
                    else
                    {
                        // Check if player ran out of balls
                        if (ballsRemaining <= 0)
                        {
                            // Trigger game over
                            inGameOver = true;
                            inGame = false;
                        }
                    }
                    return;
                }
            }
        }
    }
}
// Function to drop a new row from hidden rows
void dropNewRow()
{
    if (currentLevelDisplayOffset <= 1) // Need at least 2 rows left to drop
        return;
    // Start the velocity-based row drop animation
    isAnimatingRowDrop = true;
    gridVerticalOffset = 0.0;
    targetVerticalOffset = 2 * rowHeight; // Target is 2 rows worth of movement
}
void drawGrid()
{
    for (int r = 0; r < ROWS; r++)
    {
        int colsInRow = (r % 2 == 0) ? COLS : (COLS - 1);
        for (int c = 0; c < colsInRow; c++)
        {
            if (grid[r][c] != 0)
            {
                double x, y;
                getGridBallCenter(r, c, &x, &y);
                // Add subtle glow effect for new rows during row drop animation
                if (isAnimatingRowDrop && (r == 0 || r == 1))
                {
                    double glowIntensity = 1.0 - (gridVerticalOffset / targetVerticalOffset);
                    if (glowIntensity > 0)
                    {
                        // Draw a subtle glow behind the new balls
                        iSetTransparentColor(255, 255, 200, 0.3 * glowIntensity);
                        iFilledCircle(x, y, BUBBLE_RADIUS + 4);
                    }
                }
                // Check if this bubble is currently popping
                bool isPopping = false;
                double popScale = 1.0;
                for (int i = 0; i < poppingBubbleCount; i++)
                {
                    if (poppingBubbles[i].isActive &&
                        poppingBubbles[i].row == r &&
                        poppingBubbles[i].col == c)
                    {
                        isPopping = true;
                        popScale = getPopScale(i);
                        break;
                    }
                }
                // Only draw if not popping or still has some scale
                if (!isPopping || popScale > 0.1)
                {
                    if (isPopping)
                    {
                        // Draw with animated scale
                        drawStyledBallWithScale(x, y, grid[r][c], popScale);
                    }
                    else
                    {
                        drawStyledBall(x, y, grid[r][c]);
                    }
                }
            }
        }
    }
}

void DrawOutline()
{
    iRectangle(0, 0, GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT);
}

void ShowIncrementAnimation(int PoppedBubbleColor)
{
    switch (PoppedBubbleColor)
    {
    case 1:
        iSetColor(255, 0, 0);
        break; // Red
    case 2:
        iSetColor(0, 255, 0);
        break; // Green
    case 3:
        iSetColor(255, 255, 0);
        break; // Yellow
    case 4:
        iSetColor(0, 0, 255);
        break; // Blue
    case 5:
        iSetColor(255, 0, 255);
        break; // Pink
    default:
        iSetColor(255, 255, 255);
        break; // Black
    }
    if (ShowIncrement)
    {
        int now = iGetTimer();
        if (now - ScoreIncrementingInstant < 1000)
        {
            char IncrementedScore[20];
            sprintf(IncrementedScore, "+%d", ScoreIncrement);
            iText(500, 300, IncrementedScore, GLUT_BITMAP_HELVETICA_18);
        }
        else
            ShowIncrement = false;
    }
}

void iDraw()
{
    iClear();
    if (inHomepage)
    {
        drawHomepage();
    }
    else if (inGameOver)
    {
        drawGameOver();
    }
    else if (inLevelComplete)
    {
        drawLevelComplete();
    }
    else if (inSettings)
    {
        drawSettings();
    }
    else if (inTribute)
    {
        drawTribute();
    }
    else if (inInstruction)
    {
        drawInstruction();
    }
    else if (inHighScore)
    {
        drawHighScorePage();
    }
    else if (inNameInput)
    {
        drawNameInputPage();
    }
    else if (inLevelSelect)
    {
        drawLevelSelect();
    }
    else if (inGame && inPauseMenu)
    {
        // Show pause menu as full screen when paused
        drawPauseMenu();
    }
    else if (inGame)
    {
        // Update animations
        updateRowDropAnimation();
        updatePopAnimations(); // Update the pop animations
        // GAMING
        {
            // Optimized navy gradient background (reduced iterations but maintained look)
            for (int i = 0; i < GAME_WINDOW_HEIGHT; i += 4) // Skip 3 lines, draw 4th for performance
            {
                // Deep navy gradient: dark navy blue to midnight blue
                int blueComponent = 15 + (i * 35) / GAME_WINDOW_HEIGHT; // 15 to 50 (navy blue)
                int greenComponent = 5 + (i * 15) / GAME_WINDOW_HEIGHT; // 5 to 20 (slight green tint)
                int redComponent = 2 + (i * 8) / GAME_WINDOW_HEIGHT;    // 2 to 10 (minimal red)
                iSetColor(redComponent, greenComponent, blueComponent);
                // Draw 4 lines at once to fill gaps
                iLine(0, i, GAME_WINDOW_WIDTH, i);
                if (i + 1 < GAME_WINDOW_HEIGHT)
                    iLine(0, i + 1, GAME_WINDOW_WIDTH, i + 1);
                if (i + 2 < GAME_WINDOW_HEIGHT)
                    iLine(0, i + 2, GAME_WINDOW_WIDTH, i + 2);
                if (i + 3 < GAME_WINDOW_HEIGHT)
                    iLine(0, i + 3, GAME_WINDOW_WIDTH, i + 3);
            }

            // Beautiful stars but optimized count (15 stars)
            for (int i = 0; i < 15; i++)
            {
                int x = (i * 127 + 31) % GAME_WINDOW_WIDTH; // Pseudo-random x
                int y = (i * 89 + 53) % GAME_WINDOW_HEIGHT; // Pseudo-random y

                // Simple dot stars with navy-appropriate colors
                int starBrightness = i % 3;
                if (starBrightness == 0)
                {
                    // Dim white-blue stars
                    iSetColor(160, 170, 200);
                    iFilledCircle(x, y, 1);
                }
                else if (starBrightness == 1)
                {
                    // Medium silver stars
                    iSetColor(180, 180, 190);
                    iFilledCircle(x, y, 1);
                }
                else
                {
                    // Bright white stars (rare)
                    iSetColor(220, 220, 230);
                    iFilledCircle(x, y, 1);
                }
            }
            ShowIncrementAnimation(PoppedBubbleColor);
            ShowScore();
            drawGrid();
            DrawOutline();
            // drawing/showing all (stored) falling clustered bubbles
            for (int i = 0; i < fallingBubbleCount; i++)
            {
                if (fallingBubbles[i].isFalling)
                {
                    drawStyledBall(fallingBubbles[i].x, fallingBubbles[i].y, fallingBubbles[i].color);
                }
            }
            // drawing/showing all (stored) falling clustered bubbles
            iEllipse(shooter_x, 40, (shooter_x - 305), (shooter_y - 20));
            // shooter base
            iSetColor(0, 0, 0);
            if (!isBallMoving)
            {
                DrawShootedBall(shooter_x, shooter_y + BUBBLE_RADIUS, shootingBallColorNow);
                // Removed center circles for cleaner look
            }
            else
            {
                DrawShootedBall(ball_x, ball_y, shootingBallColorNow);
                // Removed center circles for cleaner look
            }
            double ang = atan2(mouseY - (shooter_y + BUBBLE_RADIUS), mouseX - shooter_x);
            double canonLength = 80;
            double startX = shooter_x;
            double startY = shooter_y + BUBBLE_RADIUS;
            double endX = startX + canonLength * cos(ang);
            double endY = startY + canonLength * sin(ang);
            if (endY < 110)
                endY = 110;
            // Draw dotted trajectory line to show expected collision with proper physics
            if (mouseY - (shooter_y + BUBBLE_RADIUS) >= 40 && !isBallMoving)
            {
                // Calculate initial velocity components
                double trajVx = cos(ang) * 20; // Use same velocity as actual ball
                double trajVy = sin(ang) * 20;
                // Start from surface of shooting ball, not center
                double trajX = startX + cos(ang) * BUBBLE_RADIUS;
                double trajY = startY + sin(ang) * BUBBLE_RADIUS;
                bool trajectoryComplete = false;
                // Trajectory with proper physics - up to 300 steps for multiple bounces
                for (int totalSteps = 0; totalSteps < 300 && !trajectoryComplete; totalSteps++)
                {
                    trajX += trajVx;
                    trajY += trajVy;
                    // Check left wall collision
                    if (trajX <= BUBBLE_RADIUS)
                    {
                        trajX = BUBBLE_RADIUS;
                        trajVx = -trajVx; // Reverse X velocity
                    }
                    // Check right wall collision
                    else if (trajX >= GAME_WINDOW_WIDTH - BUBBLE_RADIUS)
                    {
                        trajX = GAME_WINDOW_WIDTH - BUBBLE_RADIUS;
                        trajVx = -trajVx; // Reverse X velocity
                    }
                    // Smart top boundary logic - ball always bounces off top wall
                    if (trajY >= GAME_WINDOW_HEIGHT - 5) // When trajectory reaches near the top
                    {
                        // Ball always bounces off top wall
                        trajY = GAME_WINDOW_HEIGHT - 5;
                        trajVy = -trajVy; // Reverse Y velocity to bounce down
                    }

                    // Check bottom boundary (stop trajectory) - only stop if going way too high
                    if (trajY >= GAME_WINDOW_HEIGHT + 50) // Only stop if trajectory goes far beyond screen
                    {
                        trajectoryComplete = true;
                        break;
                    }
                    // Bubble collision check every few steps for performance
                    if (totalSteps % 3 == 0)
                    {
                        bool hit = false;
                        for (int r = 0; r < ROWS && !hit; r++)
                        {
                            int colsInRow = (r % 2 == 0) ? COLS : (COLS - 1);
                            for (int c = 0; c < colsInRow && !hit; c++)
                            {
                                if (grid[r][c] != 0)
                                {
                                    double bubbleX, bubbleY;
                                    getGridBallCenter(r, c, &bubbleX, &bubbleY);
                                    double dist = sqrt((trajX - bubbleX) * (trajX - bubbleX) +
                                                       (trajY - bubbleY) * (trajY - bubbleY));

                                    if (dist <= (2 * BUBBLE_RADIUS - 3))
                                    {
                                        hit = true;
                                        trajectoryComplete = true;
                                    }
                                }
                            }
                        }
                        if (hit)
                            break;
                    }
                    // Draw stylized red dotted line - allow drawing to the very top
                    if (trajX >= 0 && trajX <= GAME_WINDOW_WIDTH &&
                        trajY >= 0 && trajY <= GAME_WINDOW_HEIGHT)
                    {
                        // Create evenly spaced dots: every 2nd step for tight spacing like in the image
                        if (totalSteps % 2 == 0)
                        {
                            // Red color with slight brightness variation for effect
                            int brightness = 200 + (totalSteps % 3) * 20;
                            iSetColor(brightness, 50, 50);
                            // Main dot
                            iFilledCircle(trajX, trajY, 3);
                            // Add subtle glow effect with lighter inner dot
                            iSetColor(255, 100, 100);
                            iFilledCircle(trajX, trajY, 2);
                            // Add tiny white highlight for 3D effect
                            iSetColor(255, 200, 200);
                            iFilledCircle(trajX - 0.5, trajY + 0.5, 1);
                        }
                    }
                }
            }
            drawStyledBall(310, 40, shootingBallColorNext);

            // Draw bomb counter and icon
            iSetColor(255, 255, 255);
            char bombText[30];
            sprintf(bombText, "Bombs : %d", bombCount);
            iText(GAME_WINDOW_WIDTH - 180, 60, bombText, GLUT_BITMAP_HELVETICA_18);

            // Draw bomb icon with glow effect when selected
            if (bombSelected)
            {
                // Glow effect
                iSetColor(255, 200, 0);
                iFilledCircle(GAME_WINDOW_WIDTH - 40, 40, BUBBLE_RADIUS + 4);
            }
            drawBombIcon(GAME_WINDOW_WIDTH - 40, 40, bombSelected);

            // Level text (no background for performance)
            iSetColor(255, 255, 255);
            char levelText[30];
            sprintf(levelText, "Level: %d", currentLevel);
            iText(500, GAME_WINDOW_HEIGHT + 14, levelText, GLUT_BITMAP_HELVETICA_18);
            iSetColor(60, 60, 70);
            iFilledRectangle(650, GAME_WINDOW_HEIGHT + 10, 120, 30);
            iSetColor(255, 255, 255);
            iText(685, GAME_WINDOW_HEIGHT + 18, "Back", GLUT_BITMAP_HELVETICA_12);
        }
    }
}

void iMouseMove(int mx, int my)
{
    mouseX = mx;
    mouseY = my;
}

void iMouse(int button, int state, int mx, int my)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        // Play click sound for every mouse click (only if sound is enabled)
        if (soundEnabled)
        {
            iPlaySound("assets/sounds/click.mp3", false, soundVolume);
        }
        if (inGameOver)
        {
            // New Game button
            if (isPointInRect(mx, my, 300, 420, 200, 50))
            {
                // Start a completely new game from the same level
                inGameOver = false;
                inGame = true;
                startNewGame(); // Restart current level
            }
            // Back button
            else if (isPointInRect(mx, my, 300, 350, 200, 50))
            {
                inGameOver = false;
                inGame = false;
                inLevelSelect = true;
                // Restart background music when returning to level selection if not already playing
                if (backgroundMusicChannel == -1 && musicEnabled)
                {
                    backgroundMusicChannel = iPlaySound("assets/sounds/bg.wav", true, musicVolume);
                }
            }
        }
        else if (inLevelComplete)
        {
            // Next Level button
            if (isPointInRect(mx, my, 300, 420, 200, 50) && currentLevel < 11)
            {
                inLevelComplete = false;
                inGame = true;
                goToNextLevel();
            }
            // Back to Levels button
            else if (isPointInRect(mx, my, 300, 350, 200, 50))
            {
                inLevelComplete = false;
                inLevelSelect = true;
                inGame = false;
                // Restart background music when returning to level selection if not already playing
                if (backgroundMusicChannel == -1 && musicEnabled)
                {
                    backgroundMusicChannel = iPlaySound("assets/sounds/bg.wav", true, musicVolume);
                }
            }
        }
        else if (inHomepage)
        {
            // Enter Game button
            if (isPointInRect(mx, my, 300, 450, 200, 60))
            {
                inHomepage = false;
                inNameInput = true;
                // Reset player name for new game (safely)
                memset(playerName, 0, sizeof(playerName)); // Clear entire buffer
                nameInputPos = 0;
            }
            // Settings button
            else if (isPointInRect(mx, my, 300, 370, 200, 60))
            {
                inHomepage = false;
                inSettings = true;
            }
            // Tribute button
            else if (isPointInRect(mx, my, 300, 290, 200, 60))
            {
                inHomepage = false;
                inTribute = true;
            }
            // High Score button
            else if (isPointInRect(mx, my, 300, 210, 200, 60))
            {
                inHomepage = false;
                inHighScore = true;
                loadHighScores(); // Load high scores when entering the page
            }
            // Exit button
            else if (isPointInRect(mx, my, 300, 130, 200, 60))
            {
                exit(0); // Exit the game
            }
            // Instruction button
            else if (isPointInRect(mx, my, 300, 50, 200, 60))
            {
                inHomepage = false;
                inInstruction = true;
                drawInstruction();
            }
        }
        else if (inSettings)
        {
            // Back button
            if (isPointInRect(mx, my, 50, 50, 100, 40))
            {
                inSettings = false;
                inHomepage = true;
            }
            // Sound enable/disable
            else if (isPointInRect(mx, my, 200, 500, 150, 40))
            {
                soundEnabled = !soundEnabled;
            }
            // Music enable/disable
            else if (isPointInRect(mx, my, 200, 350, 150, 40))
            {
                musicEnabled = !musicEnabled;
                // Control background music based on setting
                if (musicEnabled)
                {
                    // Start background music if enabled
                    iStopAllSounds();
                    backgroundMusicChannel = iPlaySound("assets/sounds/bg.wav", true, musicVolume);
                }
                else
                {
                    // Stop background music if disabled
                    iStopAllSounds();
                    backgroundMusicChannel = -1; // Reset channel since music is stopped
                }
            }
            // Sound volume bar
            else if (isPointInRect(mx, my, 380, 465, 200, 20))
            {
                isDraggingVolume = true;
                soundVolume = ((mx - 380) * 100) / 200;
                if (soundVolume < 0)
                    soundVolume = 0;
                if (soundVolume > 100)
                    soundVolume = 100;
            }
            // Music volume bar
            else if (isPointInRect(mx, my, 380, 315, 200, 20))
            {
                isDraggingVolume = true;
                musicVolume = ((mx - 380) * 100) / 200;
                if (musicVolume < 0)
                    musicVolume = 0;
                if (musicVolume > 100)
                    musicVolume = 100;
                // Update background music volume immediately if music is enabled
                if (musicEnabled)
                {
                    // If music is already playing, just adjust the volume
                    if (backgroundMusicChannel != -1)
                    {
                        iSetVolume(backgroundMusicChannel, musicVolume);
                    }
                    else
                    {
                        // Start music if not playing
                        iStopAllSounds();
                        backgroundMusicChannel = iPlaySound("assets/sounds/bg.wav", true, musicVolume);
                    }
                }
            }
        }
        else if (inTribute)
        {
            // Back button in bottom left corner
            if (isPointInRect(mx, my, 50, 50, 100, 40))
            {
                inTribute = false;
                inHomepage = true;
            }
        }
        else if (inInstruction)
        {
            // Back button in bottom left corner
            if (isPointInRect(mx, my, 50, 50, 100, 40))
            {
                inInstruction = false;
                inHomepage = true;
            }
        }
        else if (inLevelSelect)
        {
            // Back to Home text area (adjusted for text-only)
            if (isPointInRect(mx, my, 50, 110, 130, 25))
            {
                inLevelSelect = false;
                inHomepage = true;
            }
            // Check if clicked on a level button
            else
            {
                for (int i = 0; i < 10; i++)
                {
                    int row = i / 5;
                    int col = i % 5;
                    int x = 100 + col * 120;
                    int y = 450 - row * 100;
                    if (isPointInRect(mx, my, x, y, 80, 60))
                    {
                        // Only allow clicking on unlocked levels
                        if ((i + 1) <= unlockedLevels)
                        {
                            loadLevel(i + 1);
                            // Play click sound after level loads to avoid being cut off
                            if (soundEnabled)
                            {
                                iPlaySound("assets/sounds/click.mp3", false, soundVolume);
                            }
                        }
                        break;
                    }
                }
            }
        }
        else if (inHighScore)
        {
            // Back button
            if (isPointInRect(mx, my, 50, 50, 120, 50))
            {
                inHighScore = false;
                inHomepage = true;
            }
        }
        else if (inNameInput)
        {
            // Continue button (if name is not empty)
            if (strlen(playerName) > 0 && isPointInRect(mx, my, GAME_WINDOW_WIDTH / 2 - 60, 250, 150, 50))
            {
                inNameInput = false;
                inLevelSelect = true;
                // Start background music when entering level selection if not already playing
                if (backgroundMusicChannel == -1 && musicEnabled)
                {
                    backgroundMusicChannel = iPlaySound("assets/sounds/bg.wav", true, musicVolume);
                }
            }
            // Back button
            else if (isPointInRect(mx, my, 50, 50, 100, 40))
            {
                inNameInput = false;
                inHomepage = true;
                // Start background music when going back to homepage
                // if (musicEnabled)
                // {
                //     backgroundMusicChannel = iPlaySound("assets/sounds/bg.wav", true, musicVolume);
                // }
            }
        }
        else if (inGame)
        {
            if (inPauseMenu)
            {
                // Handle pause menu clicks (updated for 3 buttons)
                int buttonWidth = 250;
                int buttonHeight = 60;
                int buttonSpacing = 80;
                int startY = 440;
                int centerX = 275;
                // Resume button
                if (isPointInRect(mx, my, centerX, startY, buttonWidth, buttonHeight))
                {
                    inPauseMenu = false; // Resume game
                }
                // Sound button
                else if (isPointInRect(mx, my, centerX, startY - buttonSpacing, buttonWidth, buttonHeight))
                {
                    soundEnabled = !soundEnabled;
                }
                // Exit button (to level selection)
                else if (isPointInRect(mx, my, centerX, startY - 2 * buttonSpacing, buttonWidth, buttonHeight))
                {
                    inGame = false;
                    inPauseMenu = false;
                    inLevelSelect = true;
                    isBallMoving = false;
                    // Restart background music when exiting game if not already playing
                    if (backgroundMusicChannel == -1 && musicEnabled)
                    {
                        backgroundMusicChannel = iPlaySound("assets/sounds/bg.wav", true, musicVolume);
                    }
                }
            }
            else
            {
                // Check if clicked on "Back" button - show pause menu
                if (isPointInRect(mx, my, 650, GAME_WINDOW_HEIGHT + 10, 120, 30))
                {
                    inPauseMenu = true; // Show pause menu instead of directly exiting
                }

                // Check for bomb icon click
                else if (!isBallMoving && isPointInRect(mx, my, GAME_WINDOW_WIDTH - 60, 20, 40, 40))
                {
                    if (bombCount > 0)
                    {
                        bombSelected = !bombSelected; // Toggle bomb selection
                        if (bombSelected)
                        {
                            // Store current shooting ball color and swap with bomb
                            shootingBallColorNow = -1; // -1 represents a bomb
                        }
                        else
                        {
                            // Restore the original color if bomb is deselected
                            shootingBallColorNow = getRandomLevelColor();
                        }
                    }
                }

                // Normal shooting logic
                else if (!isBallMoving)
                {
                    // Check if player has balls remaining
                    if (ballsRemaining <= 0)
                    {
                        // No balls left, trigger game over
                        inGameOver = true;
                        inGame = false;
                        return;
                    }
                    angle = atan2(my - (shooter_y + BUBBLE_RADIUS), mx - shooter_x);
                    if (my - (shooter_y + BUBBLE_RADIUS) >= 40)
                    {
                        vx = velocity * cos(angle);
                        vy = velocity * sin(angle);
                        ball_x = shooter_x;
                        ball_y = shooter_y + BUBBLE_RADIUS;
                        ballBounceCount = 0; // Reset bounce counter for new ball
                        moveCounted = false; // Reset move counter flag for new ball
                        isBallMoving = true;
                        int bomb = 0;
                        if (bombSelected)
                        {
                            bombCount--;
                            bomb = 1;
                            bombSelected = false;
                        }
                        // Decrease ball count when shot
                        if (!bomb)
                            ballsRemaining--;
                    }
                }
            } // Close the else block for normal game logic
        }
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        // Reset dragging flag when mouse is released
        isDraggingVolume = false;
    }
}
void iKeyboard(unsigned char key, int state)
{
    if (key == '>' && state == GLUT_DOWN)
    {
        int previous_level;
        previous_level = unlockedLevels;
        FILE *file_1 = fopen("saves/current_level.txt", "w");
        if (file_1 != NULL)
        {
            fprintf(file_1, "%d\n", previous_level);
            fclose(file_1);
        }
        unlockedLevels = 10;
        FILE *file = fopen("saves/level_progress.txt", "w");
        if (file != NULL)
        {
            fprintf(file, "%d\n", unlockedLevels);
            fclose(file);
        }
    }
    if (key == '<' && state == GLUT_DOWN)
    {
        int previous_level;
        FILE *file_1 = fopen("saves/current_level.txt", "r");
        if (file_1 != NULL)
        {
            fscanf(file_1, "%d", &previous_level);
            fclose(file_1);
        }
        unlockedLevels = previous_level;
        FILE *file = fopen("saves/level_progress.txt", "w");
        if (file != NULL)
        {
            fprintf(file, "%d\n", unlockedLevels);
            fclose(file);
        }
    }
    if (inNameInput && state == GLUT_DOWN)
    {
        switch (key)
        {
        case '\r': // Enter key
        case '\n':
            if (strlen(playerName) > 0)
            {
                inNameInput = false;
                inLevelSelect = true;

                // Only start background music if not already playing
                if (musicEnabled && backgroundMusicChannel == -1)
                {
                    backgroundMusicChannel = iPlaySound("assets/sounds/bg.wav", true, musicVolume);
                }
            }
            break;
        case 27:                          // ESC key
            strcpy(playerName, "Player"); // Default name
            inNameInput = false;
            inLevelSelect = true;
            // Start background music when entering level selection
            iStopAllSounds();
            if (musicEnabled)
            {
                backgroundMusicChannel = iPlaySound("assets/sounds/bg.wav", true, musicVolume);
            }
            break;
        case '\b': // Backspace
            if (strlen(playerName) > 0)
            {
                playerName[strlen(playerName) - 1] = '\0';
            }
            break;
        default:
            if (strlen(playerName) < 18) // Leave room for null terminator
            {
                playerName[strlen(playerName)] = key;
                playerName[strlen(playerName) + 1] = '\0';
            }
            break;
        }
        return; // Don't process other keys when in name input
    }

    if (key == '?')
        exit(0);
    // Single-press ball swapping (Space key)
    if ((key == ' ') && inGame && !isBallMoving && state == GLUT_DOWN)
    {
        if (!swapKeyPressed)
        {
            int temp = shootingBallColorNow;
            shootingBallColorNow = shootingBallColorNext;
            shootingBallColorNext = temp;
            swapKeyPressed = true;
        }
    }
    // Reset swap key state when key is released
    if ((key == ' ') && state == GLUT_UP)
    {
        swapKeyPressed = false;
    }
    // Navigation keys
    if (key == '@')
    {
        // Return to main menu from any screen
        inGame = false;
        inLevelSelect = false;
        inSettings = false;
        inTribute = false;
        inInstruction = false;
        inGameOver = false;
        inLevelComplete = false;
        inHomepage = true;
        isBallMoving = false;
        // Restart background music when returning to menu
        iStopAllSounds();
        if (musicEnabled)
        {
            iPlaySound("assets/sounds/bg.wav", true, musicVolume);
        }
    }
    if (key == 27) // ESC key
    {
        // Go back one level in menu hierarchy
        if (inGameOver)
        {
            inGameOver = false;
            inLevelSelect = true;
            // Restart background music when returning to level selection
            iStopAllSounds();
            if (musicEnabled)
            {
                backgroundMusicChannel = iPlaySound("assets/sounds/bg.wav", true, musicVolume);
            }
        }
        else if (inGame && inPauseMenu)
        {
            inPauseMenu = false; // Close pause menu, resume game
        }
        else if (inGame)
        {
            inPauseMenu = true; // Show pause menu
        }
        else if (inLevelSelect)
        {
            inLevelSelect = false;
            inHomepage = true;
            // Restart background music when returning to homepage
            iStopAllSounds();
            if (musicEnabled)
            {
                iPlaySound("assets/sounds/bg.wav", true, musicVolume);
            }
        }
        else if (inSettings || inTribute)
        {
            inSettings = false;
            inTribute = false;
            inHomepage = true;
            // Restart background music when returning to homepage
            iStopAllSounds();
            if (musicEnabled)
            {
                iPlaySound("assets/sounds/bg.wav", true, musicVolume);
            }
        }
    }
}
void iSpecialKeyboard(unsigned char key, int state) {}
void iMouseDrag(int mx, int my)
{
    if (inSettings && isDraggingVolume)
    {
        if (isPointInRect(mx, my, 380, 465, 200, 20))
        {
            soundVolume = ((mx - 380) * 100) / 200;
            if (soundVolume < 0)
                soundVolume = 0;
            if (soundVolume > 100)
                soundVolume = 100;
        }
        else if (isPointInRect(mx, my, 380, 315, 200, 20))
        {
            musicVolume = ((mx - 380) * 100) / 200;
            if (musicVolume < 0)
                musicVolume = 0;
            if (musicVolume > 100)
                musicVolume = 100;
            if (musicEnabled)
            {
                if (backgroundMusicChannel != -1)
                {
                    iSetVolume(backgroundMusicChannel, musicVolume);
                }
                else
                {
                    iStopAllSounds();
                    backgroundMusicChannel = iPlaySound("assets/sounds/bg.wav", true, musicVolume);
                }
            }
        }
    }
}

void iMouseWheel(int dir, int mx, int my) {}
int main(int argc, char *argv[])
{
    srand(time(NULL));
    glutInit(&argc, argv);
    {
        iInitializeSound();
        if (musicEnabled)
        {
            backgroundMusicChannel = iPlaySound("assets/sounds/bg.wav", true, musicVolume);
        }
        inHomepage = true;
        inLevelSelect = false;
        inGame = false;
        inSettings = false;
        inTribute = false;
        inInstruction = false;
        // Initialize high score system
        loadHighScores();
        // Initialize level progress system
        loadLevelProgress();
        shootingBallColorNow = getRandomLevelColor();
        shootingBallColorNext = getRandomLevelColor();
        iSetTimer(10, ballMovement);
        iInitializeFont();
    }
    iOpenWindow(GAME_WINDOW_WIDTH, ACTUAL_HEIGHT, "Bubble Shooter");
    return 0;
}