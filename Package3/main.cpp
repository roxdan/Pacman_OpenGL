/*
Maze Algorithm - Pacman game in OpenGL
Recursive backtracking in C++
Danillo Lange
UDL - MINF 20-21
Computer Graphics and Multimedia
main.cpp
*/

// g++ -o pacman main.cpp Maze.cpp Pacman.cpp Ghost.cpp Sphere.cpp-lGL -lglut -lGLU

#include "Maze.cpp"
#include "Pacman.cpp"
#include "Ghost.cpp"
#include "Sphere.cpp"
#include <GL/glut.h>
#include <vector>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <jpeglib.h>
#include <thread>

using namespace std;

#define PI 3.1416


// TODO:
// Fix pacman and ghost movement, it starts okay but then he became crazy - In progress


int keyflag=0;
int cell_width = 40;
int width = 20;
int heigth = 19;
int numGhosts = 3;
long last_t=0;
int initX = 2;
int initY = 2;
int anglealpha = 64;
int anglebeta = -15;
//int anglealpha = 0;
//int anglebeta = 0;
float zoomLevel = 0.5f;
float cameraX;
float cameraY;

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
void PositionObserver(float alpha,float beta,int radi);
//Textures
void ReadJPEG(char *filename,unsigned char **image,int *width, int *height);
void LoadTexture(char *filename,int dim);
void zoomFunc(int direction);

Maze maze;
Pacman player;
vector <Ghost> ghost(numGhosts);

SolidSphere sphere(10, 24, 48);
SolidSphere foodSphere(5, 6, 12);


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
    thread t1(moveGhosts);

    cameraX = (cell_width * maze.maze_width);
    cameraY = (cell_width * maze.maze_heigth);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(cell_width * maze.maze_width, cell_width * maze.maze_heigth);
    glutCreateWindow("Pacman maze!");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

    glBindTexture(GL_TEXTURE_2D,0);
    LoadTexture("pared.jpg",64);

    glBindTexture(GL_TEXTURE_2D,1);
    LoadTexture("floor_scaled.jpg",64);

    glBindTexture(GL_TEXTURE_2D,2);
    LoadTexture("roof_scaled.jpg",64);

    //t1.join();
    glutMainLoop();
    return 0;
 }

 void PositionObserver(float alpha,float beta,int radi) {
  float x,y,z;
  float upx,upy,upz;
  float modul;

  x = (float)radi*cos(alpha*2*PI/360.0)*cos(beta*2*PI/360.0);
  y = (float)radi*sin(beta*2*PI/360.0);
  z = (float)radi*sin(alpha*2*PI/360.0)*cos(beta*2*PI/360.0);

  if (beta>0)
    {
      upx=-x;
      upz=-z;
      upy=(x*x+z*z)/y;
    }
  else if(beta==0)
    {
      upx=0;
      upy=1;
      upz=0;
    }
  else
    {
      upx=x;
      upz=z;
      upy=-(x*x+z*z)/y;
    }


  modul=sqrt(upx*upx+upy*upy+upz*upz);

  upx=upx/modul;
  upy=upy/modul;
  upz=upz/modul;

  gluLookAt(x,y,z,    (maze.maze_width*cell_width)/2, (maze.maze_heigth*cell_width)/2, cell_width,     upx,upy,upz);
}

 void display() {

 	glClearColor(1.0,1.0,1.0,0.0);
  	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  	glMatrixMode(GL_MODELVIEW);
 	glLoadIdentity();
 	
 	PositionObserver(anglealpha,anglebeta,600);

  	glMatrixMode(GL_PROJECTION);
  	glLoadIdentity();
  	
  	glOrtho((-cameraX*0.6) - zoomLevel * 2,
  		(cameraX*0.6) + zoomLevel * 2,
  		(-cameraY*0.6) - zoomLevel * 2,
  		(cameraY*0.6) + zoomLevel * 2,
  		10,2000);
	
 	glMatrixMode(GL_MODELVIEW);

  	glPolygonMode(GL_FRONT,GL_FILL);
  	glPolygonMode(GL_BACK,GL_FILL);
  	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  	//gluPerspective(150, float(cell_width * maze.maze_width) / float(cell_width * maze.maze_heigth), 1, 10);

	displayMaze();
	displayFood();

	sphere.draw(player.x, player.y, cell_width/2, 1, 1, 0);

	printScore();
	
	for (int i = 0 ; i < ghost.size() ; i++) sphere.draw(ghost[i].x, ghost[i].y, cell_width/2, ghost[i].r, ghost[i].g, ghost[i].b);

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
  		//cout << "Player X: " << player.x << "Player Y:" << player.y << "\n";
  		usleep(20000);
  
  	} else if ((c == 'd') && (collisionWall(player.x+cell_width, player.y))) {
	  		player.init_movement(player.x+cell_width, player.y,160);
	  		collisionGhost(player.x/cell_width, player.y/cell_width);
	  		collisionFood(player.x/cell_width, player.y/cell_width);
	  		//cout << "Player X: " << player.x << "Player Y:" << player.y << "\n";
	  		usleep(20000);
  		
  	} else if ((c == 'a') && (collisionWall(player.x-1, player.y))) {
	  		player.init_movement(player.x-cell_width,player.y,160);
	  		collisionFood(player.x/cell_width, player.y/cell_width);
			collisionGhost(floor(player.x/cell_width), floor(player.y/cell_width));
			//cout << "Player X: " << player.x << "Player Y:" << player.y << "\n";
			usleep(20000);
		
  	} else if ((c == 's') && (collisionWall(player.x, player.y-1))) {
	  		player.init_movement(player.x,player.y-cell_width,160);
	  		collisionFood(player.x/cell_width, player.y/cell_width);
	  		collisionGhost(floor(player.x/cell_width), floor(player.y/cell_width));
	  		//cout << "Player X: " << player.x << "Player Y:" << player.y << "\n";
	  		usleep(20000);
  		
  	}

  	if (c=='i' && anglebeta<=(90-4))
    	anglebeta=(anglebeta+3);
  	else if (c=='k' && anglebeta>=(-90+4))
    	anglebeta=anglebeta-3;
  	else if (c=='j')
    	anglealpha=(anglealpha+3)%360;
  	else if (c=='l')
    	anglealpha=(anglealpha-3+360)%360;
    else if (c=='q') {
    	anglealpha = 64;
		anglebeta = -15;
    } else if (c=='z') zoomFunc(1); //zoom out
    else if (c=='x') zoomFunc(2); // zoom in
    else if (c=='c') zoomFunc(3); // reset zoom

 	glutPostRedisplay();
};

void zoomFunc(int direction) {

	if (direction==1) {
		cameraX += (cameraX - cell_width * maze.maze_width) * 0.5f;
		cameraY += (cameraY - cell_width * maze.maze_heigth) * 0.5f;
    	zoomLevel += 2.5f;

	} else if (direction==2) {
		cameraX -= (cameraX - cell_width * maze.maze_width) * 0.5f;
		cameraY -= (cameraY - cell_width * maze.maze_heigth) * 0.5f;
    	zoomLevel -= 2.5f;
	} else if (direction==3) {
		cameraX = (cell_width * maze.maze_width);
		cameraY = (cell_width * maze.maze_heigth);
		zoomLevel = 1.5f;
	}
}

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

	int aux = 0;
	
	while(1) {

		usleep(400000);
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
			if (aux <=3) {
	    		ghost[j].init_movement(ghost[j].x,ghost[j].y-cell_width,160);
	    		aux++;
	    	} else {
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
	}
}

void displayFood() {
	for (int i = 0 ; i < maze.pairFood.size() ; i++) {
		glBegin(GL_QUADS);
		glEnd(); 
		foodSphere.draw((maze.pairFood[i].first * cell_width) + (cell_width/2), (maze.pairFood[i].second * cell_width) + (cell_width/2), cell_width/2, 0, 1, 0);
	}
}

void displayMaze() {

	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
            
    for (int x = 0; x < maze.maze_width; x++) {
		for (int y = 0; y < maze.maze_heigth; y++) {
			if (maze.maze_grid[maze.translateXY(x,y)] == '#' || maze.maze_grid[maze.translateXY(x,y)] == '1') {
				
				/*
				P4 <-- P3
				        ^
				        |
				P1 --> P2
				*/

				
				//glColor3f(0.0,0.0,255.0); // Blue
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,2);
				glBegin(GL_QUADS);
					glTexCoord2f(-0.25,0.0); glVertex3i(x * (cell_width), y * (cell_width),cell_width); // P1 bottom left
				    glTexCoord2f(0.25,0.0); glVertex3i(x * (cell_width) + cell_width, y * (cell_width), cell_width); // P2 bottom right 
				    glTexCoord2f(0.25,0.25); glVertex3i(x * (cell_width) + cell_width, y * (cell_width) + cell_width, cell_width); // P3 top right
				    glTexCoord2f(-0.25,0.25); glVertex3i(x * (cell_width), y * (cell_width) + cell_width, cell_width); // P4 top left
				glEnd();
				glDisable(GL_TEXTURE_2D);

				//glColor3f(255.0,0.0,0.0); // Red	
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,0);
				glBegin(GL_QUADS);
				    glTexCoord2f(-1.0,0.0); glVertex3i(x * (cell_width), y * (cell_width),0); // P1 bottom left
				    glTexCoord2f(1.0,0.0); glVertex3i(x * (cell_width) + cell_width, y * (cell_width), 0); // P2 bottom right 
				    glTexCoord2f(1.0,1.0); glVertex3i(x * (cell_width) + cell_width, y * (cell_width) + cell_width, 0); // P3 top right
				    glTexCoord2f(-1.0,1.0); glVertex3i(x * (cell_width), y * (cell_width) + cell_width, 0); // P4 top left
				glEnd();
				glDisable(GL_TEXTURE_2D);
				
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,0);
				//glColor3f(0.0,255.0,0.0); // Green	
				glBegin(GL_QUADS);
					glTexCoord2f(-1.0,0.0);  glVertex3i(x * (cell_width) + cell_width, y * (cell_width), 0); // P1
					glTexCoord2f(1.0,0.0); glVertex3i(x * (cell_width) + cell_width, y * (cell_width) + cell_width, 0); // P2
					glTexCoord2f(1.0,1.0); glVertex3i(x * (cell_width) + cell_width, y * (cell_width) + cell_width, cell_width); // P3
					glTexCoord2f(-1.0,1.0); glVertex3i(x * (cell_width) + cell_width, y * (cell_width), cell_width); // P4
				glEnd();
				glDisable(GL_TEXTURE_2D);

				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,0);
				//glColor3f(255.0,255.0,0.0); // Yellow	
				glBegin(GL_QUADS);
				    glTexCoord2f(-1.0,0.0); glVertex3i(x * (cell_width), y * (cell_width),0); // P1 
				    glTexCoord2f(1.0,0.0); glVertex3i(x * (cell_width) + cell_width, y * (cell_width), 0); // P2 
				    glTexCoord2f(1.0,1.0); glVertex3i(x * (cell_width) + cell_width, y * (cell_width), cell_width); // P3
				    glTexCoord2f(-1.0,1.0); glVertex3i(x * (cell_width), y * (cell_width),cell_width); // P4
				glEnd();
				glDisable(GL_TEXTURE_2D);

				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,0);
				//glColor3f(255.0,0.0,255.0); // Pink	
				glBegin(GL_QUADS);
					glTexCoord2f(-1.0,0.0); glVertex3i(x * (cell_width), y * (cell_width) + cell_width, 0); // P1
					glTexCoord2f(1.0,0.0); glVertex3i(x * (cell_width), y * (cell_width),0); // P2
					glTexCoord2f(1.0,1.0); glVertex3i(x * (cell_width), y * (cell_width),cell_width); // P3
					glTexCoord2f(-1.0,1.0); glVertex3i(x * (cell_width), y * (cell_width) + cell_width, cell_width); // P4
				glEnd();
				glDisable(GL_TEXTURE_2D);

				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,0);
				//glColor3f(255.0,0.0,255.0); // Pink	
				glBegin(GL_QUADS);
					glTexCoord2f(1.0,0.0); glVertex3i(x * (cell_width) + cell_width, y * (cell_width) + cell_width, 0); // P1
					glTexCoord2f(-1.0,0.0); glVertex3i(x * (cell_width), y * (cell_width) + cell_width, 0); // P2
					glTexCoord2f(-0.5,0.5); glVertex3i(x * (cell_width), y * (cell_width) + cell_width, cell_width); // P3
					glTexCoord2f(0.5,0.5); glVertex3i(x * (cell_width) + cell_width, y * (cell_width) + cell_width, cell_width); // P4
				glEnd();
				glDisable(GL_TEXTURE_2D);

			}
			else if (maze.maze_grid[maze.translateXY(x,y)] == ' ' || maze.maze_grid[maze.translateXY(x,y)] == '0') {
				//glColor3f(0.0,0.0,0.0); // Black
				glBindTexture(GL_TEXTURE_2D,1);
				glEnable(GL_TEXTURE_2D);
				glBegin(GL_QUADS);
					glTexCoord2f(-0.5,0.0); glVertex2i(x * (cell_width), y * (cell_width)); // P1 bottom left
					glTexCoord2f(0.5,0.0); glVertex2i(x * (cell_width) + cell_width, y * (cell_width)); // P2 bottom right 
					glTexCoord2f(0.5,0.5); glVertex2i(x * (cell_width) + cell_width, y * (cell_width) + cell_width); // P3 top right
					glTexCoord2f(-0.5,0.5); glVertex2i(x * (cell_width), y * (cell_width) + cell_width); // P4 top left
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}
			else if (maze.maze_grid[maze.translateXY(x,y)] == 'L')
				glColor3f(0.0,255.0,0.0); // Green

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

	if((initX >=2 && initY >=2) && (initX <= width-2 && initY <= heigth-2) ) {
    	
		for (int i = 0; i < maze.pairFood.size() ; i ++) {
			if (maze.pairFood[i].first == initX && maze.pairFood[i].second == initY) {
				initOK = 1;
			} else {

				if(maze.pairFood[i].first == initX+1 && maze.pairFood[i].second == initY) {
	    			initX = initX+1;
	    			return;

	    		} else if(maze.pairFood[i].first == initX && maze.pairFood[i].second == initY+1) {
	    			initY = initY+1;
	    			return;

	    		} else if(maze.pairFood[i].first == initX-1 && maze.pairFood[i].second == initY) {
	    			initX = initX-1;
	    			return;

	    		} else if(maze.pairFood[i].first == initX && maze.pairFood[i].second == initY-1) {
	    			initY = initY-1;
	    			return;
	    		}

			}

		} 

	} else {
		cout << "Invalid parameters, initializing default location." << "\n";
		initX = 2;
		initY = 2;
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
      

      //moveGhosts();


      for (int i = 0 ; i < numGhosts ; i++){
      	//ghost[i].init_movement(player.x, player.y, 5);
      	ghost[i].integrate(t-last_t);
      }

      last_t=t;
    }

  	//moveGhosts();

  glutPostRedisplay();
  //usleep(10000);
}

bool collisionWall(int x, int y) {
	// if its not a wall return true

	int xn = (x - (cell_width/2))/cell_width;
	int yn = (y - (cell_width/2))/cell_width;

	if (maze.maze_grid[translateXY(xn,yn)] == ' ') {
		return true;
	} 
	else {
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

// Texture functions

void ReadJPEG(char *filename,unsigned char **image,int *width, int *height)
{
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE * infile;
  unsigned char **buffer;
  int i,j;

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);


  if ((infile = fopen(filename, "rb")) == NULL) {
    printf("Unable to open file %s\n",filename);
    exit(1);
  }

  jpeg_stdio_src(&cinfo, infile);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_calc_output_dimensions(&cinfo);
  jpeg_start_decompress(&cinfo);

  *width = cinfo.output_width;
  *height  = cinfo.output_height;


  *image=(unsigned char*)malloc(cinfo.output_width*cinfo.output_height*cinfo.output_components);

  buffer=(unsigned char **)malloc(1*sizeof(unsigned char **));
  buffer[0]=(unsigned char *)malloc(cinfo.output_width*cinfo.output_components);


  i=0;
  while (cinfo.output_scanline < cinfo.output_height) {
    jpeg_read_scanlines(&cinfo, buffer, 1);

    for(j=0;j<cinfo.output_width*cinfo.output_components;j++)
      {
	(*image)[i]=buffer[0][j];
	i++;
      }   

    }

  free(buffer);
  jpeg_finish_decompress(&cinfo);
} 



/*--------------------------------------------------------*/
/*--------------------------------------------------------*/
void LoadTexture(char *filename,int dim)
{
  unsigned char *buffer;
  unsigned char *buffer2;
  int width,height;
  long i,j;
  long k,h;

  ReadJPEG(filename,&buffer,&width,&height);

  buffer2=(unsigned char*)malloc(dim*dim*3);

  //-- The texture pattern is subsampled so that its dimensions become dim x dim --
  for(i=0;i<dim;i++)
    for(j=0;j<dim;j++)
      {
	k=i*height/dim;
	h=j*width/dim;
	
	buffer2[3*(i*dim+j)]=buffer[3*(k*width +h)];
	buffer2[3*(i*dim+j)+1]=buffer[3*(k*width +h)+1];
	buffer2[3*(i*dim+j)+2]=buffer[3*(k*width +h)+2];

      }

  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,dim,dim,0,GL_RGB,GL_UNSIGNED_BYTE,buffer2);

  free(buffer);
  free(buffer2);
}