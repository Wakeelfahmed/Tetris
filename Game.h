#include <iostream>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <chrono> // Include for time tracking
#include <fstream>
#include "Grid.h"
using namespace std;

class Game
{
private:
    Grid grid;
    Shape shape;
    Shape nextShape;
    int Score;
    time_t start;
    int Ranks[5];

public:
    Game()
    {
        srand(time(nullptr));
        Score = 100;
        start = time(0);
    }
    void reset()
    {
        grid.reset();
        srand(time(nullptr));
        Score = 100;
        start = time(0);
    }
    void init()
    {
        clear();
        refresh();
        grid.reset();
        start = time(0);

        for (int y = 0; y < GRID_HEIGHT; ++y)
            for (int x = 0; x < GRID_WIDTH; ++x)
                grid.grid[y][x] = 0;

        nextShape.setRandomShape();
        shape.setRandomShape();
        initscr();
        noecho();
        cbreak();
        curs_set(FALSE);
        start_color();
        init_pair(0, COLOR_BLUE, COLOR_BLUE);       // Blue on black
        init_pair(1, COLOR_RED, COLOR_RED);         // Red on black
        init_pair(2, COLOR_GREEN, COLOR_GREEN);     // Green on black
        init_pair(3, COLOR_GREEN, COLOR_GREEN);     // Green on black
        init_pair(4, COLOR_YELLOW, COLOR_YELLOW);   // Yellow on black
        init_pair(5, COLOR_MAGENTA, COLOR_MAGENTA); // Magenta on black
        init_pair(6, COLOR_MAGENTA, COLOR_MAGENTA); // Cyan on black
        init_pair(7, COLOR_MAGENTA, COLOR_MAGENTA); // White on black
        init_pair(8, COLOR_CYAN, COLOR_CYAN);       // White on black
        init_pair(9, COLOR_CYAN, COLOR_CYAN);       // White on black
        init_pair(10, COLOR_WHITE, COLOR_BLACK);    // White on black
        init_pair(11, COLOR_WHITE, COLOR_WHITE);    // White on black
        // for (int i = 1; i <= 10; ++i)
        // init_pair(i, i % 8 + 1, COLOR_BLACK);
        nextShape.setXY(15, 10);
        // init_pair(10, COLOR_WHITE, COLOR_BLACK);
    }

    void draw()
    {
        // clear();
        grid.draw();
        attrset(COLOR_PAIR(nextShape.shapeIndex));
        shape.draw();
        attrset(COLOR_PAIR(nextShape.shapeIndex));
        nextShape.draw();
        attrset(COLOR_PAIR(10));
        string score = "Score: " + to_string(Score);
        mvprintw(5, 25, "                      ");
        mvprintw(5, 25, "%s", score.c_str());
        string timeStr = "Elapsed time: " + std::to_string(static_cast<int>(difftime(time(0), start))) + " seconds";
        mvprintw(3, 25, "%s", timeStr.c_str());
        refresh();
    }

    bool update()
    {
        if (!shape.checkCollision(grid.grid, shape.x, shape.y + 1))
        {
            shape.eraseshape();
            shape.moveDown();
            mvprintw(0, 10, " fdaafds ");
            mvprintw(15, 25, "%d ", shape.shapeIndex);
            return true;
        }
        else
        {
            grid.placeShape(shape);
            grid.checkLines(Score);
            nextShape.eraseNextshape();
            mvprintw(0, 10, " fdaafds ");
            mvprintw(15, 25, "%d ", shape.shapeIndex);
            shape = nextShape;
            shape.ResetXY();
            nextShape.setRandomShape();
            if (shape.checkCollision(grid.grid, shape.x, shape.y))
            {
                attrset(COLOR_PAIR(8));
                mvprintw(GRID_HEIGHT / 2, (GRID_WIDTH / 2), "Game Over");
                attrset(COLOR_PAIR(10));
                insertScore(Score);
                refresh();
                napms(2000);
                return false;
            }
            return true;
        }
    }

    void handleInput()
    {
        int ch = getch();
        switch (ch)
        {
        case 'a':
            shape.moveLeft(grid.grid);
            break;
        case 'd':
            shape.moveRight(grid.grid);
            break;
        case 'w':
            shape.rotateClockwise(grid.grid);
            break;
        case 's':
            shape.rotateCounterClockwise(grid.grid);
            break;
        }
    }

    void run()
    {

        nodelay(stdscr, TRUE);
        while (true)
        {
            draw();
            handleInput();
            if (!update())
                return;
            napms(300);
        }
    }

    void insertScore(int score)
    {
        for (int i = 0; i < 5; i++)
            if (score > Ranks[i])
            {
                for (int j = 5 - 1; j > i; j--)
                    Ranks[j] = Ranks[j - 1];

                Ranks[i] = score;
                return;
            }
    }
    void readScoresFromFile()
    {
        ifstream file("ranking.txt");
        if (file.is_open())
        {
            file.seekg(0, std::ios::end);
            for (int i = 0; i < 5; i++)
                file >> Ranks[i];

            file.close();
        }
        else
            std::cerr << "Unable to open file " << "ranking.txt" << std::endl;
    }
    void ShowRankings()
    {
        clear();
        printw("RANKINGS\n");
        for (int i = 0; i < 5; i++)
            printw("%d. %s\n", (i + 1), to_string(Ranks[i]).c_str());
        refresh();
        napms(5000);
    }
};
