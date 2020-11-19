/*
 * Author: Sammy Shuck
 * Name: Triangles
 * Week: Two
 * Assignment: Module Two Tutorial
 * Course: CS-330-J2956 Comp Graphic and Visualization 20EW2
 * Date: 11/1/2020
 * Version: 1.0
 * Copyright: Copyright © 2017 SNHU COCE
 */

#include <GL/freeglut.h> // include the freeGLUT header file

/* Implements Display Callback Handler */
void displayGraphics() {
    /* Specify the RBG and alpha values used when the color buffers are cleared. Clolor is set to Black with full
       Opacity */
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Uses the color buffer and sets the background color of the window
    glClear(GL_COLOR_BUFFER_BIT);

    // starting delimiter for triangle primitive
    glBegin(GL_TRIANGLES);
		// set the color to red using floating point values
        glColor3f(1.0f, 0.0f, 0.0f);
        // Set the red vertex position on x y plane using floating point values
        glVertex2f(-1.0f, 0.0f);

        // set the color to green using floating point values
        glColor3f(0.0f, 1.0f, 0.0f);
        // Set the red vertex position on x y plane using floating point values
        glVertex2f(0.0f, 1.0f);

        // set the color to blue using floating point values
        glColor3f(0.0f, 0.0f, 1.0f);
        // Set the red vertex position on x y plane using floating point values
        glVertex2f(1.0f,0.0f);

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
    glutCreateWindow("OpenGL Triangle");

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