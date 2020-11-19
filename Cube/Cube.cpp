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

#include <GL/glut.h> // include the freeGLUT header file

/* setup the program */
void drawCube() {
    // starting delimiter for triangle primitive
    glBegin(GL_QUADS);
    // FRONT //
    // set the color to red using floating point values
    glColor3f(1.0f, 0.0f, 0.0f);
    // Cube front : the red vertex position on x y z plane using floating point values
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // RIGHT //
    // set the color to Green using floating point values
    glColor3f(0.0f, 1.0f, 0.0f);
    // Cube right : the green vertex position on x y z plane using floating point values
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // TOP //
    // set the color to blue using floating point values
    glColor3f(0.0f, 0.0f, 1.0f);
    // Cube top : the blue vertex position on x y z plane using floating point values
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    // LEFT //
    // set the color to yellow using floating point values
    glColor3f(1.0f, 1.0f, 0.0f);
    // Cube left : the yellow vertex position on x y z plane using floating point values
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // BACK //
    // set the color to dull green using floating point values
    glColor3f(0.0f, 0.5f, 0.4f);
    // Cube back : the dull green vertex position on x y z plane using floating point values
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    // BOTTOM //
    // set the color to grey using floating point values
    glColor3f(0.50f, 0.50f, 0.50f);
    // Cube bottom : the grey vertex position on x y z plane using floating point values
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

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
    glRotatef(45.0f,0.4f,-1.0f,-0.1f);
    drawCube();
    // Rotate model on x, y, and z
    glRotatef(150.0f,0.0f,1.0f,0.0f);
    drawCube();
    // Rotate model on x, y, and z
    glRotatef(150.0f,0.0f,1.0f,-0.4f);
    drawCube();
    // Rotate model on x, y, and z
    glRotatef(155.0f,0.0f,1.0f,-0.4f);
    drawCube();

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
    glutCreateWindow("3D Cube");

    // Specify the widows dimensions (width, height)
    glutInitWindowSize(640, 480);
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