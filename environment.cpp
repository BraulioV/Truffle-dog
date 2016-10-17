
#include <iostream>
using std::cout;
using std::endl;
#include <iomanip>
using std::setw;
#include <string>
using std::string;
#include "stdlib.h"
#include <cmath>

#include "environment.h"
#include "random_num_gen.h"
#include"include/GL/glui.h"

Environment::Environment(ifstream &infile)
{
	string comment;
	getline(infile, comment);
	infile >> agentPosX_ >> agentPosY_ >> Grow_prob_ >> randomSeed_;
    agentOriented_=0; //Siempre mira hacia el norte

	for (int row=0; row<MAZE_SIZE; row+=1)
	{
		for (int col=0; col<MAZE_SIZE; col+=1)
		{
            maze2_[row][col]=0;
			char c;
			infile >> c;
			if (c == MAP_ROAD) {
   				maze_[row][col] = 0;
			}
			else if (c == MAP_OBSTACLE) {
   				maze_[row][col] = OBSTACLE;
			}
			else {
				cout << c << " is an unknown symbol in agent.map!"; exit(1);
			}
		}// for - col
	}// for - row

	bump_ = false;
	trufa_size_ = -1;
	collected_=0;
	GlobalTrufa_=0;
	preAction_=Agent::actIDLE;
}
// -----------------------------------------------------------
int Environment::TrufaAmount(int x, int y) const
{
	if (maze_[x][y] == OBSTACLE) return 0;
	else return maze_[x][y];
}
// -----------------------------------------------------------
void Environment::Show(int w,int h) const
{
	const static float trufa_level=50.0;
	float length=(float)(w>h?h:w)/(float)(MAZE_SIZE+4);
	float size=0.8*length;
	for (int row=0; row<MAZE_SIZE; row+=1)
	{
		for (int col=0; col<MAZE_SIZE; col+=1)
		{
			float x=(col-MAZE_SIZE/2)*length*2+length,y=(MAZE_SIZE/2-row)*length*2-length;
			if (maze_[row][col] == OBSTACLE){
				//cout << "XXXXXX";

				glBegin(GL_POLYGON);
					glColor3f(0.5,0.25,0.0);
					glVertex2f(x-length,y-length);	glVertex2f(x+length,y-length);
					glVertex2f(x+length,y+length);	glVertex2f(x-length,y+length);
				glEnd();
			}
			else
			{
				char symbol = ' ';
   				if (row == agentPosX_ && col == agentPosY_){
					symbol = '=';
					switch(preAction_){
						case Agent::actIDLE:
   						    switch (agentOriented_){
   						      case 0: // Orientacion Norte
							         glBegin(GL_POLYGON);
   						             glColor3f(0.0,0.0,1.0);
								     glVertex2f(x,y+size);
								     glVertex2f(x+size,y-size);	glVertex2f(x-size,y-size);
							         glEnd();
							         break;
                              case 1: // Orientacion Este
							         glBegin(GL_POLYGON);
   						             glColor3f(0.0,0.0,1.0);
								     glVertex2f(x+size,y);
								     glVertex2f(x-size,y-size);	glVertex2f(x-size,y+size);
								     glEnd();
								     break;
                              case 2: // Orientacion Sur
							         glBegin(GL_POLYGON);
   						             glColor3f(0.0,0.0,1.0);
								     glVertex2f(x,y-size);
								     glVertex2f(x+size,y+size);	glVertex2f(x-size,y+size);
								     glEnd();
								     break;
                              case 3: // Orientacion Oeste
							         glBegin(GL_POLYGON);
   						             glColor3f(0.0,0.0,1.0);
								     glVertex2f(x-size,y);
								     glVertex2f(x+size,y-size);	glVertex2f(x+size,y+size);
								     glEnd();
								     break;
   						    }
							break;
						case Agent::actSNIFF:
							glBegin(GL_POLYGON);
								glColor3f(1.0,0.8,0.3);
								glVertex2f(x-size,y-size);	glVertex2f(x+size,y-size);
								glVertex2f(x+size,y+size);	glVertex2f(x-size,y+size);
							glEnd();
							break;
						case Agent::actEXTRACT:
							glBegin(GL_POLYGON);
								glColor3f(1.0,0.3,0.8);
								glVertex2f(x-size,y-size);	glVertex2f(x+size,y-size);
								glVertex2f(x+size,y+size);	glVertex2f(x-size,y+size);
							glEnd();
							break;
						case Agent::actFORWARD:
							if(bump_){
								glColor3f(1.0,0.0,0.0);
							}
							else{
								glColor3f(0.0,1.0,0.0);
							}
   						    switch (agentOriented_){
   						      case 0: // Orientacion Norte
							         glBegin(GL_POLYGON);
								     glVertex2f(x,y+size);
								     glVertex2f(x+size,y-size);	glVertex2f(x-size,y-size);
							         glEnd();
							         break;
                              case 1: // Orientacion Este
							         glBegin(GL_POLYGON);
								     glVertex2f(x+size,y);
								     glVertex2f(x-size,y-size);	glVertex2f(x-size,y+size);
								     glEnd();
								     break;
                              case 2: // Orientacion Sur
							         glBegin(GL_POLYGON);
								     glVertex2f(x,y-size);
								     glVertex2f(x+size,y+size);	glVertex2f(x-size,y+size);
								     glEnd();
								     break;
                              case 3: // Orientacion Oeste
							         glBegin(GL_POLYGON);
								     glVertex2f(x-size,y);
								     glVertex2f(x+size,y-size);	glVertex2f(x+size,y+size);
								     glEnd();
								     break;
   						    }
							break;
						case Agent::actTURN_L:
						case Agent::actTURN_R:
								glColor3f(0.0,1.0,0.0);
   						    switch (agentOriented_){
   						      case 0: // Orientacion Norte
							         glBegin(GL_POLYGON);
								     glVertex2f(x,y+size);
								     glVertex2f(x+size,y-size);	glVertex2f(x-size,y-size);
							         glEnd();
							         break;
                              case 1: // Orientacion Este
							         glBegin(GL_POLYGON);
								     glVertex2f(x+size,y);
								     glVertex2f(x-size,y-size);	glVertex2f(x-size,y+size);
								     glEnd();
								     break;
                              case 2: // Orientacion Sur
							         glBegin(GL_POLYGON);
								     glVertex2f(x,y-size);
								     glVertex2f(x+size,y+size);	glVertex2f(x-size,y+size);
								     glEnd();
								     break;
                              case 3: // Orientacion Oeste
							         glBegin(GL_POLYGON);
								     glVertex2f(x-size,y);
								     glVertex2f(x+size,y-size);	glVertex2f(x+size,y+size);
								     glEnd();
								     break;
   						    }
							break;
					}
				}
				if (maze2_[row][col]==0){
                    glBegin(GL_POLYGON);
                        glColor3f(((float)(trufa_level-maze_[row][col]))/trufa_level,((float)(trufa_level-maze_[row][col]))/trufa_level,((float)(trufa_level-maze_[row][col]))/trufa_level);
                        glVertex2f(x-length,y-length);	glVertex2f(x+length,y-length);
                        glVertex2f(x+length,y+length);	glVertex2f(x-length,y+length);
                    glEnd();
                }
                else if (maze2_[row][col]==1){
                    glBegin(GL_POLYGON);
                        glColor3f(1.0,1.0,1.0);
                        glVertex2f(x-size,y-size);	glVertex2f(x+size,y-size);
                        glVertex2f(x+size,y+size);	glVertex2f(x-size,y+size);
                    glEnd();
                    glBegin(GL_POLYGON);
                        glColor3f(0.0,0.0,0.0);
                        glVertex2f(x-length,y-length);	glVertex2f(x+length,y-length);
                        glVertex2f(x+length,y+length);	glVertex2f(x-length,y+length);
                    glEnd();
                }
                else if (maze2_[row][col]==2){
                    glBegin(GL_POLYGON);
                        glColor3f(1.0,1.0,1.0);
                        glVertex2f(x-length/2,y-length/2);	glVertex2f(x+length/2,y-length/2);
                        glVertex2f(x+length/2,y+length/2);	glVertex2f(x-length/2,y+length/2);
                    glEnd();
                    glBegin(GL_POLYGON);
                        glColor3f(0.0,0.0,0.0);
                        glVertex2f(x-length,y-length);	glVertex2f(x+length,y-length);
                        glVertex2f(x+length,y+length);	glVertex2f(x-length,y+length);
                    glEnd();
                }
                else if (maze2_[row][col]==3){
                    glBegin(GL_POLYGON);
                        glColor3f(0.0,0.0,0.0);
                        glVertex2f(x-length,y-length);	glVertex2f(x+length,y-length);
                        glVertex2f(x+length,y+length);	glVertex2f(x-length,y+length);
                    glEnd();
                }
                else {
                    glBegin(GL_POLYGON);
                        glColor3f(1.0,1.0,1.0);
                        for(int j=0;j<100;j++)
                        {
                            float angle = ((2*3.1415926535897*j)/100);

                           glVertex2f(x+size*cos(angle),y+size*sin(angle));
                        }
                    glEnd();

                }
				//cout << symbol;
				//cout << setw(4) << maze_[row][col];
				//cout << symbol;

			}
		}// for - col
		//cout << endl;
	}// for - row
}
// -----------------------------------------------------------
void Environment::Change(const RandomNumberGenerator &rng)
{
	for (int row=0; row<MAZE_SIZE; row+=1)
	{
		for (int col=0; col<MAZE_SIZE; col+=1)
		{
			if (maze_[row][col] != OBSTACLE &&
			    static_cast<double>(rng.RandomValue())/RAND_MAX < Grow_prob_) // probability: 0.01
			{
			    if (maze2_[row][col]<4){
                    if (maze2_[row][col]==0){
                        if (maze_[row][col]<2)
                            maze_[row][col]+=1;
                        else
                            maze_[row][col]+=2;

                        if (maze_[row][col]>=20){
                            maze_[row][col]=20;
                            maze2_[row][col]=1;
                        }
                    }
                    else {
                        maze2_[row][col]++;
                    }
			    }else{
                    maze2_[row][col]++;
                    maze_[row][col]=0;
			    }

			}
		}// for - col
	}// for - row
}
// -----------------------------------------------------------
void Environment::AcceptAction(Agent::ActionType action)
{
	bump_ = false;

	switch (action)
	{
		case Agent::actEXTRACT:
			if(maze_[agentPosX_][agentPosY_] > 0){
                collected_+=maze_[agentPosX_][agentPosY_];
				maze_[agentPosX_][agentPosY_] = 0;
				if (maze2_[agentPosX_][agentPosY_]<4){
                    maze2_[agentPosX_][agentPosY_]=0;
				}
			}
            trufa_size_= maze_[agentPosX_][agentPosY_];
			break;
		case Agent::actSNIFF:
            trufa_size_= maze_[agentPosX_][agentPosY_];
			break;
		case Agent::actFORWARD:
		    switch(agentOriented_){
                case 0: // Orientacion Norte
			            if (maze_[agentPosX_-1][agentPosY_] != OBSTACLE) agentPosX_-=1;
			            else bump_ = true;
			            break;
			    case 1: // Orientacion Este
			            if (maze_[agentPosX_][agentPosY_+1] != OBSTACLE) agentPosY_+=1;
			            else bump_ = true;
			            break;
                case 2: // Orientacion Sur
			            if (maze_[agentPosX_+1][agentPosY_] != OBSTACLE) agentPosX_+=1;
			            else bump_ = true;
                        break;
                case 3: // Orientacion Oeste
			            if (maze_[agentPosX_][agentPosY_-1] != OBSTACLE) agentPosY_-=1;
			            else bump_ = true;
                        break;

		    }
		    if (!bump_)
                trufa_size_ = -1;
		    break;
		case Agent::actTURN_L:
		    agentOriented_ = (agentOriented_+3)%4;
			break;
		case Agent::actTURN_R:
		    agentOriented_ = (agentOriented_+1)%4;
			break;
	}
	preAction_=action;
}
// -----------------------------------------------------------
