/*
Pacman.cpp file

*/

#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <iostream>
using namespace std;

#define MOVE 1
#define QUIET 2

////
void set_position(int x,int y);
void init_movement(int destination_x,int destination_y,int duration);
void integrate(long t);
void draw();
int roundUp(int numToRound, int multiple);
void set_cellWidth (int cell_width);

class Pacman {

public:

	float x,y;   //-- Current position
	float vx,vy; //-- Velocity vector
	int state;
	int score = 0;
	int cell_width = 40;

	long time_remaining;

public:

	Pacman() {
	  state=QUIET;
	}

	void set_position(int x,int y) {
	  this->x = x;
	  this->y = y;
	}

	void set_cellWidth(int cell_width) {
		this->cell_width = cell_width;
	}

	//-----------------------------------------------

	void init_movement(int destination_x,int destination_y,int duration) {
	  vx = (destination_x - x)/duration;
	  vy = (destination_y - y)/duration;

	  state=MOVE;
	  time_remaining=duration;
	}

	//-----------------------------------------------

	void integrate(long t) {
	  if(state==MOVE && t<time_remaining)
	    {
	      x = x + vx*t;
	      y = y + vy*t;
	      time_remaining-=t;
	    }
	  else if(state==MOVE && t>=time_remaining)
	    {
	      x = roundUp(x + vx*time_remaining,cell_width)-cell_width/2;
	      y = roundUp(y + vy*time_remaining,cell_width)-cell_width/2;
	    	//x = floor(x + vx*time_remaining);
	    	//y = floor(y + vy*time_remaining);
	      //cout << "x: " << x << "y: " << y << "\n";
	      state=QUIET;
	    }
	}

	int roundUp(int numToRound, int multiple)
	{
		if (multiple == 0)
		    return numToRound;

		int remainder = numToRound % multiple;
		if (remainder == 0)
		    return numToRound;

		return numToRound + multiple - remainder;
	}

	//-----------------------------------------------

	void draw() {
	  glColor3f(1,1,0); //yellow!
	  glBegin(GL_QUADS);
	  glVertex2i(floor(x)-10,floor(y)-10);
	  glVertex2i(floor(x)+10,floor(y)-10);
	  glVertex2i(floor(x)+10,floor(y)+10);
	  glVertex2i(floor(x)-10,floor(y)+10);
	  glEnd();
	}
};