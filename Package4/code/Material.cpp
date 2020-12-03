#include <GL/glut.h>
#include <vector>
#include <math.h>
#include <unistd.h>

class Material {
    public:
        GLfloat ambient[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat diffuse[4] = {0.8f, 0.8f, 0.8f, 1.0f};
        GLfloat specular[4]= {0.5f, 0.5f, 0.5f, 1.0f};
        GLfloat emission[4] = {0.2f, 0.2f, 0.2f, 1.0f};
        GLfloat shininess = 1;

    void apply() {
        glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, this->ambient);
        glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, this->diffuse);
        glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, this->specular);
        glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, this->emission);
        glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, this->shininess);
    }
};