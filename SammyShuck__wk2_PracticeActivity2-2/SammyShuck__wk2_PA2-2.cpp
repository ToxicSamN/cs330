/*
 * Author: Sammy Shuck
 * Name: Practice Activity 2-2
 * Week: Two
 * Assignment: Module Two Practice Activity 2-2
 * Course: CS-330-J2956 Comp Graphic and Visualization 20EW2
 * Date: 11/8/2020
 * Version: 1.0
 * Copyright: Copyright © 2017 SNHU COCE
 * C++ Version: 20
 */

#include <GL/glut.h> // include the freeGLUT header file

/* Displays Grid lines at 0.1 intervals */
void draw3DGridLines() {
    float gspacing = 0.1f;
    // starting Gridlines
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    // horizontal lines
    // horizontal x=-1, y=1 | x=x*-1 y=y
    for (float x = -1.0f;x < 1.1f; x += gspacing) {
        for (float y = 1.0f; y > -1.1f; y -= gspacing) {
            glVertex2f(x, y);
            glVertex2f(x * (-1.0f), y);
        }
    }

    // vertical   x=-1, y=1 | x=x    y=y*-1
    for (float y = 1.0f; y > -1.1f; y -= gspacing) {
        for (float x = -1.0f;x < 1.1f; x += gspacing) {
            glVertex2f(x, y);
            glVertex2f(x, y* (-1.0f));
        }
    }

    // horizontal lines
    // horizontal x=-1, y=1 | x=x*-1 y=y
    for (float z = -1.0f;z < 1.1f; z += gspacing) {
        for (float y = 1.0f; y > -1.1f; y -= gspacing) {
            glVertex3f(0, y, z);
            glVertex3f(0, y, z * (-1.0f));
        }
    }

    // vertical   x=-1, y=1 | x=x    y=y*-1
    for (float y = 1.0f; y > -1.1f; y -= gspacing) {
        for (float z = -1.0f;z < 1.1f; z += gspacing) {
            glVertex3f(0, y, z);
            glVertex3f(0, y* (-1.0f), z);
        }
    }
    for (float z = -1.0f;z < 1.1f; z += gspacing) {
        for (float x = 1.0f; x > -1.1f; x -= gspacing) {
            glVertex3f(x, 0, z);
            glVertex3f(x, 0, z * (-1.0f));
        }
    }

    // vertical   x=-1, y=1 | x=x    y=y*-1
    for (float x = 1.0f; x > -1.1f; x -= gspacing) {
        for (float z = -1.0f;z < 1.1f; z += gspacing) {
            glVertex3f(x, 0, z);
            glVertex3f(x* (-1.0f), 0, z);
        }
    }

    glEnd();

    // X,Y 0 Axis
    glBegin(GL_LINES);
    glColor3f(0.0f, 1.0f, 0.0f);
    // x-axis
    glVertex3f(-1.0, 0.0, 0.0f);
    glVertex3f(1.0, 0.0, 0.0f);

    // y-axis
    glVertex3f(0.0, 1.0, 0.0f);
    glVertex3f(0.0, -1.0, 0.0f);

    // z-axis
    glVertex3f(0.0, 0.0f, 1.0);
    glVertex3f(0.0, 0.0f, -1.0);
    glEnd();
}

/*
 * draw3DPolygon draws a 3D shape based on predefined
 * x,y,z coordinates
 */
void draw3DPolygon() {
    // define surface colors of the image
    // this image has 6 surfaces so 6 definitions are required
    // this could be replaced with a random color generator
    GLfloat surfaceColor[6][3] = {
            // red
            {1.0f,0.0f,0.0f},
            // blue
            {0.0f,0.0f,1.0f},
            // green
            {0.0f,1.0f,0.0f},
            //yellow
            {1.0f,1.0f,0.0f},
            //dull green
            {0.0f,0.5f,0.4f},
            // grey
            {0.5f,0.5f,0.5f},
    };

    // 3D shape is predefined as a 6 surface
    // shape with 4 points on each surface
    // and each point has x, y, z coordinates
    GLfloat polygon[6][4][3] = {
            // top of the shape
            {
                    {0.5f,1.0f,1.0f},
                    {-0.5f, 1.0f, 1.0f},
                    {-0.5f, 1.0f, -1.0f},
                    {0.5f,1.0f,-1.0f},

            },
            // front of the shape
            {
                    {0.5f,1.0f,1.0f},
                    {-0.5f, 1.0f, 1.0f},
                    {-1.0f,-1.0f,1.0f},
                    {1.0f,-1.0f,1.0f},
            },
            // right of the shape
            {
                    {1.0f,-1.0f,1.0f},
                    {0.5f,1.0f,1.0f},
                    {0.5f,1.0f,-1.0f},
                    {1.0f,-1.0f,-1.0f},
            },
            // back of the shape
            {
                    {1.0f,-1.0f,-1.0f},
                    {0.5f,1.0f,-1.0f},
                    {-0.5f, 1.0f, -1.0f},
                    {-1.0f,-1.0f,-1.0f},
            },
            // left of the shape
            {
                    {-1.0f,-1.0f,-1.0f},
                    {-0.5f, 1.0f, -1.0f},
                    {-0.5f, 1.0f, 1.0f},
                    {-1.0f,-1.0f,1.0f},

            },
            // bottom of the shape
            {
                    {-1.0f,-1.0f,1.0f},
                    {-1.0f,-1.0f,-1.0f},
                    {1.0f,-1.0f,-1.0f},
                    {1.0f,-1.0f,1.0f},
            },



    };


    // starting delimiter for triangle primitive
    glBegin(GL_QUADS);
    // loop through the surfaces and draw each surface with
    // each surface having a separate color defined in surfaceColor[]
    for (int i = 0; i < 6; i++){
        glColor3f(surfaceColor[i][0],surfaceColor[i][1],surfaceColor[i][2]);
        glVertex3fv(polygon[i][0]);
        glVertex3fv(polygon[i][1]);
        glVertex3fv(polygon[i][2]);
        glVertex3fv(polygon[i][3]);
    }

    // ending delimiter for triangle primitive
    glEnd();
}

/* Implements Display Callback Handler */
void display() {

    // Clears the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Required for transforming (translating, rotating, and scaling) graphic objects
    // Activates the model-view matrix
    glMatrixMode(GL_MODELVIEW);
    // Replaces the current matrix with the identity matrix. i.e. resets or refreshes the matrix for the next frame when the window is resized
    glLoadIdentity();
    // Move model to the center of the world with a z depth of -7
    glTranslatef(-0.5f, 0.0f, -8.0f);

    // Rotate model on x, y, and z
    glRotatef(35.0f,0.8f,-0.6f,-1.0f); // front,top,right
    //glRotatef(150.0f,0.0f,1.0f,0.0f); // back,left
    //glRotatef(150.0f,0.0f,1.0f,-0.4f); //back, left, bottom

    draw3DPolygon();
    // draw3DGridLines();

    // Swap front and back fram buffers (similar to glFlush)
    glutSwapBuffers();
}

/* Sets aspect ration and clipping volume upon window reshape */
void reshape(GLsizei width, GLsizei height){
    if (height == 0) height = 1;
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    glViewport(0,0,width,height);

    // Set the aspect ratio of the clipping volume to match the viewport
    // Create a Projection matrix
    glMatrixMode(GL_PROJECTION);
    // Reset Projection Matrix
    glLoadIdentity();
    // Set perspective projection, fov, aspect, zNear and zFar clipping
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

/* Depth and tesselation settings */
void applyDepthSettings() {
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

/* Main function required for immediate mode */
int main(int argc, char** argv){

    // Initialize the freeGLUT library
    glutInit(&argc, argv);
    // Enable double buffered mode
    glutInitDisplayMode(GLUT_DOUBLE);
    // Create a window and title
    glutCreateWindow("Sammy Shuck - PA2-2");

    // Specify the widows dimensions (width, height)
    glutInitWindowSize(320, 320);
    // Specify the position of the window's top left corner
    glutInitWindowPosition(0,0);

    // Set the display callback for the current window
    glutDisplayFunc(display);
    // Sets the display callback for the window re-size event
    glutReshapeFunc(reshape);

    // Apply depth settings
    applyDepthSettings();

    // Enters the GLUT event processing loop
    glutMainLoop();

    // exits the main function
    return 0;
}