#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <chrono> // Include for time tracking
#include <fstream>
using namespace std;

const int GRID_HEIGHT = 20;
const int GRID_WIDTH = 10;
const int NUM_SHAPES = 9;

bool shapes[NUM_SHAPES][4][4] = {
    {{1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // sq
    {{1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}}, // vertical  //green
    {{1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // horizontal   //yellow
    {{0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // tri   //blue
    {{1, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}}, // L
    {{0, 1, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}}, // fliped L
    {{1, 1, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}}, // upside down L
    {{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}};

class Shape
{
public:
    bool shape[4][4];
    int shapeIndex;
    short x, y;

    Shape() : shapeIndex(0), x((GRID_WIDTH / 2) + 1), y(0)
    {
        setRandomShape();
    }
    void setXY(short x, short y)
    {
        this->x = x;
        this->y = y;
    }
    void ResetXY()
    {
        this->x = (GRID_WIDTH / 2) + 1;
        this->y = 0;
    }
    void setRandomShape()
    {
        shapeIndex = rand() % NUM_SHAPES;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                shape[i][j] = shapes[shapeIndex][i][j];
    }

    void draw() const
    {
        attrset(COLOR_PAIR(shapeIndex));
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                if (shape[i][j])
                    mvprintw(y + i, (x + j) * 2, "[]");
    }

    void eraseshape() const
    {
        attrset(COLOR_PAIR(10));
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                if (shape[i][j])
                    mvprintw(y + i, (x + j) * 2, " * ");
    }
    void eraseNextshape() const
    {
        attrset(COLOR_PAIR(10));
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                if (shape[i][j])
                    mvprintw(y + i, (x + j) * 2, "  ");
    }

    bool checkCollision(bool grid[GRID_HEIGHT][GRID_WIDTH], int newX, int newY) const
    {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                if (shape[i][j] && (newX + j < 0 || newX + j >= GRID_WIDTH || newY + i >= GRID_HEIGHT || grid[newY + i][newX + j]))
                    return true;
        return false;
    }

    void moveLeft(bool grid[GRID_HEIGHT][GRID_WIDTH])
    {
        if (!checkCollision(grid, x - 1, y))
            x--;
    }

    void moveRight(bool grid[GRID_HEIGHT][GRID_WIDTH])
    {
        if (!checkCollision(grid, x + 1, y))
            x++;
    }

    void moveDown()
    {
        y++;
    }
    bool canRotate(bool newShape[4][4], bool grid[GRID_HEIGHT][GRID_WIDTH]) const
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                if (newShape[i][j] && (x + j < 0 || x + j >= GRID_WIDTH || y + i >= GRID_HEIGHT || grid[y + i][x + j]))
                    return false;
            }
        }
        return true;
    }
    void rotateClockwise(bool grid[GRID_HEIGHT][GRID_WIDTH])
    {
        bool temp[4][4];

        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                temp[j][4 - i - 1] = shape[i][j];

        if (canRotate(temp, grid))
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    shape[i][j] = temp[i][j];
    }
    void rotateCounterClockwise(bool grid[GRID_HEIGHT][GRID_WIDTH])
    {
        bool temp[4][4];

        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                temp[4 - j - 1][i] = shape[i][j];

        if (canRotate(temp, grid))
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    shape[i][j] = temp[i][j];
    }
};
