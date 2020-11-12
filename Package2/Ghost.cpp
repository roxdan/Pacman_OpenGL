/*
Ghost.cpp file

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
void moveGhost();
void draw();

class Ghost {

public:

	float x,y;   //-- Current position
	float vx,vy; //-- Velocity vector
	int state;
	float r, g, b = 0.0;
	int dead = 0;

	long time_remaining;

public:

	Ghost() {
	  state=QUIET;
	}

	void set_position(int x,int y, float r, float g, float b) {
	  this->x = x;
	  this->y = y;
	  this->r = r;
	  this->g = g;
	  this->b = b;
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
	  glColor3f(r,g,b);
	  glBegin(GL_QUADS);
	  glVertex2i(floor(x)-10,floor(y)-10);
	  glVertex2i(floor(x)+10,floor(y)-10);
	  glVertex2i(floor(x)+10,floor(y)+10);
	  glVertex2i(floor(x)-10,floor(y)+10);
	  glEnd();
	}
};