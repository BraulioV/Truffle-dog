#include "evaluator.h"
#include "environment.h"

void Evaluator::Eval(Agent::ActionType action, const Environment &env)
{
	if (action == Agent::actSNIFF)
		consumedEnergy_ += 1;
	else if (action != Agent::actIDLE)
		consumedEnergy_ += 1;


	for (int row=0; row<Environment::MAZE_SIZE; row+=1)
	{
		for (int col=0; col<Environment::MAZE_SIZE; col+=1)
		{
			long long da = env.TrufaAmount(row, col);
			Global_Trufa_ = (da);
		}
	}
}


long long Evaluator::Global_Trufa(const Environment &env)
{
    Global_Trufa_=0;

	for (int row=0; row<Environment::MAZE_SIZE; row+=1)
	{
		for (int col=0; col<Environment::MAZE_SIZE; col+=1)
		{
			long long da = env.TrufaAmount(row, col);
			Global_Trufa_ += (da);
		}
	}

	return Global_Trufa_;
}
