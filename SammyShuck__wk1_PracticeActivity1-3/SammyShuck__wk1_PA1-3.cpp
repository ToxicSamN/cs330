/*
 * Author: Sammy Shuck
 * Name: Practice Activity 1
 * Week: One
 * Assignment: 1-3 Practice Activity 1: Creating a Context in OpenGL
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

    // Empty all buffers and execute all issues commands to be accepted by the rendering engine
    glFlush();
}

/* Main function required for immediate mode */
int main(int argc, char** argv){

    // Initialize the freeGLUT library
    glutInit(&argc, argv);
    // Create a window and title
    glutCreateWindow("Sammy Shuck");

    // Specify the widows dimensions (width, height)
    glutInitWindowSize(640, 480);
    // Specify the position of the window's top left corner
    glutInitWindowPosition(0,0);

    // Set the display callback for the current window
    glutDisplayFunc(displayGraphics);
    // Enters the GLUT event processing loop
    glutMainLoop();

    return 0;
}