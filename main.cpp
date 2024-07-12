#include <iostream>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <chrono> // Include for time tracking
#include <fstream>
#include "Game.h"
using namespace std;

int main()
{
    initscr(); // Initialize ncurses
    Game game;

    short choice;
    while (true)
    {
        clear();
        printw(" _  _  _  _  _   _  _  _  _  _   _  _  _  _  _   _  _  _  _      _  _  _     _  _  _  _   \n");
        printw("(_)(_)(_)(_)(_) (_)(_)(_)(_)(_) (_)(_)(_)(_)(_) (_)(_)(_)(_) _  (_)(_)(_)  _(_)(_)(_)(_)_ \n");
        printw("      (_)       (_)                   (_)       (_)         (_)    (_)    (_)          (_)\n");
        printw("      (_)       (_) _  _              (_)       (_) _  _  _ (_)    (_)    (_)_  _  _  _   \n");
        printw("      (_)       (_)(_)(_)             (_)       (_)(_)(_)(_)       (_)      (_)(_)(_)(_)_ \n");
        printw("      (_)       (_)                   (_)       (_)   (_) _        (_)     _           (_)\n");
        printw("      (_)       (_) _  _  _  _        (_)       (_)      (_) _   _ (_) _  (_)_  _  _  _(_)\n");
        printw("      (_)       (_)(_)(_)(_)(_)       (_)       (_)         (_) (_)(_)(_)   (_)(_)(_)(_)  \n");
        printw("Enter your choice:\n");
        printw("0. Start Game \n");
        printw("1. View Ranking ");
        refresh();
        cin >> choice;
        game.readScoresFromFile();
        switch (choice)
        {
        case 0:
            game.init();
            game.run();
            game.reset();
            clear();
            refresh();
            break;
        case 1:
            game.ShowRankings();
            std::cout << "Viewing ranking...\n";
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }
    }
    endwin();

    return 0;
}
