/*
Pacman.cpp file

*/

#include <GL/glut.h>
#include <vector>
#include <cmath>
using namespace std;

#define MOVE 1
#define QUIET 2

////
void set_position(int x,int y);
void init_movement(int destination_x,int destination_y,int duration);
void integrate(long t);
void draw();

class Pacman {

public:

	float x,y;   //-- Current position
	float vx,vy; //-- Velocity vector
	int state;
	int score = 0;

	long time_remaining;

public:

	Pacman() {
	  state=QUIET;
	}

	void set_position(int x,int y) {
	  this->x = x;
	  this->y = y;
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
	      x = x + vx*time_remaining;
	      y = y + vy*time_remaining;
	      state=QUIET;
	    }
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