#include "Archer.h"



Archer::Archer(int row, int col, char teamName) {
    this->row = row;
    this->col = col;
    damage = 1;
    health = 6;
    defence = 0;
    attackSpeed = 0.5;
    attackRange = 20;
    target = nullptr;
    this->teamName = teamName;
    elixir = 2;
}



Archer::~Archer() {
    
}

int Archer::move() {
    return 0;
}


int Archer::attack() {
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

void Archer::defend(int attack) {
    if (defence <= attack) {
        attack -= defence;
        defence = 0;
        health -= attack;
    }
    else {
        defence -= attack;
    }
}

int Archer::ultimate() {
    return 0;
}

void Archer::caracteristic() {

}

Units* Archer::findTarget() {
    Units* target = nullptr;
    int targetDistance = 0;
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
            if (distance > targetDistance) {
                target = unit;
                targetDistance = distance;
            }
        }
    }
    return target;
}