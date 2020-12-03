/*
Maze Algorithm
Recursive backtracking in C++
Danillo Lange
UDL - MINF 20-21
Maze.cpp
*/

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3


#include <iostream>
#include <time.h>
#include <cmath>
//#include "Maze.h"
using namespace std;

// Functions

void mazeGenerate(int x, int y);
void initializeGrid(int x, int y);
int translateXY(int x, int y);
int checkBounds(int x, int y);
void printMaze();
void addRoom();
void mapMirror();
char step(int x, int y);


class Maze {

    public:        

        int maze_width, maze_heigth;
        char* maze_grid;

    public:

    void mazeGenerate(int x, int y) {
        initializeGrid(x, y);
        step(1,1);
        addRoom();
        mapMirror();
        printMaze();
    }

    //Fills the grid with # (wall)
    void initializeGrid(int x, int y) {

        maze_width = x;
        maze_heigth = y;

        maze_grid = new char [(int)maze_width*(int)maze_heigth];

        for (int i = 0; i < maze_width * maze_heigth; i++) maze_grid[i]= '#';
    }

    // Function to translate x, y coords to the grid (single-dimensional index)
    int translateXY(int x, int y){
        return (y * maze_width + x );
    }

    // Function to check if x and y are within bounds
    int checkBounds (int x, int y) {
        if ((x < 0 || x >= maze_width) || (y < 0 || y >= maze_heigth)) return false;

        return true;
    }

    // Function to print the maze in text mode
    void printMaze() {
        for (int y = 0 ; y < maze_heigth; y++) {
            for (int x = 0; x < maze_width; x++) {
                cout << maze_grid[translateXY(x,y)] << " "; 
            }

        cout << endl;
        }
        cout << endl;
    }

    // Function to buld the room
    void addRoom (){
        int ycenter = floor(maze_heigth/2);
        int xcenter = floor(maze_width/2);

        for (int i = (xcenter-5) ; i < xcenter+5; i++) {
            for (int j = (ycenter-3) ; j <= (ycenter+2); j++) {
                if ((i == (xcenter-4) && j != (ycenter-3) && j != (ycenter+2)) || (j == (ycenter+1) && i != (xcenter-5))) maze_grid[translateXY(i, j)] = '1';
                else if (i == (xcenter-5)) maze_grid[translateXY(i, j)] = ' ';
                else if (j== (ycenter-3)) maze_grid[translateXY(i, j)] = ' ';
                else if ((i == (xcenter-3) && j == (ycenter-2)) || (i == (xcenter-2) && j == (ycenter-2))) maze_grid[translateXY(i, j)] = '#';
                else if (j == (ycenter) || j == (ycenter-1) || j== (ycenter+1)) maze_grid[translateXY(i, j)] = ' ';
                else 
                    maze_grid[translateXY(i, j)] = ' ';
            }
        }
    }
    // Function to mirror the map

    void mapMirror() {

        int draw, blank;

        blank = ceil(maze_width / 2);
        draw = floor(maze_width / 2);
        

        for (int i = 0; i < maze_heigth; i++) {
            for (int j = 0; j < draw; j++) {
                maze_grid[translateXY((blank+j), i)] = maze_grid[translateXY((draw-1-j),i)];

            }
        }
    }

    // Recursive Algorithm

    char step(int x, int y) {
        int direction[4];
        direction[0] = NORTH;
        direction[1] = EAST;
        direction[2] = SOUTH;
        direction[3] = WEST;

        // shuffle the desired direction
        for (int i=0; i<4; ++i) {
            int r = rand() & 3;
            int temp = direction[r];
            direction[r] = direction[i];
            direction[i] = temp;
        }

        maze_grid[translateXY(x,y)] = ' ';

        // Loop to attempt to visit that direction
        for (int i=0 ; i<4 ; i++){

            //Initialize aux variables
            int dx=0, dy=0;


            switch (direction[i])
            {
                case NORTH: 
                    dy = -1; 
                    break;
                case SOUTH: 
                    dy = 1; 
                    break;
                case EAST: 
                    dx = 1; 
                    break;
                case WEST: 
                    dx = -1; 
                    break;
            }

            // Aux variables to offset
            // So basically, if dx/dy = 1, the shift turns it into a 2
            // if dx/dy = -1, the shift turns into a -2
            int x2 = x + (dx<<1);
            int y2 = y + (dy<<1);

            if (checkBounds(x2, y2)) {
                if (maze_grid[translateXY(x2, y2)] == '#') {
                    maze_grid[translateXY(x2-dx, y2-dy)] = ' ';

                    // repeat recursively with the new rows and columns value
                    step(x2, y2);
                }
            }
            
        }
        return *maze_grid;
    }
};
