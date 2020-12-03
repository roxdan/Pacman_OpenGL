/*
Ghost.cpp file

*/

#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <time.h>
using namespace std;

#define MOVE 1
#define QUIET 2

////
void set_position(int x,int y);
void init_movement(int destination_x,int destination_y,int duration);
void integrate(long t);
void moveGhost();
void draw();
int roundUp(int numToRound, int multiple);
void set_cellWidth (int cell_width);

class Ghost {

public:

	float x,y;   //-- Current position
	float vx,vy; //-- Velocity vector
	int state;
	float r, g, b = 0.0;
	int dead = 0;
	int cell_width = 40;

	long time_remaining;

public:

	Ghost() {
	  state=QUIET;
	  
	//GLUquadric *quadratic = gluNewQuadric();

	//if( !quadratic){
	//    throw new PROGRAM_EXCEPTION( "Cannot initialize quadartic", NULL);
	//}
	//gluQuadricNormals(quadratic, GLU_SMOOTH);
	
	}

	void set_position(int x,int y, float r, float g, float b) {
	  this->x = x;
	  this->y = y;
	  this->r = r;
	  this->g = g;
	  this->b = b;
	}

	void set_cellWidth (int cell_width) {
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
	      x = roundUp(x + vx*time_remaining,40)-20;
	      y = roundUp(y + vy*time_remaining,40)-20;
	      state=QUIET;
	    }
	}

	int roundUp(int numToRound, int multiple) {
		if (multiple == 0)
		    return numToRound;

		int remainder = numToRound % multiple;
		if (remainder == 0)
		    return numToRound;

		return numToRound + multiple - remainder;
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
	  /*
	glPushMatrix();
		glTranslatef(floor(x), floor(y), 10);
		gluSphere( quadratic, 10.0f, 16, 16); // Tried r = 0.02f, 0.2f, 2.0f, 20.0f, none works
	glPopMatrix();
	*/
	}
};