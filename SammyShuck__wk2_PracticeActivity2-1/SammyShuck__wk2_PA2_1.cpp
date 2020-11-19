/*
 * Author: Sammy Shuck
 * Name: Practice Activity 2-1
 * Week: Two
 * Assignment: Module Two Practice Activity 2-1
 * Course: CS-330-J2956 Comp Graphic and Visualization 20EW2
 * Date: 11/8/2020
 * Version: 1.0
 * Copyright: Copyright © 2017 SNHU COCE
 * C++ Version: 20
 */

#include <GL/gl.h> // include the freeGLUT header file
#include <GL/glut.h> // include the freeGLUT header file
#include <math.h>

const float radius = 0.3f;

/*
 * QuadrantShift class is a simple datastructure
 * for defining 2D x, y shifting.
 * Shifts are based on the follwoing definitions
 * Quadrant I   : (+,+)
 * Quadrant II  : (-,+)
 * Quadrant III : (-,-)
 * Quadrant IV  : (+,-)
 */
class QuadrantShift {

private:
    struct Quadrant {
        float x;
        float y;

        Quadrant(){
            x = 0.0f;
            y = 0.0f;
        }
    };

public:
    QuadrantShift();
    QuadrantShift(float base);
    void rebase(float base);
    Quadrant Q1;
    Quadrant Q2;
    Quadrant Q3;
    Quadrant Q4;
};
/**
 * Default Constructor
 */
QuadrantShift::QuadrantShift() {
    Q1.x = Q1.y = Q2.x = Q2.y = Q3.x = Q3.y = Q4.x = Q4.y = 0.0f;
}
/**
 * Constructor with base
 */
QuadrantShift::QuadrantShift(float base) {
    Q1.x = Q1.y = Q2.y = Q4.x = base;
    Q2.x = Q3.x = Q3.y = Q4.y = -base;
}
void QuadrantShift::rebase(float base) {
    Q1.x = Q1.y = Q2.y = Q4.x = base;
    Q2.x = Q3.x = Q3.y = Q4.y = -base;
}

/*
 * drawPentagon calculates all of the points of the
 * pentagon and draws the polygon shape in Quadrant I
 */
void drawPentagon(){
    GLfloat pentagon[5][2];

    // number of points
    float n = 5.0f;
    // initial theta angle is 0
    float theta = 0.0f;
    // increment theta by 360/n degrees
    float thetaIncrement = (360.0f / n) * (M_PI / 180.0f);

    // x and y shifting to center images to different quadrants
    // QuandrantShift.Q#.x/y provides x, y values to shift
    // the images.
    QuadrantShift q = QuadrantShift(0.5f);

    // pentagon and pentagram building using trig functions
    // calculate the (x,y) vectors for each pentagon point
    // while centering the shape in quadrant I
    for (int i = 0; i < n; i++) {
        pentagon[i][0] = radius * sinf(theta) + q.Q1.x;
        pentagon[i][1] = radius * cosf(theta) + q.Q1.y;

        theta += thetaIncrement;
    }

    // Draw a Pentagon in quadrant I
    glBegin(GL_POLYGON);
    glColor3f(0.0f, 1.0f, 0.0f);
    for (int i = 0; i < n; i++) {
        glVertex2fv(pentagon[i]);
    }
    glEnd();
}

/*
 * drawHeptagon calculates all of the points of the
 * heptagon and draws the polygon shape in Quadrant II
 */
void drawHeptagon(){
    // heptagon building using trig functions
    GLfloat heptagon[7][2];

    // x and y shifting to center images to different quadrants
    // QuandrantShift.Q#.x/y provides x, y values to shift
    // the images.
    QuadrantShift q = QuadrantShift(0.5f);

    // number of points
    float n = 7.0f;
    // initial theta angle is 0
    float theta = 0.0f;
    // increment theta by 360/n degrees
    float thetaIncrement = (360.0f / n) * (M_PI / 180.0f);

    // calculate the (x,y) vectors for each heptagon point
    // while centering the shape in quadrant II
    for (int i = 0; i < n; i++){
        heptagon[i][0] = radius * sinf(theta) + q.Q2.x;
        heptagon[i][1] = radius * cosf(theta) + q.Q2.y;

        theta += thetaIncrement;
    }

    // Draw a Heptagon in quadrant II
    glBegin(GL_POLYGON);
    glColor3f(0.0f, 0.0f, 1.0f);
    for (int i = 0; i < n; i++) {
        glVertex2fv(heptagon[i]);
    }
    glEnd();
}

/*
 * drawPentagram calculates all of the points of the
 * pentagram as well as the inner pentagon and draws
 * the polygon shape in Quadrant IV
 */
void drawPentagram(){
    GLfloat pentagram[5][2];
    GLfloat pentagram_inner_points[5][2];

    // number of points
    float n = 5.0f;
    // initial theta angle is 0
    float theta = 0.0f;
    // increment theta by 360/n degrees
    float thetaIncrement = (360.0f / n) * (M_PI / 180.0f);

    // each smaller triangle will have a 36 degree angle
    // from the center point to the inner point
    float theta_c = 36 * (M_PI/180.0f);
    // each tip will be bisected into 2 triangles therefore
    // instead of 36 degree angle it becomes 18 degrees
    float theta_t = 18 * (M_PI/180.0f);
    float theta_inner = 126 * (M_PI/180.0f); // 180 - 18 - 36 = 126

    // x and y shifting to center images to different quadrants
    // QuandrantShift.Q#.x/y provides x, y values to shift
    // the images.
    QuadrantShift q = QuadrantShift(0.5f);

    // pentagon and pentagram building using trig functions
    // calculate the (x,y) vectors for each pentagram point
    // while centering the shape in quadrant IV
    for (int i = 0; i < 5; i++) {
        float b = radius * sinf(theta_t) / sinf(theta_inner);
        pentagram[i][0] = radius * sinf(theta) + q.Q4.x;
        pentagram[i][1] = radius * cosf(theta) + q.Q4.y;

        // calculate the inner points which is the pentagon
        // inside the pentagram
        pentagram_inner_points[i][0] = b * sinf(theta_c) + q.Q4.x;
        pentagram_inner_points[i][1] = b * cosf(theta_c) + q.Q4.y;

        theta += thetaIncrement;
        theta_c += thetaIncrement;
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
    glTranslatef(0.0f, 0.0f, 0.0f);

    drawPentagon();
    drawHeptagon();
    drawPentagram();

    glutSwapBuffers();
}

/* Main function required for immediate mode */
int main(int argc, char** argv){

    // Initialize the freeGLUT library
    glutInit(&argc, argv);
    // Enable double buffered mode
    glutInitDisplayMode(GLUT_DOUBLE);
    // Create a window and title
    glutCreateWindow("Sammy Shuck - PA2-1");

    // Specify the widows dimensions (width, height)
    glutInitWindowSize(1280, 720);
    // Specify the position of the window's top left corner
    glutInitWindowPosition(0,0);

    // Set the display callback for the current window
    glutDisplayFunc(display);

    // Enters the GLUT event processing loop
    glutMainLoop();

    // exits the main function
    return 0;
}
