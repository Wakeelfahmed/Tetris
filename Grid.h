#include <iostream>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <chrono> // Include for time tracking
#include <fstream>
#include "Shapes.h"
using namespace std;

class Grid
{
public:
    bool grid[GRID_HEIGHT][GRID_WIDTH];
    short grid_color[GRID_HEIGHT][GRID_WIDTH];

    Grid()
    {
        for (int y = 0; y < GRID_HEIGHT; ++y)
            for (int x = 0; x < GRID_WIDTH; ++x)
            {
                grid[y][x] = false;
                grid_color[y][x] = COLOR_WHITE;
            }
    }
    void reset()
    {
        for (int y = 0; y < GRID_HEIGHT; ++y)
            for (int x = 0; x < GRID_WIDTH; ++x)
                grid[y][x] = false;
    }
    void draw() const
    {
        for (int y = 0; y < GRID_HEIGHT; ++y)
            for (int x = 0; x < GRID_WIDTH; ++x)
                if (grid[y][x] == 0)
                {
                    attrset(COLOR_PAIR(10));
                    mvprintw(y, x * 2, " * ");
                }
                else
                {
                    attrset(COLOR_PAIR(grid_color[y][x]));

                    mvprintw(y, x * 2, "[]");
                    attrset(COLOR_PAIR(10));
                }
    }

    void placeShape(const Shape &shape)
    {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                if (shape.shape[i][j])
                {
                    grid[shape.y + i][shape.x + j] = 1;
                    grid_color[shape.y + i][shape.x + j] = shape.shapeIndex;
                }
    }
    void checkLines(int &Score)
    {
        short number_of_lines_completed = 0;
        for (int y = GRID_HEIGHT - 1; y >= 0; --y)
        {
            bool lineComplete = true;
            for (int x = 0; x < GRID_WIDTH; ++x)
            {
                if (grid[y][x] == 0)
                {
                    lineComplete = false;
                    break;
                }
            }
            if (lineComplete)
            {
                number_of_lines_completed++;
                for (int k = y; k > 0; --k)
                    for (int x = 0; x < GRID_WIDTH; ++x)
                        grid[k][x] = grid[k - 1][x];
                for (int x = 0; x < GRID_WIDTH; ++x)
                    grid[0][x] = 0;
            }
        }
        if (number_of_lines_completed == 4)
            Score += 800;
        else if (number_of_lines_completed >= 4)
            Score = Score + 800 + number_of_lines_completed * 1200;
        else if (number_of_lines_completed < 4)
            Score = Score + 100 * number_of_lines_completed;
    }
};