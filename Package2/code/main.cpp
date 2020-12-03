/*
Maze Algorithm - Pacman game in OpenGL
Recursive backtracking in C++
Danillo Lange
UDL - MINF 20-21
Computer Graphics and Multimedia
main.cpp
*/

// g++ -o pacman main.cpp Maze.cpp Pacman.cpp Ghost.cpp -lGL -lglut -lGLU

#include "Maze.cpp"
#include "Pacman.cpp"
#include "Ghost.cpp"
#include <GL/glut.h>
#include <vector>
#include <utility>
#include <unistd.h>
#include <string.h>
using namespace std;


// TODO:
// Fix pacman movement, it starts okay but then he became crazy - In progress
// Remove deadends of the maze - OK
// Fix ghost movimentation - OK
// Create a function to show the score, it should increase when you eat a fruit (and the fruit should disappear) - OK


int keyflag=0;
int cell_width = 40;
int width = 20;
int heigth = 19;
int numGhosts = 3;
long last_t=0;
int initX = 2;
int initY = 2;

void display();
void displayMaze();
void displayFood();
void keyboard(unsigned char c,int x,int y);
void checkUserInput();
bool collisionWall(int x, int y);
void idle();
int translateXY(int x, int y);
void moveGhosts();
void collisionFood(int x, int y);
void collisionGhost(int x, int y);
void printScore();
void initializeGhosts();
void checkStartInput(); // with some bugs

Maze maze;
Pacman player;
vector <Ghost> ghost(numGhosts);

int main(int argc,char *argv[]) {
    // Feed the srand
    srand(time(NULL));

    checkUserInput();

    maze.mazeGenerate(width, heigth);
    cout << "\n" << "Parameters used: " << width << " columns and " << heigth << " rows." << "\n";
    cout << "Num of ghosts: " << numGhosts << "\n";
    checkStartInput();

    // Initialize pacman
    player.set_position((initX * cell_width)-cell_width/2,(initY*cell_width)-cell_width/2);
    
    // Initialize ghosts
    initializeGhosts();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(cell_width * maze.maze_width, cell_width * maze.maze_heigth);
    glutCreateWindow("Pacman maze!");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,(cell_width * maze.maze_width)-1,0,(cell_width * maze.maze_heigth)-1);
   
    glutMainLoop();
    return 0;
 }

 void display() {

 	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	displayMaze();
	displayFood();
	player.draw();
	printScore();
	
	for (int i = 0 ; i < ghost.size() ; i++) ghost[i].draw();

	glutSwapBuffers();
}


void keyboard(unsigned char c,int x,int y) {
	// If you press r it runs the algorithm again and print it again
	if (c == 'r') {
		maze.mazeGenerate(width, heigth);
		player.score = 0;
		cout << "\n" << "Parameters used: " << width << " columns and " << heigth << " rows." << "\n";
 	}

 	//player.set_position(1,1);]
 	// Basic movimentation

  	if ((c == 'w') && collisionWall(player.x,player.y+cell_width)) {
  		player.init_movement(player.x,player.y+cell_width,160);
  		collisionGhost(floor(player.x/cell_width), floor(player.y/cell_width));
  		collisionFood(player.x/cell_width, player.y/cell_width);
  
  	} else if ((c == 'd') && (collisionWall(player.x+cell_width, player.y))) {
	  		player.init_movement(player.x+cell_width,player.y,160);
	  		collisionGhost(floor(player.x/cell_width), floor(player.y/cell_width));
	  		collisionFood(player.x/cell_width, player.y/cell_width);
  		
  	} else if ((c == 'a') && (collisionWall(player.x-cell_width, player.y))) {
	  		player.init_movement(player.x-cell_width,player.y,160);
	  		collisionFood(player.x/cell_width, player.y/cell_width);
			collisionGhost(floor(player.x/cell_width), floor(player.y/cell_width));
		
  	} else if ((c == 's') && (collisionWall(player.x, player.y-cell_width))) {
	  		player.init_movement(player.x,player.y-cell_width,160);
	  		collisionFood(player.x/cell_width, player.y/cell_width);
	  		collisionGhost(floor(player.x/cell_width), floor(player.y/cell_width));
  		
  	}

  	moveGhosts();

 	glutPostRedisplay();
};

void initializeGhosts() {
    for (int i = 0 ; i < numGhosts; i++) {

		float r, g, b = 0.0;

		// set the colors of the ghosts
		if (i == 0){
			r = 0;
			g = 0;
			b = 255;
		} else if (i == 1) {
			r = 255;
			g = 0;
			b = 0;
		} else if (i == 2) {
			r = 0;
			g = 255;
			b = 0;
		}

		ghost[i].set_position((maze.spawn[i].first*cell_width)-cell_width/2, (maze.spawn[i].second*cell_width)-cell_width/2,r,g,b);
	}
}

void moveGhosts() {

	//usleep(1000000);

	int direction[4];
    direction[0] = 0;
    direction[1] = 1;
    direction[2] = 2;
    direction[3] = 3;

    for (int i=0; i<4; ++i) {
        int r = rand() & 3;
        int temp = direction[r];
        direction[r] = direction[i];
        direction[i] = temp;
    }


    for (int j = 0; j < numGhosts ; j++) {
		switch (direction[rand()&3])
		{
		    case 0: 
				if ((collisionWall(ghost[j].x, ghost[j].y+cell_width))) ghost[j].init_movement(ghost[j].x,ghost[j].y+cell_width,160);
		        break;
		    case 1:
				if ((collisionWall(ghost[j].x+cell_width, ghost[j].y))) ghost[j].init_movement(ghost[j].x+cell_width,ghost[j].y,160);
		        break;
		    case 2: 
		        if ((collisionWall(ghost[j].x-cell_width, ghost[j].y))) ghost[j].init_movement(ghost[j].x-cell_width,ghost[j].y,160);
		        break;
		    case 3: 
		        if ((collisionWall(ghost[j].x, ghost[j].y-cell_width))) ghost[j].init_movement(ghost[j].x,ghost[j].y-cell_width,160);
		        break;
		    	
		}
	}    
    
}

void displayFood() {

	glColor3f(0.0,255.0,0.0); // Green


	for (int i = 0 ; i < maze.pairFood.size() ; i++) {
			glBegin(GL_QUADS);
					glVertex2i((maze.pairFood[i].first * cell_width) + (cell_width/2) - 5, (maze.pairFood[i].second * cell_width) + (cell_width/2) - 5);
					glVertex2i((maze.pairFood[i].first * cell_width) + (cell_width/2) + 5, (maze.pairFood[i].second * cell_width) + (cell_width/2) - 5);
					glVertex2i((maze.pairFood[i].first * cell_width) + (cell_width/2) + 5, (maze.pairFood[i].second * cell_width) + (cell_width/2) + 5);
					glVertex2i((maze.pairFood[i].first * cell_width) + (cell_width/2) - 5, (maze.pairFood[i].second * cell_width) + (cell_width/2) + 5);
			glEnd();
	}
}

void displayMaze() {

	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT);
            
    for (int x = 0; x < maze.maze_width; x++) {
		for (int y = 0; y < maze.maze_heigth; y++) {
			if (maze.maze_grid[maze.translateXY(x,y)] == '#' || maze.maze_grid[maze.translateXY(x,y)] == '1') {
				glColor3f(0.0,0.0,255.0); // Black

			}
			else if (maze.maze_grid[maze.translateXY(x,y)] == ' ' || maze.maze_grid[maze.translateXY(x,y)] == '0')
				glColor3f(0.0,0.0,0.0); // White
			else if (maze.maze_grid[maze.translateXY(x,y)] == 'L')
				glColor3f(0.0,255.0,0.0); // Green

			glBegin(GL_QUADS);
				glVertex2i(x * (cell_width), y * (cell_width)); // top left
				glVertex2i(x * (cell_width) + cell_width, y * (cell_width)); // top right 
				glVertex2i(x * (cell_width) + cell_width, y * (cell_width) + cell_width); // bottom right
				glVertex2i(x * (cell_width), y * (cell_width) + cell_width); // bottom left
			glEnd();
		}
	}
}

void checkUserInput() {

	int check=0;

    // do some checking about the constraints and window size
	cout << "Hi! The perfect size is 20 width and 19 height! \n";
	while(check==0) {
    	cout <<"Insert width (pair): \n";
   	 	cin >> width;
    	cout <<"Insert heigth (odd): \n";
    	cin >> heigth;
    	cout << "Num of ghosts? (1-3) \n";
    	cin >> numGhosts;

    	if ((width%2==0 || heigth%2==1) && (width > 10 && heigth > 10) && (width < 55 && heigth < 55)){ 
    		if (heigth > 20) {
    			cell_width = floor(600/heigth);
    			//cell_width = 30;
    		} else
    			cell_width = 40;

    		check=1;
    	}
    	else {
    		cout << "Please correct the inputs! The width should be pair and the heigth odd, also greater than 10 and smaller than 55\n";
    	}


    }
}

void checkStartInput() {

	cout << "Player starting position x (min 2 and max " << width-2 << "): \n";
    cin >> initX;
    cout << "Player starting position y (min 2 and max " << heigth-2 << "): \n";
    cin >> initY;

    int initOK = 0;

	if(initX >=2 && initY >=2) {
    	
		for (int i = 0; i < maze.pairFood.size() ; i ++) {
			if (maze.pairFood[i].first == initX && maze.pairFood[i].second == initY) {
				//cout << "Está okay! \n";
				initOK = 1;
			} else {

				if(maze.pairFood[i].first == initX+1 && maze.pairFood[i].second == initY) {
					//cout << "kinda okay! 1\n";
	    			initX = initX+1;
	    			return;

	    		} else if(maze.pairFood[i].first == initX && maze.pairFood[i].second == initY+1) {
	    			//cout << "kinda okay! 2\n";
	    			initY = initY+1;
	    			return;

	    		} else if(maze.pairFood[i].first == initX-1 && maze.pairFood[i].second == initY) {
	    			//cout << "kinda okay! 3\n";
	    			initX = initX-1;
	    			return;

	    		} else if(maze.pairFood[i].first == initX && maze.pairFood[i].second == initY-1) {
	    			//cout << "kinda okay! 4\n";
	    			initY = initY-1;
	    			return;
	    		}

			}

		} 

	}
}

void idle()
{
  long t;

  t=glutGet(GLUT_ELAPSED_TIME); 

  if(last_t==0)
    last_t=t;
  else
    {
      player.integrate(t-last_t);
      
      for (int i = 0 ; i < numGhosts ; i++) ghost[i].integrate(t-last_t);

      last_t=t;
    }

  	//moveGhosts();

  glutPostRedisplay();
}

bool collisionWall(int x, int y) {
	// if its not a wall return true
	if (maze.maze_grid[translateXY(x/cell_width,y/cell_width)] == ' ' || maze.maze_grid[translateXY(x/cell_width,y/cell_width)] == '0') {
		//cout << "True! \n";
		return true;
	} 
	else {
		//cout << "False! \n";
		return false;
	}
}

void collisionFood(int x, int y) {

	for (int i = 0; i < maze.pairFood.size(); i++)
		if (x == maze.pairFood[i].first && y == maze.pairFood[i].second) {
			maze.pairFood.erase(maze.pairFood.begin() + i);
			player.score ++;
		}

}

void collisionGhost(int x, int y) {
	for (int i = 0 ; i < ghost.size() ; i ++ ) {
		//cout << "Player: " << x << ", " << y << "\n";
		//cout << "Ghost: " << i << " " << floor(ghost[i].x/cell_width) << ", " << floor(ghost[i].y/cell_width) << "\n";
		if (x == floor(ghost[i].x/cell_width) && y == floor(ghost[i].y/cell_width)) {
			//ghost.erase(ghost.begin() + i);
			//cout << "Ghost " << i+1 << " Erased!\n";
			cout << "Game over! You were touched by a ghost.\n";
		}
	}
}

int translateXY(int x, int y){
        return (y * width + x );
}
// print score function
void printScore() {
	glColor3f(0.0,255.0,0.0);
	glRasterPos2f(1 * cell_width, 1 * cell_width/2);
	int len, i;

	std::string text = "Score: ";
	text += std::to_string(player.score);

	len = text.size();

	for(i=0; i < len ; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
}