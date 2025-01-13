#include "Knight.h"



Knight::Knight(int row, int col, char teamName) {
	this->row = row;
	this->col = col;
	damage = 1;
	health = 11;
	defence = 0;
	attackSpeed = 0.5;
    attackRange = 1;
	target = nullptr;
    this->teamName = teamName;
    elixir = 2;
}

Knight::~Knight() {
    
}

int Knight::move() {
    int distance;
    distance = Arena::calculateDistance(this->row, this->col, target->getRow(), target->getCol());
    // Move towards the target until the distance to the target is equal to the attack distance
    if (distance > attackRange) {
        // Define the start and end points.
        Point start = { row, col };
        Point end = { target->getRow(), target->getCol() };

        // Find the shortest path through the maze.
        vector<Point> path = Arena::findShortestPath(start, end);
        size_t pathLength = path.size();

        
        if (pathLength < 1) {
            return 0;
        }
        else { // Print the path(if there is one).
            Arena::swapCell(row, col, path[0].row, path[0].col);
            setRow(path[0].row);
            setCol(path[0].col);

            return 1;
        }
    }
    return 0;
}


int Knight::attack() {
    int distance;
    distance = Arena::calculateDistance(this->row, this->col, target->getRow(), target->getCol());
    if (distance <= attackRange) {
        target->defend(damage);
        if (!getTarget()->isAlive()) {
            Arena::removeUnits(getTarget());
        }
        return 1;
    }
    return 0;
}

void Knight::defend(int attack) {
    if (defence <= attack) {
        attack -= defence;
        defence = 0;
        health -= attack;
    }
    else {
        defence -= attack;
    }
}

int Knight::ultimate() {
    return 0;
}

void Knight::caracteristic() {

}

Units* Knight::findTarget() {
    Units* target = nullptr;
    int targetDistance = INT_MAX;
    list<Units*> enemies;
    if (teamName == '1') {
        enemies = Arena::getTeam2();
    }
    else {
        enemies = Arena::getTeam1();
    }
    int distance;
    for (auto& unit : enemies) {
        if (unit->isAlive()) {
            distance = Arena::calculateDistance(this->row, this->col, unit->getRow(), unit->getCol());
            if (distance < targetDistance) {
                target = unit;
                targetDistance = distance;
            }
        }
    }
    return target;
}