#define _USE_MATH_DEFINES
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <cmath>
#include <GL/glut.h>

class SolidSphere
{
protected:
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texcoords;
    std::vector<GLushort> indices;
    

public:
    GLfloat ambient[4] = {};
    GLfloat diffuse[4] = {};
    GLfloat specular[4] = {};
    GLfloat emission[4] = {};
    GLfloat shininess = 0;

    SolidSphere(float radius, unsigned int rings, unsigned int sectors)
    {   
        float const R = 1./(float)(rings-1);
        float const S = 1./(float)(sectors-1);
        int r, s;

        vertices.resize(rings * sectors * 3);
        normals.resize(rings * sectors * 3);
        texcoords.resize(rings * sectors * 2);
        std::vector<GLfloat>::iterator v = vertices.begin();
        std::vector<GLfloat>::iterator n = normals.begin();
        std::vector<GLfloat>::iterator t = texcoords.begin();
        for(r = 0; r < rings; r++) for(s = 0; s < sectors; s++) {
                float const y = sin( -M_PI_2 + M_PI * r * R );
                float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
                float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );

                *t++ = s*S;
                *t++ = r*R;

                *v++ = x * radius;
                *v++ = y * radius;
                *v++ = z * radius;

                *n++ = x;
                *n++ = y;
                *n++ = z;
        }

        indices.resize(rings * sectors * 4);
        std::vector<GLushort>::iterator i = indices.begin();
        for(r = 0; r < rings; r++) for(s = 0; s < sectors; s++) {
                *i++ = r * sectors + s;
                *i++ = r * sectors + (s+1);
                *i++ = (r+1) * sectors + (s+1);
                *i++ = (r+1) * sectors + s;
        }
    }

    void draw(GLfloat x, GLfloat y, GLfloat z, int r, int g, int b)
    {
        //glColor3f(r,g,b); //yellow!

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(x,y,z);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
        glNormalPointer(GL_FLOAT, 0, &normals[0]);
        glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);
        glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
        glPopMatrix();
    }
};