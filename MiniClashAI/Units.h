#pragma once
#include <string>
#include <list>
#include <cmath>
using namespace std;

class Units
{
private:

public:
	Units() {};
	virtual ~Units() {};
	virtual int move() { return 0; }; //move to the the closest target until distance to target matches attackDistance
	virtual int attack() { return 0; }; //attack all targets
	virtual void defend(int attack) {};	//reduce defence by attack, if defence equals 0 reduce health
	virtual int ultimate() { return 0; }; //use Units ultimate
	virtual void caracteristic() {}; //change units behavior
	virtual Units* findTarget() { return nullptr; } //find targets according to his caracteristics
	virtual bool isAlive() { return false; }
	virtual string toString() { return ""; } //show Units information

	
	virtual int getHealth() { return 0; }
	virtual int getRow() { return 0; }
	virtual int getCol() { return 0; }
	virtual void setRow(int row) {}
	virtual void setCol(int col) {}
	virtual char getSymbol() { return '-'; }
	virtual char getTeamName() { return '?'; }
	virtual Units* getTarget() { return nullptr; }
	virtual void setTarget(Units* target) {}
	virtual int getAttackRange() { return 0; }
	virtual int getElixir() { return 0; }
	virtual void setElixir(int elixir) {}
};