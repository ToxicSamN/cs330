/*
 * Author: Sammy Shuck
 * Name: CS-330 Final Project
 * Week: Seven
 * Assignment: 7-1 Final Project Submission
 * Course: CS-330-J2956 Comp Graphic and Visualization 20EW2
 * Date: 12/12/2020
 * Version: 1.0
 * Copyright: Copyright © 2017 SNHU COCE
 */

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>

// GLM Math Header inclusions
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

#define WINDOW_TITLE "Modern OpenGL"

/*Shader program macro*/
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version "\n" #Source
#endif

/*function prototypes*/
void UResizeWindow(int, int);
void URenderGraphics(void);
void UCreateShader(void);
void UCreateBuffers(void);
void UInitialize(int, char*[]);
void UInitWindow(int, char*[]);
void UCreateVBO(void);
void DestroyVBO(void);
void UCreateShaders(void);
void DestroyShaders(void);
void Cleanup(void);
void KeyboardFunction(unsigned char, int, int);
void UKeyRelease(unsigned char key, int x, int y);
void MouseClick(int btn, int state, int x, int y);
void MouseMove(int x, int y);
void MousePressMove(int x, int y);
void IdleFunction(void);

/* Global variable declarations */
GLuint
        WindowWidth = 800,
        WindowHeight = 600,
        VertexShaderId, // vertex shader
FragmentShaderId, // fragment shader
ProgramId, // shader program
VaoId,
        VboId,
        IndexBufferId,
        modKey,
        texture;

GLfloat
        cameraSpeed = 0.0005f, // camera movement speed per frame
zoomSpeed = 0.5f, // camera zoom speed per frame
lastMouseX = 400,
        lastMouseY = 300,
        mouseOffsetX,
        mouseOffsetY,
        yaw = 0.0f,
        pitch = 0.0f,
        sensitivity = 0.5f,
        scaleX = 1.0f,
        scaleY = 1.0f,
        scaleZ = 1.0f;

glm::vec3
        front,
        cameraForwardZ,
        cameraPos = glm::vec3(0.0f,0.0f,0.0f),
        cameraUpY = glm::vec3(0.0f,1.0f,0.0f);

GLchar
        currKey; // store current key pressed

bool
        mouseDetected = true, // flag for determinig when the mouse is detected in the window
zoom, // flag to tell the program to zoom
orbit; // flag to tell the program to orbit


/*Vertex Shader Program Source Code*/
const GLchar* vertexShaderSource = GLSL(330,
                                        layout(location=0) in vec3 vertex_Position; // Receive vertex coordinates from attribute 0. i.e. 2 floats per vertex

                                                /*Get the vertex colors from the Vertex Buffer object*/
                                                layout(location=1) in vec3 colorFromVBO; // for attribute 1 expect vec(4) floats passed into the vertex shader

                                                out vec3 colorFromVShader; // declare a vec 4 variable that will referencethe vertex colors passed into the Vertex shader from the buffer

                                                // Global variable for the transform matrices
                                                uniform mat4 model;
                                                uniform mat4 view;
                                                uniform mat4 projection;

                                                void main(){
                                                    gl_Position = projection * view * model * vec4(vertex_Position, 1.0f); // Send the vertex positions to gl_Position vec 4
                                                    colorFromVShader = colorFromVBO; // references vertex colors sent from the buffer
                                                }
                                   );

/*  Fragment Shader Program Source Code*/
const GLchar* fragmentShaderSource = GLSL(330,
                                          in vec3 colorFromVShader; // Vertex colors from the vertex shader

                                                  out vec4 gpuColor; // vec 4 variable that will reference the vertex colors passed into the fragment shader from the vertex shader

                                                  void main(){
                                                      gpuColor = vec4(colorFromVShader, 1.0f); // Send the vertex colors to the GPU
                                                  }
                                     );

const int
    numObjects = 9,
    numTrianglesPerObject = 8;
// table definition for each front and back of the object.
// since this is a cube like object (6 sides) then the same
// pattern can be applied to each piece of the table
// v0, v1, v3 front triangle 1
// v2, v3, v0 front triangle 2
// v4, v5, v7 back triangle 1
// v6, v7, v4 back triangle 2
// v4, v5, v1 left triangle 1
// v0, v1, v4 left triangle 2
// v2, v3, v7 right triangle 1
// v6, v7, v2 right triangle 2
// v4, v0, v2 top triangle 1
// v6, v2, v4 top triangle 2
// v5, v1, v3 bottom triangle 1
// v7, v3, v5 bottom triangle 2
GLfloat tableDefinitions[numObjects][numTrianglesPerObject][6] = {
        // table top
        {
                {-0.500f,-0.250f, 0.000f,   1.0f, 1.0f, 1.0f},
                {-0.500f,-0.250f,-0.013f,   1.0f, 1.0f, 1.0f},
                { 0.500f,-0.250f, 0.000f,   1.0f, 1.0f, 1.0f},
                { 0.500f,-0.250f,-0.013f,   1.0f, 1.0f, 1.0f},
                {-0.500f, 0.250f, 0.000f,   1.0f, 1.0f, 1.0f},
                {-0.500f,-0.250f,-0.013f,   1.0f, 1.0f, 1.0f},
                { 0.500f, 0.250f, 0.000f,   1.0f, 1.0f, 1.0f},
                { 0.500f, 0.250f,-0.013f,   1.0f, 1.0f, 1.0f},
        },
        // table front border
        {
                {-0.474f,-0.224f,-0.013f,   1.0f, 1.0f, 1.0f},
                {-0.474f,-0.224f,-0.052f,   1.0f, 1.0f, 1.0f},
                { 0.474f,-0.224f,-0.013f,   1.0f, 1.0f, 1.0f},
                { 0.474f,-0.224f,-0.052f,   1.0f, 1.0f, 1.0f},
                {-0.474f,-0.211f,-0.013f,   1.0f, 1.0f, 1.0f},
                {-0.474f,-0.211f,-0.052f,   1.0f, 1.0f, 1.0f},
                { 0.474f,-0.211f,-0.013f,   1.0f, 1.0f, 1.0f},
                { 0.474f,-0.211f,-0.052f,   1.0f, 1.0f, 1.0f},

        },
        // table back border
        {
                {-0.474f,0.211f,-0.013f,   1.0f, 1.0f, 1.0f},
                {-0.474f,0.211f,-0.052f,   1.0f, 1.0f, 1.0f},
                { 0.474f,0.211f,-0.013f,   1.0f, 1.0f, 1.0f},
                { 0.474f,0.211f,-0.052f,   1.0f, 1.0f, 1.0f},
                {-0.474f,0.224f,-0.013f,   1.0f, 1.0f, 1.0f},
                {-0.474f,0.224f,-0.052f,   1.0f, 1.0f, 1.0f},
                { 0.474f,0.224f,-0.013f,   1.0f, 1.0f, 1.0f},
                { 0.474f,0.224f,-0.052f,   1.0f, 1.0f, 1.0f},
        },
        // table left border
        {
                {-0.474f,-0.211f,-0.013f,   1.0f, 1.0f, 1.0f},
                {-0.474f,-0.211f,-0.052f,   1.0f, 1.0f, 1.0f},
                {-0.461f,-0.211f,-0.013f,   1.0f, 1.0f, 1.0f},
                {-0.461f,-0.211f,-0.052f,   1.0f, 1.0f, 1.0f},
                {-0.474f, 0.211f,-0.013f,   1.0f, 1.0f, 1.0f},
                {-0.474f, 0.211f,-0.052f,   1.0f, 1.0f, 1.0f},
                {-0.461f, 0.211f,-0.013f,   1.0f, 1.0f, 1.0f},
                {-0.461f, 0.211f,-0.052f,   1.0f, 1.0f, 1.0f},
        },
        // table right border
        {
                {0.461f,-0.211f,-0.013f,   1.0f, 1.0f, 1.0f},
                {0.461f,-0.211f,-0.052f,   1.0f, 1.0f, 1.0f},
                {0.474f,-0.211f,-0.013f,   1.0f, 1.0f, 1.0f},
                {0.474f,-0.211f,-0.052f,   1.0f, 1.0f, 1.0f},
                {0.461f, 0.211f,-0.013f,   1.0f, 1.0f, 1.0f},
                {0.461f, 0.211f,-0.052f,   1.0f, 1.0f, 1.0f},
                {0.474f, 0.211f,-0.013f,   1.0f, 1.0f, 1.0f},
                {0.474f, 0.211f,-0.052f,   1.0f, 1.0f, 1.0f},

        },
        // table front left leg
        {
                {-0.4285f,-0.2110f,-0.0130f,   1.0f, 1.0f, 1.0f},
                {-0.4285f,-0.2110f,-0.5000f,   1.0f, 1.0f, 1.0f},
                {-0.4610f,-0.2110f,-0.0130f,   1.0f, 1.0f, 1.0f},
                {-0.4610f,-0.2110f,-0.5000f,   1.0f, 1.0f, 1.0f},
                {-0.4285f,-0.1785f,-0.0130f,   1.0f, 1.0f, 1.0f},
                {-0.4285f,-0.1785f,-0.5000f,   1.0f, 1.0f, 1.0f},
                {-0.4610f,-0.1785f,-0.0130f,   1.0f, 1.0f, 1.0f},
                {-0.4610f,-0.1785f,-0.5000f,   1.0f, 1.0f, 1.0f},
        },
        // table front right leg
        {
                {0.4285f,-0.2110f,-0.0130f,   1.0f, 1.0f, 1.0f},
                {0.4285f,-0.2110f,-0.5000f,   1.0f, 1.0f, 1.0f},
                {0.4610f,-0.2110f,-0.0130f,   1.0f, 1.0f, 1.0f},
                {0.4610f,-0.2110f,-0.5000f,   1.0f, 1.0f, 1.0f},
                {0.4285f,-0.1785f,-0.0130f,   1.0f, 1.0f, 1.0f},
                {0.4285f,-0.1785f,-0.5000f,   1.0f, 1.0f, 1.0f},
                {0.4610f,-0.1785f,-0.0130f,   1.0f, 1.0f, 1.0f},
                {0.4610f,-0.1785f,-0.5000f,   1.0f, 1.0f, 1.0f},

        },
        // table back left leg
        {
                {-0.4610f,0.1785f,-0.0130f,   1.0f, 1.0f, 1.0f},
                {-0.4610f,0.1785f,-0.5000f,   1.0f, 1.0f, 1.0f},
                {-0.4285f,0.1785f,-0.0130f,   1.0f, 1.0f, 1.0f},
                {-0.4285f,0.1785f,-0.5000f,   1.0f, 1.0f, 1.0f},
                {-0.4610f,0.2110f,-0.0130f,   1.0f, 1.0f, 1.0f},
                {-0.4610f,0.2110f,-0.5000f,   1.0f, 1.0f, 1.0f},
                {-0.4285f,0.2110f,-0.0130f,   1.0f, 1.0f, 1.0f},
                {-0.4285f,0.2110f,-0.5000f,   1.0f, 1.0f, 1.0f},
        },
        // table back right leg
        {
                {0.4285f,0.1785f,-0.0130f,   1.0f, 1.0f, 1.0f},
                {0.4285f,0.1785f,-0.5000f,   1.0f, 1.0f, 1.0f},
                {0.4610f,0.1785f,-0.0130f,   1.0f, 1.0f, 1.0f},
                {0.4610f,0.1785f,-0.5000f,   1.0f, 1.0f, 1.0f},
                {0.4285f,0.2110f,-0.0130f,   1.0f, 1.0f, 1.0f},
                {0.4285f,0.2110f,-0.5000f,   1.0f, 1.0f, 1.0f},
                {0.4610f,0.2110f,-0.0130f,   1.0f, 1.0f, 1.0f},
                {0.4610f,0.2110f,-0.5000f,   1.0f, 1.0f, 1.0f},
        },
};


/*
 * VertexMeta class is a way of defining the coordinate plane and
 * providing the necessary strides and offsets for glVertexAttribPointer
 * values. Based on the dimensions provides (2 or 3) will determine the
 * stride and offset values
 */
class VertexMeta {
private:
    GLuint numDimensions;

    // vertex num floats is r, g, b, a = 4
    const size_t VertexColorOffset = 3;

public:
    VertexMeta(); // default constructor
    explicit VertexMeta(GLuint d); // constructor setting number of dimensions
    void setDimension(GLuint d);
    GLuint getDimension();
    size_t getColorStride();
    size_t getColorOffset();
    size_t getVertexStride();
    GLvoid* getRGBAOffset();
};
/**
 * Default Constructor
 */
VertexMeta::VertexMeta() {
    this->numDimensions = 3; // default to 3 dimensions
}
/**
 * Constructor with dimension
 */
VertexMeta::VertexMeta(GLuint d) {
    this->numDimensions = d;
}
/**
 * Setter for setting the number of dimensions to calculate
 * @param d int : defines the number of dimension
 */
void VertexMeta::setDimension(GLuint d){
    this->numDimensions = d;
}
/**
 * Getter for returning the number of dimensions to calculate
 * @return int: the number of dimension
 */
GLuint VertexMeta::getDimension(){
    return this->numDimensions;
}
/**
 * Returns the size_t value for color stride based on the dimension set i.e. 2D vs 3D
 * 2D color stride is {r, g, b, a, x, y }
 * 3D color stride is {r, g, b, a, x, y, z }
 * @return size_t value of dimensional color stride
 */
size_t VertexMeta::getColorStride() {
    return sizeof(float) * (this->numDimensions + this->VertexColorOffset);
}
/**
 * Returns the size_t value for color offset, this is always 4 = RGBA
 * @return size_t value of number of floats ina color definitions RGBA
 */
size_t VertexMeta::getColorOffset() {
    return this->VertexColorOffset;
}
/**
 * Returns the size_t value for Vertex stride based on the dimension set i.e. 2D vs 3D
 * 2D vertex stride is {x, y, r, g, b, a }
 * 3D vertex stride is {x, y, z, r, g, b, a }
 * @return size_t value of dimensional vertex stride
 */
size_t VertexMeta::getVertexStride() {
    return sizeof(float) * (this->numDimensions + this->VertexColorOffset);
}
/**
 * Returns the RGBA offset based on the dimension set i.e. 2D vs 3D
 * num floats until the beginning of each color in 2D vertices = 2 {x, y}
 * num floats until the beginning of each color in 3D vertices = 3 {x, y, z}
 * @return const char* value of dimensional RGB offset
 */
GLvoid* VertexMeta::getRGBAOffset() {
    return (GLvoid*)(sizeof(GLfloat) * this->numDimensions);
}


/* Main Program */
int main(int argc, char* argv[]) {
    UInitialize(argc, argv); // Initialize the OpenGL program
    glutMainLoop(); // Starts the OpenGL loop in the background

    exit(EXIT_SUCCESS); // Terminates the program successfully
}

// properly cleanup at the end
void Cleanup(){
    DestroyShaders();
    DestroyVBO();
}

// Implements the UInitialize function
void UInitialize(int argc, char* argv[]){
    // glew status variable
    GLenum GlewInitResult;

    UInitWindow(argc, argv); // Creates the window

    glewExperimental = GL_TRUE;

    // checks glew status
    GlewInitResult = glewInit();
    if (GlewInitResult != GLEW_OK){
        fprintf(stderr, "ERROR: %s\n", glewGetErrorString(GlewInitResult));
        exit(EXIT_FAILURE);
    }

    // Displays GPU OpenGL version
    fprintf(stderr, "INFO: OpenGL Version: %s\n", glGetString(GL_VERSION));

    UCreateShader(); // Calls the function to create the shader program
    UCreateVBO(); // Calls the function to create the vertex buffer object

    // Sets the background color of the window to black, Optional
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

// Implements the UInitWindow function
void UInitWindow(int argc, char* argv[]){
    // Initialize freeglut
    glutInit(&argc, argv);

    // Memory buffer setup for display
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    // Sets the window size
    glutInitWindowSize(WindowWidth, WindowHeight);

    // Creates a window with the macro placeholder title
    glutCreateWindow(WINDOW_TITLE);

    glutReshapeFunc(UResizeWindow); // Called when the window is resized
    glutDisplayFunc(URenderGraphics); // Renders graphics on the screen
    glutKeyboardFunc(KeyboardFunction); // changes the graphics based upon a keystroke
    glutKeyboardUpFunc(UKeyRelease);
    glutMouseFunc(MouseClick);
    glutPassiveMotionFunc(MouseMove);
    glutMotionFunc(MousePressMove);
    glutIdleFunc(IdleFunction);
    glutCloseFunc(Cleanup); // properly cleans up system resources
}

/* Resizes the widow */
void UResizeWindow(int w, int h){
    WindowWidth = w;
    WindowHeight = h;
    glViewport(0,0, WindowWidth, WindowHeight);
}

/* Renders Graphics */
void URenderGraphics(void){

    glEnable(GL_DEPTH_TEST); // Enable z-depth

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(VaoId);

    cameraForwardZ = front;

    // Declares a 4x4 identity matrix uniform to the handle transformations
    glm::mat4  model;
    model = glm::translate(model, glm::vec3(0.0f,0.0f,0.0f));
    //Rotates Shape 45 degrees on the z axis
    model = glm::rotate(model, 45.0f, glm::vec3(0.0f,1.0f,0.0f));
    // Scales the shape down by half it's original size in xyz
    model = glm::scale(model, glm::vec3(scaleX, scaleY, scaleZ));

    // Transforms the camera
    glm::mat4 view;
    view = glm::lookAt(cameraForwardZ, cameraPos, cameraUpY);

    // Creates a perspective projection
    glm::mat4 projection;
    projection = glm::perspective(45.0f, (GLfloat)WindowWidth / (GLfloat)WindowHeight, 0.1f, 100.0f);

    // Retrieves and passes transform matrices to the shader program
    GLint modelLoc = glGetUniformLocation(ProgramId, "model");
    GLint viewLoc = glGetUniformLocation(ProgramId, "view");
    GLint projLoc = glGetUniformLocation(ProgramId, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glutPostRedisplay();

    /* Creates the triangle */
    // Draw the triangle using the indices
    glDrawArrays(GL_TRIANGLES, 0, 1000);

    glBindVertexArray(0); // deactivates the vertex array object

    glutSwapBuffers(); // flips the back buffer with the front buffer every frame
}

/* Creates the shader program */
void UCreateShader(void){

    // Vertex Shader
    VertexShaderId = glCreateShader(GL_VERTEX_SHADER); // Create a Vertex Shader Object
    glShaderSource(VertexShaderId, 1, &vertexShaderSource, NULL); // retrieves the vertex shader source code
    glCompileShader(VertexShaderId); // Compile the vertex shader

    // Fragrment Shader
    FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER); // Create a vertex shader object
    glShaderSource(FragmentShaderId, 1, &fragmentShaderSource, NULL); // retrieves the fragment shader source code
    glCompileShader(FragmentShaderId); // Compile the fragment shader

    // Create a shader program object
    ProgramId = glCreateProgram();

    // Attaches the vertex and fragment shader to the shader program
    glAttachShader(ProgramId, VertexShaderId);
    glAttachShader(ProgramId, FragmentShaderId);

    glLinkProgram(ProgramId); // Links the shader program
    glUseProgram(ProgramId); // Uses the shader program

    // delete the vertex and fragment shaders once linked
    glDeleteShader(VertexShaderId);
    glDeleteShader(FragmentShaderId);
}
// destroy the shaders properly
void DestroyShaders(void){
    GLenum ErrorCheckValue = glGetError();

    glUseProgram(0);

    glDetachShader(ProgramId, VertexShaderId);
    glDetachShader(ProgramId, FragmentShaderId);

    glDeleteProgram(ProgramId);

    ErrorCheckValue = glGetError();
    if (ErrorCheckValue != GL_NO_ERROR)
    {
        fprintf(
                stderr,
                "ERROR: Could not destroy the shaders: %s \n",
                gluErrorString(ErrorCheckValue)
        );

        exit(-1);
    }
}

/* Creates the buffer and array objects */
void UCreateVBO(){
    // define the number of dimension 2D (x, y) coordinates or 3D (x, y, z) coordinates
    VertexMeta VertexInfo = VertexMeta(3);

    GLfloat a[] = {
            // table top
            -0.500f,-0.250f, 0.000f,   1.0f, 1.0f, 1.0f, // v0
            -0.500f,-0.250f,-0.013f,   1.0f, 1.0f, 1.0f, // v1
            0.500f,-0.250f,-0.013f,   1.0f, 1.0f, 1.0f, // v3
            0.500f,-0.250f, 0.000f,   1.0f, 1.0f, 1.0f, // v2
            0.500f,-0.250f,-0.013f,   1.0f, 1.0f, 1.0f, // v3
            -0.500f,-0.250f, 0.000f,   1.0f, 1.0f, 1.0f, // v0

            -0.500f, 0.250f, 0.000f,   1.0f, 1.0f, 1.0f, // v4
            -0.500f,-0.250f,-0.013f,   1.0f, 1.0f, 1.0f, // v5
            0.500f, 0.250f,-0.013f,   1.0f, 1.0f, 1.0f, // v7
            0.500f, 0.250f, 0.000f,   1.0f, 1.0f, 1.0f, // v6
            0.500f, 0.250f,-0.013f,   1.0f, 1.0f, 1.0f, // v7
            -0.500f, 0.250f, 0.000f,   1.0f, 1.0f, 1.0f, // v4

            -0.500f, 0.250f, 0.000f,   1.0f, 1.0f, 1.0f, // v4
            -0.500f,-0.250f,-0.013f,   1.0f, 1.0f, 1.0f, // v5
            -0.500f,-0.250f,-0.013f,   1.0f, 1.0f, 1.0f, // v1
            -0.500f,-0.250f, 0.000f,   1.0f, 1.0f, 1.0f, // v0
            -0.500f,-0.250f,-0.013f,   1.0f, 1.0f, 1.0f, // v1
            -0.500f, 0.250f, 0.000f,   1.0f, 1.0f, 1.0f, // v4

            0.500f,-0.250f, 0.000f,   1.0f, 1.0f, 1.0f, // v2
            0.500f,-0.250f,-0.013f,   1.0f, 1.0f, 1.0f, // v3
            0.500f, 0.250f,-0.013f,   1.0f, 1.0f, 1.0f, // v7
            0.500f, 0.250f, 0.000f,   1.0f, 1.0f, 1.0f, // v6
            0.500f, 0.250f,-0.013f,   1.0f, 1.0f, 1.0f, // v7
            0.500f,-0.250f, 0.000f,   1.0f, 1.0f, 1.0f, // v2

            -0.500f, 0.250f, 0.000f,   1.0f, 1.0f, 1.0f, // v4
            -0.500f,-0.250f, 0.000f,   1.0f, 1.0f, 1.0f, // v0
            0.500f,-0.250f, 0.000f,   1.0f, 1.0f, 1.0f, // v2
            0.500f, 0.250f, 0.000f,   1.0f, 1.0f, 1.0f, // v6
            0.500f,-0.250f, 0.000f,   1.0f, 1.0f, 1.0f, // v2
            -0.500f, 0.250f, 0.000f,   1.0f, 1.0f, 1.0f, // v4

            -0.500f,-0.250f,-0.013f,   1.0f, 1.0f, 1.0f, // v5
            -0.500f,-0.250f,-0.013f,   1.0f, 1.0f, 1.0f, // v1
            0.500f,-0.250f,-0.013f,   1.0f, 1.0f, 1.0f, // v3
            0.500f, 0.250f,-0.013f,   1.0f, 1.0f, 1.0f, // v7
            0.500f,-0.250f,-0.013f,   1.0f, 1.0f, 1.0f, // v3
            -0.500f,-0.250f,-0.013f,   1.0f, 1.0f, 1.0f, // v5
            
            // table front border
            -0.474f,-0.224f,-0.013f
                            -0.474f,-0.224f,-0.052f

    };

    glGenVertexArrays(1, &VaoId);
    glBindVertexArray(VaoId);

    //GLuint VboId; // variable for vertex buffer object id
    glGenBuffers(1, &VboId); // Creates 2 buffers
    glBindBuffer(GL_ARRAY_BUFFER, VboId); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(a), a, GL_STATIC_DRAW); // sends vertex or coordinate data to the GPU

    /* Creates the vertex Attribute Pointer */
    glEnableVertexAttribArray(0); // Specifies the initial position of the coordinates in the buffer

    /*Sets an attribute pointer position for the vertex colors i.e. Attribute 1 for rgba floats. Attribute 0 was for position x, y */
    glEnableVertexAttribArray(1); // Specifies position 1 for the color values in the buffer

    /*
     * Instructs the GPU on how to handle the vertex buffer object data.
     * Parameters: attribPointerPosition | coordinates per vertex | data type | deactivate normalization | 0 strides | o offset
     */
    glVertexAttribPointer(0, VertexInfo.getDimension(), GL_FLOAT, GL_FALSE, VertexInfo.getVertexStride(), (GLvoid*)0);
    glVertexAttribPointer(1, VertexInfo.getColorOffset(), GL_FLOAT, GL_FALSE, VertexInfo.getColorStride(), VertexInfo.getRGBAOffset());

    glBindVertexArray(0);

}
// Destory VBO properly
void DestroyVBO(void){
    GLenum ErrorCheckValue = glGetError();

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &VboId);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &IndexBufferId);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VaoId);

    ErrorCheckValue = glGetError();
    if (ErrorCheckValue != GL_NO_ERROR)
    {
        fprintf(
                stderr,
                "ERROR: Could not destroy the VBO: %s \n",
                gluErrorString(ErrorCheckValue)
        );

        exit(-1);
    }
}

/*
 * When a key is pressed in the window then process this keystroke
 * and perform a behavior.
 */
void KeyboardFunction(unsigned char key, int X, int Y)
{
    switch (key)
    {
        case 'w':
            std::cout << "pressed w" << std::endl;
            cameraPos += cameraSpeed * cameraForwardZ;
            break;
        case 's':
            std::cout << "pressed s" << std::endl;
            cameraPos -= cameraSpeed * cameraForwardZ;
            break;
        case 'a':
            std::cout << "pressed a" << std::endl;
            cameraPos -= glm::normalize(glm::cross(cameraForwardZ, cameraUpY)) * cameraSpeed;
            break;
        case 'd':
            std::cout << "pressed d" << std::endl;
            cameraPos += glm::normalize(glm::cross(cameraForwardZ, cameraUpY)) * cameraSpeed;
            break;
        default:
            std::cout << "Press a key!" << std::endl;
    }
}

void UKeyRelease(unsigned char key, int x, int y){
    std::cout << currKey << " released" << std::endl;
    currKey = '0';
}

bool IsAltKeyPressed(){
    // get the modifier key
    modKey = glutGetModifiers();
    // if the modifer key pressed is alt return true
    return modKey == GLUT_ACTIVE_ALT;
}

// track mouse button clicks
void MouseClick(int btn, int state, int x, int y){
    // Alt+Left Mouse is an orbital command
    if ((btn == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN) && IsAltKeyPressed()){
        std::cout << "Alt + Left mouse button pressed" << std::endl;
        zoom = false;
        orbit = true;
    }

    // Alt+Right Mouse is a zoom command
    if ((btn == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN) && IsAltKeyPressed()){
        std::cout << "Alt + Right mouse button pressed" << std::endl;
        orbit = false;
        zoom = true;
    }
    // reset orbit/zoom variables when mouse press is released
    if (state == GLUT_UP) {
        orbit = false;
        zoom = false;
    }
}

void SetView(){
    // Sets the front variable that controls CameraForwardZ
    front.x = 10.0f * cos(glm::radians(yaw));
    front.y = 10.0f * sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw)) * 10.f;
}

// tracks the mouse movement withing the window
void MouseMove(int x, int y){
    if (mouseDetected) {
        lastMouseX = x;
        lastMouseY = y;
        mouseDetected = false;
    }

    SetView();
}

// tracks when mouse button is pressed and moved
void MousePressMove(int x, int y){
    if (zoom){
        // if current mouse position y vector is less than the previous vector
        // zoom out, otherwise current mouse y vector is greater so zoom in
        if (y < lastMouseY) {
            // zoom out
            // if while zooming out the image will reverse and start zooming in again, so let's
            // control this to not surpass zoomSpeed value
            if (scaleX <= zoomSpeed) {
                scaleX = scaleY = scaleZ = zoomSpeed;
            }else {
                scaleX = scaleY = scaleZ -= zoomSpeed;
            }
            std::cout << "scale (X, Y, Z)" << scaleX << " " << scaleY << " " << scaleZ << std::endl;
        }else {
            // zoom in
            scaleX += zoomSpeed;
            scaleY += zoomSpeed;
            scaleZ += zoomSpeed;
            std::cout << "scale (X, Y, Z)" << scaleX << " " << scaleY << " " << scaleZ << std::endl;
        }

        // Updates with the new mouse coordinates
        lastMouseX = x;
        lastMouseY = y;
    }

    if (orbit){
        // Gets the direction the mouse was move in x and y
        mouseOffsetX = x - lastMouseX;
        mouseOffsetY = y - lastMouseY;

        // Updates with the new mouse coordinates
        lastMouseX = x;
        lastMouseY = y;

        // Applies mouse sensitivity
        mouseOffsetX *= sensitivity;
        mouseOffsetY *= sensitivity;

        // Accumulates the yaw and pith variables
        yaw += mouseOffsetX;
        pitch += mouseOffsetY;

    }

    SetView();
}

/*
 * Mark the normal plane of current window as needing to be redisplayed.
 * The next iteration through glutMainLoop, the window's display callback will be called to
 * redisplay the window's normal plane
 */
void IdleFunction(void)
{
    glutPostRedisplay();
}
