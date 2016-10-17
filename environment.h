
#ifndef ENVIRONMENT__
#define ENVIRONMENT__

#include <fstream>
using std::ifstream;

#include "agent.h"
// -----------------------------------------------------------
//				class Environment
// -----------------------------------------------------------

class RandomNumberGenerator;

class Environment
{
public:
	static const int MAZE_SIZE = 10;

	Environment(ifstream &infile);
	void Show(int,int) const;
	void Change(const RandomNumberGenerator &rng);
	void AcceptAction(Agent::ActionType);

	bool IsTrufaHere() const { return maze_[agentPosX_][agentPosY_] > 0; }
	bool isJustBump() const { return bump_; }
	int  TrufaAmount(int x, int y) const;
	int  TrufaAmount() const {return trufa_size_;}
	int  RandomSeed() const { return randomSeed_; }
	int  TrufaCollected() const {return collected_;}
private:
	static const int OBSTACLE = -1;
	static const char MAP_ROAD = '-', MAP_OBSTACLE = 'O';

	bool bump_;
	int trufa_size_;
	int agentPosX_, agentPosY_, agentOriented_; // Orientacion: 0 Norte, 1 Este, 2 Sur, 3 Oeste.
	int maze_[MAZE_SIZE][MAZE_SIZE]; // -1: Obstacle, >=0: amount of dirt
	int maze2_[MAZE_SIZE][MAZE_SIZE];
	int randomSeed_;
	int collected_;
	double Grow_prob_;
	double GlobalTrufa_;

	/**/
	Agent::ActionType preAction_;
};
// -----------------------------------------------------------

#endif
