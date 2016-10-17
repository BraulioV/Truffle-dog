#ifndef AGENT__
#define AGENT__

#include <string>
#include <vector>
using namespace std;

class Environment;
class Agent
{
public:

	Agent(){
        bump_=false;
        can_i_smell_ = true;
        orientacion = time_to_smell_ = 0;				// Empezamos mirando al norte e indicando que no
        								// tenemos obstaculos ninguno al rededor
    
        for (int i = 0; i < TAM; i++){
        	for (int j = 0; j < TAM; j++){
        		my_world[i][j].not_read_since_ = 0;
        		my_world[i][j].not_pass_since_ = 0;
        		my_world[i][j].size_ = 0;
        		my_world[i][j].is_wall_ = false;
        	}
        }

        POS_X = POS_Y = 10;

        TIME = 200;						// Tiempo medio para recogida en mapas rápidos y lentos
        
        ult_accion_ = actIDLE;			// La inicializamos diciendo que no ha hecho nada
	}

	
	enum ActionType
	{
	    actFORWARD,
	    actTURN_L,
	    actTURN_R,
	    actSNIFF,
	    actEXTRACT,
	    actIDLE
	};

	void Perceive(const Environment &env);
	void IncreaseTime(int x, int y);
	void ShowWorld();
	void DelimitMyWorld();
	ActionType WheretoMove(int x, int y);
	ActionType Think();
	double CalculateGrowingTime();
private:
	struct cell_data_ {
		int not_read_since_;
		int not_pass_since_;
		int size_;
		bool is_wall_;
	};

	int trufa_size_;
	
	static const int TAM = 20;

	int POS_X, POS_Y, TIME;

	bool can_i_smell_;

	cell_data_ my_world[TAM][TAM];

	bool bump_;
	int orientacion, time_to_smell_;

	ActionType ult_accion_;
};

string ActionStr(Agent::ActionType);

#endif
