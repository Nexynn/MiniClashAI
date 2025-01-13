#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib> // For rand() and srand()
#include <ctime> // For time()
#include <chrono>
#include <fstream>

// Define a type to represent a point in the maze.
struct Point {
    int row, col;
};

#include "Units.h"
#include "Knight.h"
#include "Archer.h"
#include "Barbarian.h"




class Arena {
private:
    static Units* cells[8][5];
    static list<Units*> team1;
    static list<Units*> team2;
    static int elixirTeam1;
    static int elixirTeam2;
    static list<char> deck1;
    static list<char> deck2;
    

public:
    const static list<Units*> getTeam1() { return team1; }
    const static list<Units*> getTeam2() { return team2; }

    Arena();
    ~Arena();
    static void playTeam(char teamNumber);
    static void initialisationEquipe();
    static void randomPlacement(char teamNumber);
    static void setUnits(Units* units); // Set the value of a cell at the given row and column
    static Units* getCell(int row, int col); // Get the value of a cell at the given row and column
    static void updateSide(list<Units*>& side);
    static void removeUnits(Units* units);
    static void swapCell(int row1, int col1, int row2, int col2);
    static void print();
    static void clearConsoleOutput();
    static void predictGame(char teamNumber);

    static int calculateDistance(int row1, int col1, int row2, int col2);
    static bool isWithinBounds(const Point& p);
    static vector<Point> findShortestPath(Point start, Point end);
    static vector<Point> _findShortestPath(Point start, Point end, vector<vector<bool>> visited, int maxPath, int currentLength);
    friend ostream& operator<<(ostream& out, Arena& arena);
};