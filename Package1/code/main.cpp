/*
Maze Algorithm
Recursive backtracking in C++
Danillo Lange
UDL - MINF 20-21
main.cpp
*/

#include "Maze.cpp"
#include <GL/glut.h>
#include <vector>
using namespace std;


Maze maze;

void display();;
void keyboard(unsigned char c,int x,int y);

int keyflag=0;
int cell_width = 40;
int width;
int heigth;

int main(int argc,char *argv[]) {
    // Feed the srand
    srand(time(NULL));

    int check=0;

    // do some checking about the constraints and window size
    while(check==0) {
    	cout <<"Insert width: \n";
   	 	cin >> width;
    	cout <<"Insert heigth: \n";
    	cin >> heigth;

    	if ((width%2==1 || heigth%2==1) && (width > 10 && heigth > 10) && (width < 55 && heigth < 55)){ 
    		if (heigth > 20) {
    			cell_width = floor(600/heigth);
    			//cell_width = 30;
    		} else
    			cell_width = 40;

    		check=1;
    	}
    	else {
    		cout << "Please correct the inputs! It must be odds, greater than 10 and smaller than 55\n";
    	}
    }

    maze.mazeGenerate(width, heigth);
    cout << "\n" << "Parameters used: " << width << " columns and " << heigth << " rows." << "\n";

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(cell_width * maze.maze_width, cell_width * maze.maze_heigth);
    glutCreateWindow("Pacman maze!");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,(cell_width * maze.maze_width)-1,0,(cell_width * maze.maze_heigth)-1);
   
    glutMainLoop();
    return 0;
 }

 void display() {
	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT);
            
    for (int x = 0; x < maze.maze_width; x++) {
		for (int y = 0; y < maze.maze_heigth; y++) {
			if (maze.maze_grid[maze.translateXY(x,y)] == '#' || maze.maze_grid[maze.translateXY(x,y)] == '1') {
				glColor3f(0.0,0.0,0.0); // Black

			}
			else if (maze.maze_grid[maze.translateXY(x,y)] == ' ')
				glColor3f(255.0,255.0,255.0); // White

			glBegin(GL_QUADS);
				glVertex2i(x * (cell_width), y * (cell_width)); // top left
				glVertex2i(x * (cell_width) + cell_width, y * (cell_width)); // top right 
				glVertex2i(x * (cell_width) + cell_width, y * (cell_width) + cell_width); // bottom right
				glVertex2i(x * (cell_width), y * (cell_width) + cell_width); // bottom left
			glEnd();
		}
	}
	glutSwapBuffers();
}


void keyboard(unsigned char c,int x,int y) {
	// If you press r it runs the algorithm again and print it again
	if (c == 'r') {
		maze.mazeGenerate(width, heigth);
		cout << "\n" << "Parameters used: " << width << " columns and " << heigth << " rows." << "\n";
 	}

 	glutPostRedisplay();
};
