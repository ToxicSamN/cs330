
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
    glutCreateWindow("My First OpenGL Window");

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