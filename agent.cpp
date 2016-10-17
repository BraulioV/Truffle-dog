#include "agent.h"
#include "environment.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <utility>
#include <iomanip>

#define BLACK    "\033[0m"
#define RED      "\033[41m"
#define WHITE    "\033[47m"

using namespace std;
/*
    representar una variable entera para indicar los 4 puntos cardinales y usaremos un vector
    de cuatro entradas para averiguar las posiciones donde tengo un obstaculo
*/

// -----------------------------------------------------------
Agent::ActionType Agent::Think(){
    
    switch(ult_accion_){
        case actFORWARD:
            if(bump_){

                switch(orientacion){
                    case 0:             // Mira hacia el Norte
                        my_world[POS_X-1][POS_Y].is_wall_ = true;
                        break;

                    case 1:             // Mira hacia el Este
                        my_world[POS_X][POS_Y+1].is_wall_ = true;
                        break;

                    case 2:             // Mira hacia el SUr
                        my_world[POS_X+1][POS_Y].is_wall_ = true;
                        break;

                    case 3:             // Mira hacia el Oeste
                        my_world[POS_X][POS_Y-1].is_wall_ = true;
                        break;
                }
           }

            else {
                switch(orientacion){
                    case 0:             // Mira hacia el Norte
                        POS_X--;
                        break;

                    case 1:             // Mira hacia el Este
                        POS_Y++;
                        break;

                    case 2:             // Mira hacia el Sur
                        POS_X++;
                        break;

                    case 3:             // Mira hacia el Oeste
                        POS_Y--;
                        break;
                }
                
            }
                        
        break;
            
        case actTURN_R:
            orientacion++;
            
            if(orientacion > 3)     // orientacion = (orientacion + 1)%3
                orientacion = 0;
        break;

        case actTURN_L:
        // Si es -1, tenemos que decirle que se ha dado la vuelta
            orientacion--;
            if (orientacion < 0){
                orientacion = 3;
            }
        break;

        case actSNIFF:
            my_world[POS_X][POS_Y].size_ = trufa_size_;
        break;
    }

    ActionType accion;

    if(can_i_smell_ && (time_to_smell_ % 5 == 0)){
        accion = actSNIFF;

        if(time_to_smell_ >= TIME){
            can_i_smell_ = false;
            double factor = CalculateGrowingTime();

            if(factor >= 0.1 && factor > 0.5)
                TIME = 310;
            else
                TIME = 585;
        }
    }

    else if(!can_i_smell_ && ult_accion_ != actTURN_L && 
        ult_accion_ != actTURN_R && my_world[POS_X][POS_Y].not_read_since_ > TIME)
        accion=actEXTRACT;

    else 
        accion = WheretoMove(POS_X,POS_Y);
        
    ult_accion_ = accion;

    IncreaseTime(POS_X, POS_Y);

    // ShowWorld();

    return accion;
}
// -----------------------------------------------------------
void Agent::Perceive(const Environment &env)
{
    trufa_size_ = env.TrufaAmount();
    bump_ = env.isJustBump();
}

double Agent::CalculateGrowingTime(){
    double factor = 0;
    int times_i_smelt = 0;

    for(int i = 0; i < TAM; i++)
        for(int j = 0; j < TAM; j++){
            factor += my_world[i][j].size_;
            times_i_smelt++;
        }

    return factor/100;
}

// -----------------------------------------------------------
string ActionStr(Agent::ActionType accion)
{
    switch (accion)
    {
    case Agent::actFORWARD: return "FORWARD";
    case Agent::actTURN_L: return "TURN LEFT";
    case Agent::actTURN_R: return "TURN RIGHT";
    case Agent::actSNIFF: return "SNIFF";
    case Agent::actEXTRACT: return "EXTRACT";
    case Agent::actIDLE: return "IDLE";
    default: return "???";
    }
}
// -----------------------------------------------------------
void Agent::IncreaseTime(int x, int y){
    for(int i = 0; i < TAM; i++)
        for(int j = 0; j < TAM; j++){
            if(my_world[i][j].is_wall_){
                my_world[i][j].not_pass_since_ = -1;
                my_world[i][j].not_read_since_ = -1;
            }
            else{
                my_world[i][j].not_pass_since_++;
                my_world[i][j].not_read_since_++;
            }
        }

    my_world[x][y].not_pass_since_ = 0;

    if(ult_accion_ == actEXTRACT)
        my_world[x][y].not_read_since_ = 0;

    time_to_smell_++;
}

// -----------------------------------------------------------
void Agent::ShowWorld(){
    cout << endl << "Posicion actual: [" << POS_X << ", " << POS_Y << "]" << endl;
    cout << "Orientacion:" << orientacion << endl;
    cout << "TIME: " << TIME << endl;
    cout << "Tiempo para oler " << time_to_smell_ << endl;
    cout << "Ultima accion: ";

    switch(ult_accion_){
        case actFORWARD:
            cout << "Seguir adelante" << endl;
        break;

        case actTURN_L:
            cout << "Girar a la izquierda" << endl;
        break;
        case actTURN_R:
            cout << "Girar a la derecha" << endl;
        break;
        case actEXTRACT:
            cout << "Extraer" << endl;
        break;
        case actSNIFF:
            cout << "Oler" << endl;
        break;
   }

    for (int i = 0; i < TAM; ++i){
        for (int j = 0; j < TAM; j++){
            cout << setw(5) << my_world[i][j].not_read_since_ << setw(2);

                if (my_world[i][j].is_wall_)
                    cout << RED <<'.' << BLACK <<setw(1);
                else
                    cout << WHITE << '.'<< BLACK << setw(1);
        }
        cout << endl;
    }
}

// -----------------------------------------------------------
Agent::ActionType Agent::WheretoMove(int x, int y){

    ActionType next_action;
        switch(orientacion){
            case 0:
                if(!my_world[x-1][y].is_wall_){
                    if(my_world[x-1][y].not_pass_since_ < my_world[x][y-1].not_pass_since_ /*&& !my_world[x][y-1].is_wall_*/)
                        next_action = actTURN_L;
                    
                    else if(my_world[x-1][y].not_pass_since_ < my_world[x][y+1].not_pass_since_ /*&& !my_world[x][y+1].is_wall_*/)
                        next_action = actTURN_R;

                    else                        
                        next_action = actFORWARD;

                }
                else {
                    if(!my_world[x][y+1].is_wall_ && 
                        my_world[x][y+1].not_pass_since_ > my_world[x][y-1].not_pass_since_)
                            next_action = actTURN_R;

                    else if(!my_world[x][y-1].is_wall_ && 
                        my_world[x][y-1].not_pass_since_ > my_world[x][y+1].not_pass_since_)
                            next_action = actTURN_L;

                    else
                        next_action = actTURN_R;
                }
            break;

            case 1:
                if(!my_world[x][y+1].is_wall_){

                    if(my_world[x][y+1].not_pass_since_ < my_world[x-1][y].not_pass_since_ /*&& !my_world[x-1][y].is_wall_*/)
                        next_action = actTURN_L;

                    else if(my_world[x][y+1].not_pass_since_ < my_world[x+1][y].not_pass_since_ /*&& !my_world[x+1][y].is_wall_*/)
                        next_action = actTURN_R;

                    else                        
                        next_action = actFORWARD;
                }

                else {

                    if(!my_world[x+1][y].is_wall_ && 
                        my_world[x+1][y].not_pass_since_ > my_world[x-1][y].not_pass_since_)
                            next_action = actTURN_R;

                    else if(!my_world[x-1][y].is_wall_ && 
                        my_world[x-1][y].not_pass_since_ > my_world[x+1][y].not_pass_since_)
                            next_action = actTURN_L;
                    else
                        next_action = actTURN_R;
                }
            break;

            case 2:
                if(!my_world[x+1][y].is_wall_){
                    if(my_world[x+1][y].not_pass_since_ < my_world[x][y+1].not_pass_since_ /*&& !my_world[x][y+1].is_wall_*/)
                        next_action = actTURN_L;

                    else if(my_world[x+1][y].not_pass_since_ < my_world[x][y-1].not_pass_since_ /*&& !my_world[x][y-1].is_wall_*/)
                        next_action = actTURN_R;

                    else
                        next_action = actFORWARD;
                }

                else {
                    if(!my_world[x][y-1].is_wall_ && 
                        my_world[x][y-1].not_pass_since_ > my_world[x][y+1].not_pass_since_)
                            next_action = actTURN_R;

                    else if(!my_world[x][y+1].is_wall_ && 
                        my_world[x][y+1].not_pass_since_ > my_world[x][y-1].not_pass_since_)
                            next_action = actTURN_L;

                    else
                        next_action = actTURN_R;
                }
            break;

            case 3:
                if(!my_world[x][y-1].is_wall_){
                    if(my_world[x][y-1].not_pass_since_ < my_world[x+1][y].not_pass_since_ /*&& !my_world[x+1][y].is_wall_*/)
                        next_action = actTURN_L;

                    else if(my_world[x][y-1].not_pass_since_ < my_world[x-1][y].not_pass_since_ /*&& !my_world[x-1][y].is_wall_*/)
                        next_action = actTURN_R;

                    else
                        next_action = actFORWARD;
                }
                else {
                    if(!my_world[x-1][y].is_wall_ && 
                        my_world[x-1][y].not_pass_since_ > my_world[x+1][y].not_pass_since_)
                            next_action = actTURN_R;

                    else if(!my_world[x+1][y].is_wall_ && 
                        my_world[x+1][y].not_pass_since_ > my_world[x-1][y].not_pass_since_)
                            next_action = actTURN_L;

                    else
                        next_action = actTURN_R;
                }
            break;
        }

    return next_action;
}