#pragma once
#include "Arena.h"

class Barbarian : public Units
{
private:
	int row;
	int col;
	int damage;
	int health;
	int defence;
	double attackSpeed;
	int attackRange;
	Units* target;
	char symbol = 'b';
	char teamName;
	int elixir;


public:
	Barbarian(int row, int col, char teamName);
	~Barbarian();
	int move() override; //move to the the closest target until distance to target matches attackDistance
	int attack() override; //attack all targets by calling their defend function with damage as attributs
	void defend(int attack) override;	//reduce defence by attack, if defence equals 0 reduce health
	int ultimate() override; //use Units ultimate
	void caracteristic() override;  //change units behavior
	bool isAlive() override { return health > 0; }
	Units* findTarget() override; //find the closest enemy(if teamName == 1 use Arena.getTeam2())


	int getHealth() override { return health; }
	int getRow() override { return row; }
	int getCol() override { return col; }
	void setRow(int row) override { this->row = row; }
	void setCol(int col) override { this->col = col; }
	char getSymbol() override { return symbol; }
	char getTeamName() override { return teamName; }
	Units* getTarget() override { return target; }
	void setTarget(Units* target) override { this->target = target; }
	int getAttackRange() override { return attackRange; }
	int getElixir() override { return elixir; }
	void setElixir(int elixir) override { this->elixir = elixir; }
};
