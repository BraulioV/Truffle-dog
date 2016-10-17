
#ifndef EVALUATOR__
#define EVALUATOR__

#include "agent.h"

class Environment;

class Evaluator
{
public:
	Evaluator():Global_Trufa_(0), consumedEnergy_(0) {};

	long long TrufaDegree() const { return Global_Trufa_; }
	long long ConsumedEnergy() const { return consumedEnergy_; }
    long long Global_Trufa(const Environment &env);


	void Eval(Agent::ActionType action, const Environment &env);

	static const int LIFE_TIME = 2000;

private:
	long long Global_Trufa_, consumedEnergy_;
};

#endif
