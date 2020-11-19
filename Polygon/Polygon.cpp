/*
 * Author: Sammy Shuck
 * Name: Polygon
 * Week: Two
 * Assignment: Module Two Tutorial
 * Course: CS-330-J2956 Comp Graphic and Visualization 20EW2
 * Date: 11/1/2020
 * Version: 1.0
 * Copyright: Copyright © 2017 SNHU COCE
 */

#include <GL/freeglut.h> // include the freeGLUT header file

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
void displayGraphics() {
    /* Specify the RBG and alpha values used when the color buffers are cleared. Clolor is set to Black with full
       Opacity */
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Uses the color buffer and sets the background color of the window
    glClear(GL_COLOR_BUFFER_BIT);

    drawGridLines();

    // starting delimiter for triangle primitive
    glBegin(GL_POLYGON);
    // set the color to red using floating point values
        glColor3f(0.0f, 1.0f, 0.50f);

        // Sets 5 vertices to form the Polygon
        glVertex2f(-0.50f, 0.50f);
        glVertex2f(0.0f, 1.0f);
        glVertex2f(0.50f, 0.50f);
        glVertex2f(0.50f,-0.50f);
        glVertex2f(-0.50f,-0.50f);

    // ending dilimiter for triangle primitive
    glEnd();

    // Empty all buffers and execute all issues commands to be accepted by the rendering engine
    glFlush();
}

/* Main function required for immediate mode */
int main(int argc, char** argv){

    // Initialize the freeGLUT library
    glutInit(&argc, argv);
    // Create a window and title
    glutCreateWindow("OpenGL Polygon");

    // Specify the widows dimensions (width, height)
    glutInitWindowSize(1280, 720);
    // Specify the position of the window's top left corner
    glutInitWindowPosition(0,0);

    // Set the display callback for the current window
    glutDisplayFunc(displayGraphics);
    // Enters the GLUT event processing loop
    glutMainLoop();

    // exits the main function
    return 0;
}