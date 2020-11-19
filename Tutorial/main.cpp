/*
 * Author: Sammy Shuck
 * Name: Cube
 * Week: Two
 * Assignment: Module Two Tutorial
 * Course: CS-330-J2956 Comp Graphic and Visualization 20EW2
 * Date: 11/1/2020
 * Version: 1.0
 * Copyright: Copyright © 2017 SNHU COCE
 */

#include <GL/gl.h> // include the freeGLUT header file
#include <GL/glu.h> // include the freeGLUT header file
#include <GL/glut.h> // include the freeGLUT header file
#include <GL/freeglut.h> // include the freeGLUT header file
#include <math.h>

struct Quadrant {
    float x;
    float y;
};

struct QuandrantShift {
    Quadrant Q1{}, Q2{}, Q3{}, Q4{};

    QuandrantShift(float base) {
        /*
         * Quadrant I   = +,+
         * Quadrant II  = -,+
         * Quadrant III = -,-
         * Quadrant IV  = +,-
         */
        Q1.x = Q1.y = Q2.y = Q4.x = base;
        Q2.x = Q3.x = Q3.y = Q4.y = -base;
    }
};

/* Displays Grid lines at 0.1 intervals */
void drawGridLines() {
    // starting Gridlines
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    // horizontal lines
    // horizontal x=-1, y=1 | x=x*-1 y=y
    for (float x = -1.0f;x < 1.1f; x += 0.1f) {
        for (float y = 1.0f; y > -1.1f; y -= 0.1f) {
            glVertex2f(x, y);
            glVertex2f(x * (-1.0f), y);
        }
    }

    // vertical   x=-1, y=1 | x=x    y=y*-1
    for (float y = 1.0f; y > -1.1f; y -= 0.1f) {
        for (float x = -1.0f;x < 1.1f; x += 0.1f) {
            glVertex2f(x, y);
            glVertex2f(x, y* (-1.0f));
        }
    }
    glEnd();

    // X,Y 0 Axis
    glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2f(-1.0, 0.0);
        glVertex2f(1.0, 0.0);
        glVertex2f(0.0, 1.0);
        glVertex2f(0.0, -1.0);
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
    // Move modelo the center of the world with a z depth of -7
    glTranslatef(0.0f, 0.0f, 0.0f);

    // Rotate model on x, y, and z
    glRotatef(0.0f,0.0f,0.0f,0.0f);

    // define pi
    //float pi = 3.14159265359;
    //float pi = M_PI;

    // a pentagram is equally spaced so 360/5 = 72 degrees
    // convert 72 degrees to radians.
    float theta = (360.0f/5.0f) * (M_PI/180.0f);
    // each smaller triangle will have a 36 degree angle
    // from the center point to the inner point
    float theta_c = 36 * (M_PI/180.0f);
    // each tip will be bisected into 2 triangles therefore
    // instead of 36 degree angle it becomes 18 degrees
    float theta_t = 18 * (M_PI/180.0f);
    float theta_inner = 126 * (M_PI/180.0f); // 180 - 18 - 36 = 126

    /* build the vector points in clockwise manner QuadrantI, QuadrantIV, QuadrantIII, QuadrantI
     * these are the 5 outer points of the pentagram.
     */
    // x and y shifting to center images to different quadrants
    // QuandrantShift.Q#.x/y provides x, y values to shift
    // the images.
    auto q = QuandrantShift(0.5f);

    float radius = 0.3f;
    float angleIncrement = (360.0f / 5.0f) * (M_PI / 180.0f);  // n is 5 in your case

    GLfloat pentagon[5][2];
    GLfloat pentagram[5][2];
    GLfloat pentagram_inner_points[5][2];
    theta = 0.0f;
    // pentagon and pentagram building using trig functions
    for (int i = 0; i < 5; i++) {
        pentagon[i][0] = radius * sinf(theta) + q.Q1.x;
        pentagon[i][1] = radius * cosf(theta) + q.Q1.y;

        float b = radius * sinf(theta_t) / sinf(theta_inner);
        pentagram[i][0] = radius * sinf(theta) + q.Q4.x;
        pentagram[i][1] = radius * cosf(theta) + q.Q4.y;
        pentagram_inner_points[i][0] = b * sinf(theta_c) + q.Q4.x;
        pentagram_inner_points[i][1] = b * cosf(theta_c) + q.Q4.y;

        theta += angleIncrement;
        theta_c += angleIncrement;
    }

    // heptagon building using trig functions
    GLfloat heptagon[7][2];
    theta = 0.0f;
    angleIncrement = (360.0f / 7.0f) * (M_PI / 180.0f);
    for (int i = 0; i < 7; i++){
        heptagon[i][0] = radius * sinf(theta) + q.Q2.x;
        heptagon[i][1] = radius * cosf(theta) + q.Q2.y;
        theta += angleIncrement;
    }

    // Draw a Pentagram in quadrant IV
    GLfloat center[2] = { q.Q4.x, q.Q4.y };
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);

        for (int i = 0; i < 5; i++) {
            glVertex2fv(center);
            glVertex2fv(pentagram[i]);
            glVertex2fv(pentagram_inner_points[i]);

            if (i == 4) {
                glVertex2fv(center);
                glVertex2fv(pentagram[0]);
                glVertex2fv(pentagram_inner_points[i]);
            }else {
                glVertex2fv(center);
                glVertex2fv(pentagram[i+1]);
                glVertex2fv(pentagram_inner_points[i]);
            }
        }
    glEnd();

    // Draw a Pentagon in quadrant I
    glBegin(GL_POLYGON);
        glColor3f(0.0f, 1.0f, 0.0f);
        for (auto & p : pentagon) {
            glVertex2fv(p);
        }
    glEnd();

    // Draw a Heptagon in quadrant II
    glBegin(GL_POLYGON);
        glColor3f(0.0f, 0.0f, 1.0f);
        for (auto & h : heptagon) {
            glVertex2fv(h);
        }
    glEnd();

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
    glutCreateWindow("Tutorial");

    // Specify the widows dimensions (width, height)
    glutInitWindowSize(1280, 720);
    // Specify the position of the window's top left corner
    glutInitWindowPosition(0,0);

    // Set the display callback for the current window
    glutDisplayFunc(display);
    // Sets the display callback for the window re-size event
    //glutReshapeFunc(reshape);

    // Apply depth settings
    //applyDepthSettings();

    // Enters the GLUT event processing loop
    glutMainLoop();

    // exits the main function
    return 0;
}