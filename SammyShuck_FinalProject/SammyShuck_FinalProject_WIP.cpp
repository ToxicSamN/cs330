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

// SOIL image loader inclusion
#include "SOIL2/SOIL2.h"

using namespace std;

#define WINDOW_TITLE "Modern OpenGL"

/*function prototypes*/
void UResizeWindow(int, int);
void URenderGraphics(void);
void UGenerateTexture();
void UInitialize(int, char*[]);
void UInitWindow(int, char*[]);
void UCreateVBO(void);
void DestroyVBO(void);
void Cleanup(void);
void KeyboardFunction(unsigned char, int, int);
void UKeyRelease(unsigned char key, int x, int y);
void MouseClick(int btn, int state, int x, int y);
void MouseMove(int x, int y);
void MousePressMove(int x, int y);
void UMouseScroll(int wheel, int direction, int x, int y);
void IdleFunction(void);

/* Global variable declarations */
GLuint
        WindowWidth = 800,
        WindowHeight = 600,
        ProgramId, // shader program
VAO,
        VBO,
        modKey,
        texture;

GLfloat
        cameraSpeed = 0.0505f, // camera movement speed per frame
zoomSpeed = 0.5f, // camera zoom speed per frame
lastMouseX = 400,
        lastMouseY = 300,
        mouseOffsetX,
        mouseOffsetY,
        yaw = 0.0f,
        pitch = 0.0f,
        sensitivity = 0.5f,
        scaleX = 6.0f,
        scaleY = 6.0f,
        scaleZ = 6.0f;

glm::vec3
        front,
        cameraForwardZ,
        cameraPos = glm::vec3(0.5f,0.0f,0.0f),
        cameraUpY = glm::vec3(0.0f,1.0f,0.0f);


// key and fill light colors
glm::vec3 keyLightColor(1.0f, 1.0f, 1.0f); // green light
glm::vec3 fillLightColor(0.999, 0.999, 0.999); // white light

// Light position
glm::vec3 keyLightPos(-5.0f, 15.0f, -10.0f);
glm::vec3 fillLightPos(5.0f, 0.0f, 10.0f);


glm::mat4 projection = glm::perspective(45.0f, (GLfloat)WindowWidth / (GLfloat)WindowHeight, 0.1f, 100.0f);

GLchar
        currKey; // store current key pressed

bool
        mouseDetected = true, // flag for determinig when the mouse is detected in the window
zoom, // flag to tell the program to zoom
orbit; // flag to tell the program to orbit



/*
 * Shader object contains the shader definitions and allows for a more condensed use
 * of the compiling of the vertex and fragment shaders
 */
class Shader
{
private:
    GLuint vertexShaderId, fragmentShaderId;

    /* Shader program macro */
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version "\n" #Source
#endif

    /* Light Vertex Shader source code */
    const GLchar *vertexShaderSource = GLSL(330,
                                            layout(location = 0) in vec3 position; // VAP position 0 for vertex position data
                                                    layout(location = 1) in vec3 normal; // VAP position 1 for normals
                                                    layout(location = 2) in vec2 textureCoordinate;

                                                    out vec3 Normal; // for outgoing normals to fragment shader
                                                    out vec3 FragmentPos; // for outgoing color / pixels to fragment shader
                                                    out vec2 mobileTextureCoordinate; // coordinates for texture

                                                    // uniform / global variables for the transform matrices
                                                    uniform mat4 model;
                                                    uniform mat4 view;
                                                    uniform mat4 projection;

                                                    void main() {
                                                        gl_Position = projection * view * model * vec4(position, 1.0f);// Transforms vertices into clip coordinates
                                                        Normal = mat3(transpose(inverse(model))) * normal; // get normal vectors in world space only and exclude normal translation properties
                                                        FragmentPos = vec3(model * vec4(position, 1.0f)); // Gets fragment / pixel position in world space only (exclude view and projection)
                                                        mobileTextureCoordinate = vec2(textureCoordinate.x, 1.0f - textureCoordinate.y); // flips the texture horizontally
                                                    }
                                       );

    /* Light Fragment Shader source code */
    const GLchar *fragmentShaderSource = GLSL(330,
                                              in vec3 Normal; //For incoming normals
                                                      in vec3 FragmentPos; //for incoming fragment position
                                                      in vec2 mobileTextureCoordinate;

                                                      out vec4 gpuOut; //for outgoing light color to the GPU

                                                      //Uniform / Global variables for object color, light color, light position and camera/view position
                                                      uniform vec3 keyLightColor;
                                                      uniform vec3 fillLightColor;
                                                      uniform vec3 keyLightPos;
                                                      uniform vec3 fillLightPos;
                                                      uniform vec3 viewPosition;
                                                      uniform sampler2D uTexture; //Useful when working with multiple textures

                                                      void main() {

                                                          /* Calculate Ambient Lighting */
                                                          float ambientStrength = 0.1f;
                                                          vec3 keyAmbient = ambientStrength * keyLightColor; //Generate ambient light color
                                                          vec3 fillAmbient = ambientStrength * fillLightColor;//Generate second ambient light color

                                                          /* Calculate Diffuse Lighting */
                                                          vec3 norm = normalize(Normal); //Normalize vectors to 1 unit
                                                          vec3 keyLightDirection = normalize(keyLightPos - FragmentPos); //Calculate distance (light direction) between light source and fragments/pixels on
                                                          vec3 fillLightDirection = normalize(fillLightPos - FragmentPos); //Calculate distance (light direction) between light source and fragments/pixels on
                                                          float keyImpact = max(dot(norm, keyLightDirection), 0.0); //Calculate diffuse impact by generating dot product of normal and light
                                                          float fillImpact = max(dot(norm, fillLightDirection), 0.0); //Calculate diffuse impact by generating dot product of normal and light
                                                          vec3 keyDiffuse = keyImpact * keyLightColor; //Generate diffuse light color
                                                          vec3 fillDiffuse = fillImpact * fillLightColor; //Generate diffuse light color

                                                          /* Calculate Specular Lighting */
                                                          float highlighSize = 0.5f;
                                                          float keySpecularIntensity = 0.5f;
                                                          float fillSpecularIntensity = 1.0f;

                                                          vec3 viewDir = normalize(viewPosition - FragmentPos); //Calculate view direction
                                                          vec3 keyReflectDir = reflect(-keyLightDirection, norm); //Calculate reflection vector
                                                          vec3 fillReflectDir = reflect(-fillLightDirection, norm); //Calculate reflection vector
                                                          // Calculate specular component
                                                          float keySpecularComponent = pow(max(dot(viewDir, keyReflectDir), 0.0), highlighSize);
                                                          float fillSpecularComponent = pow(max(dot(viewDir, fillReflectDir), 0.0), highlighSize);
                                                          vec3 keySpecular = keySpecularIntensity * keySpecularComponent * keyLightColor;
                                                          vec3 fillSpecular = fillSpecularIntensity * keySpecularComponent * keyLightColor;

                                                          /* Calculate phong result */
                                                          vec3 keyPhong = (keyAmbient + keyDiffuse + keySpecular) * vec3(texture(uTexture, mobileTextureCoordinate));
                                                          vec3 fillPhong = (fillAmbient + fillDiffuse + fillSpecular) * vec3(texture(uTexture, mobileTextureCoordinate));

                                                          gpuOut = vec4(keyPhong + fillPhong, 1.0f); //Send lighting results to GPU
                                                      }
                                         );
    /* Vertex Shader source code */
    const GLchar * textureVertexShaderSource = GLSL(330,
                                                    layout (location = 0) in vec3 position; // vertex data from Vertex Attrib Pointer 0
                                                            layout (location = 1) in vec2 textureUV;

                                                            out vec2 mobileTextureCoordinate;

                                                            // Global variables for the transform matrices
                                                            uniform mat4 model;
                                                            uniform mat4 view;
                                                            uniform mat4 projection;

                                                            void main(){
                                                                gl_Position = projection * view * model * vec4(position, 1.0f); // transforms vertices to clip coordinates
                                                                mobileTextureCoordinate = vec2(textureUV.x, 1-textureUV.y); // flips the texture horizontally
                                                            }
                                               );

    /* fragment shader source code */
    const GLchar * textureFragmentShaderSource = GLSL(330,
                                                      in vec2 mobileTextureCoordinate;

                                                              out vec4 gpuTexture; // variable to pass color data to the GPU

                                                              uniform sampler2D uTexture; //Useful when working with multiple textures

                                                              void main(){
                                                                  gpuTexture = texture(uTexture, mobileTextureCoordinate);
                                                              }
                                                 );


public:
    GLuint Program;

    /* Constructor for generating the shader */
    Shader(){
        this->Program  = 0;
        this->vertexShaderId = 0;
        this->fragmentShaderId = 0;
    }

    void Compile()
    {
        // Compile the shaders
        // Vertex shader
        this->vertexShaderId = glCreateShader(GL_VERTEX_SHADER); // creates the vertex shader
        glShaderSource(this->vertexShaderId, 1, &this->vertexShaderSource, NULL); // Attaches the vertex shader to the source code
        glCompileShader(this->vertexShaderId); // compiles the vertex shader

        // Fragment shader
        this->fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER); // Creates the fragment shader
        glShaderSource(this->fragmentShaderId, 1, &this->fragmentShaderSource, NULL); // Attaches the fragment shader to the source code
        glCompileShader(this->fragmentShaderId); // compiles the fragment shader

        // Shader Program
        this->Program = glCreateProgram(); // Creates the shader program and returns an id
        glAttachShader(this->Program, this->vertexShaderId); // Attach vertex shader to the shader program
        glAttachShader(this->Program, this->fragmentShaderId); // Attach fragment shader to the shader program
        glLinkProgram(this->Program); // link vertex and fragment shader to shader program

        // delete the vertex and fragment shaders once linked
        glDeleteShader(this->vertexShaderId);
        glDeleteShader(this->fragmentShaderId);
    }
    void CompileTextureOnly()
    {
        // Compile the shaders
        // Vertex shader
        this->vertexShaderId = glCreateShader(GL_VERTEX_SHADER); // creates the vertex shader
        glShaderSource(this->vertexShaderId, 1, &this->textureVertexShaderSource, NULL); // Attaches the vertex shader to the source code
        glCompileShader(this->vertexShaderId); // compiles the vertex shader

        // Fragment shader
        this->fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER); // Creates the fragment shader
        glShaderSource(this->fragmentShaderId, 1, &this->textureFragmentShaderSource, NULL); // Attaches the fragment shader to the source code
        glCompileShader(this->fragmentShaderId); // compiles the fragment shader

        // Shader Program
        this->Program = glCreateProgram(); // Creates the shader program and returns an id
        glAttachShader(this->Program, this->vertexShaderId); // Attach vertex shader to the shader program
        glAttachShader(this->Program, this->fragmentShaderId); // Attach fragment shader to the shader program
        glLinkProgram(this->Program); // link vertex and fragment shader to shader program

        // delete the vertex and fragment shaders once linked
        glDeleteShader(this->vertexShaderId);
        glDeleteShader(this->fragmentShaderId);
    }

    // Use the current shader program
    void Use() {

        glUseProgram(this->Program);

    }
    void Cleanup(){
        GLenum ErrorCheckValue = glGetError();

        glUseProgram(0);

        glDetachShader(this->Program, this->vertexShaderId);
        glDetachShader(this->Program, this->fragmentShaderId);

        glDeleteProgram(this->Program);

        ErrorCheckValue = glGetError();
        if (ErrorCheckValue != GL_NO_ERROR)
        {
            fprintf(
                    stderr,
                    "ERROR: Could not destroy the shaders: %s \n",
                    gluErrorString(ErrorCheckValue)
            );
        }
    }

};

Shader shader = Shader();

/* Main Program */
int main(int argc, char* argv[]) {
    UInitialize(argc, argv); // Initialize the OpenGL program
    glutMainLoop(); // Starts the OpenGL loop in the background

    exit(EXIT_SUCCESS); // Terminates the program successfully
}

// properly cleanup at the end
void Cleanup(){
    shader.Cleanup();
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


    UCreateVBO(); // Calls the function to create the vertex buffer object
    UGenerateTexture();

    shader.Compile();
    shader.Use();

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
    glutMouseWheelFunc(UMouseScroll);
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
    glDepthFunc(GL_LESS);
    // Enable alpha support to make the code more dynamic
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glBindVertexArray(VAO);

    cameraForwardZ = front;

    // Declares a 4x4 identity matrix uniform to the handle transformations
    glm::mat4  model;
    model = glm::translate(model, glm::vec3(0.0f,0.0f,0.0f));
    //Rotates Shape 45 degrees on the z axis
    model = glm::rotate(model, 90.0f, glm::vec3(0.5f,0.0f,0.0f));
    // Scales the shape down by half it's original size in xyz
    model = glm::scale(model, glm::vec3(scaleX, scaleY, scaleZ));

    // Transforms the camera
    glm::mat4 view;
    view = glm::lookAt(cameraForwardZ, cameraPos, cameraUpY);
    view = glm::rotate(view, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // Retrieves and passes transform matrices to the shader program
    GLint modelLoc = glGetUniformLocation(shader.Program, "model");
    GLint viewLoc = glGetUniformLocation(shader.Program, "view");
    GLint projLoc = glGetUniformLocation(shader.Program, "projection");
    GLint txLoc = glGetUniformLocation(shader.Program, "uTexture");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform1i(txLoc, 0);

    /* Pass color, light position, and camera position to the shader program
    *
    * uniform vec3 keyLightColor
    * uniform vec3 fillLightColor
    * uniform vec3 keyLightPos
    * uniform vec3 fillLightPos
    * uniform vec3 viewPosition
    */
    GLint keyLightColorLoc = glGetUniformLocation(shader.Program, "keyLightColor");
    GLint fillLightColorLoc = glGetUniformLocation(shader.Program, "fillLightColor");
    GLint keyLightPosLoc = glGetUniformLocation(shader.Program, "keyLightPos");
    GLint fillLightPosLoc = glGetUniformLocation(shader.Program, "fillLightPos");
    GLint viewPositionLoc = glGetUniformLocation(shader.Program, "viewPosition");

    // pass color, light, and camera data to the cube shader programs corresponding uniforms
    glUniform3f(keyLightColorLoc, keyLightColor.r, keyLightColor.g, keyLightColor.b);
    glUniform3f(fillLightColorLoc, fillLightColor.r, fillLightColor.g, fillLightColor.b);
    glUniform3f(keyLightPosLoc, keyLightPos.x, keyLightPos.y, keyLightPos.z);
    glUniform3f(fillLightPosLoc, fillLightPos.x, fillLightPos.y, fillLightPos.z);
    glUniform3f(viewPositionLoc, cameraPos.x, cameraPos.y, cameraPos.z);

    glutPostRedisplay();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    /* Creates the triangle */
    // Draw the triangle using the indices
    glDrawArrays(GL_TRIANGLES, 0, 500);

    glBindVertexArray(0); // deactivates the vertex array object

    glutSwapBuffers(); // flips the back buffer with the front buffer every frame
}


/* Creates the buffer and array objects */
void UCreateVBO(){

    GLfloat vertices[] = {

            //		x			y			z			normX	normY	normZ		u		v

            // table top
            // front
            -0.5000f,	-0.2500f,	0.0000f,		0.0f,	0.0f,	1.0f,		0.0f,	1.0f,		// v0
            -0.5000f,	-0.2500f,	0.0130f,		0.0f,	0.0f,	1.0f,		0.0f,	0.0f,		// v1
            0.5000f,	-0.2500f,	0.0130f,		0.0f,	0.0f,	1.0f,		1.0f,	0.0f,		// v3
            0.5000f,	-0.2500f,	0.0000f,		0.0f,	0.0f,	1.0f,		1.0f,	1.0f,		// v2
            0.5000f,	-0.2500f,	0.0130f,		0.0f,	0.0f,	1.0f,		1.0f,	0.0f,		// v3
            -0.5000f,	-0.2500f,	0.0000f,		0.0f,	0.0f,	1.0f,		0.0f,	1.0f,		// v0

            // back					0.0f,	0.0f,	-1.0f,
            -0.5000f,	0.2500f,	0.0000f,		0.0f,	0.0f,	-1.0f,		0.0f,	1.0f,		// v4
            -0.5000f,	0.2500f,	0.0130f,		0.0f,	0.0f,	-1.0f,		0.0f,	0.0f,		// v5
            0.5000f,	0.2500f,	0.0130f,		0.0f,	0.0f,	-1.0f,		1.0f,	0.0f,		// v7
            0.5000f,	0.2500f,	0.0000f,		0.0f,	0.0f,	-1.0f,		1.0f,	1.0f,		// v6
            0.5000f,	0.2500f,	0.0130f,		0.0f,	0.0f,	-1.0f,		1.0f,	0.0f,		// v7
            -0.5000f,	0.2500f,	0.0000f,		0.0f,	0.0f,	-1.0f,		0.0f,	1.0f,		// v4

            // left
            -0.5000f,	0.2500f,	0.0000f,		-1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v4
            -0.5000f,	0.2500f,	0.0130f,		-1.0f,	0.0f,	0.0f,		0.0f,	0.0f,		// v5
            -0.5000f,	-0.2500f,	0.0130f,		-1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v1
            -0.5000f,	-0.2500f,	0.0000f,		-1.0f,	0.0f,	0.0f,		1.0f,	1.0f,		// v0
            -0.5000f,	-0.2500f,	0.0130f,		-1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v1
            -0.5000f,	0.2500f,	0.0000f,		-1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v4

            // right
            0.5000f,	-0.2500f,	0.0000f,		1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v2
            0.5000f,	-0.2500f,	0.0130f,		1.0f,	0.0f,	0.0f,		0.0f,	0.0f,		// v3
            0.5000f,	0.2500f,	0.0130f,		1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v7
            0.5000f,	0.2500f,	0.0000f,		1.0f,	0.0f,	0.0f,		1.0f,	1.0f,		// v6
            0.5000f,	0.2500f,	0.0130f,		1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v7
            0.5000f,	-0.2500f,	0.0000f,		1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v2

            // top
            -0.5000f,	0.2500f,	0.0000f,		0.0f,	1.0f,	0.0f,		0.0f,	1.0f,		// v4
            -0.5000f,	-0.2500f,	0.0000f,		0.0f,	1.0f,	0.0f,		0.0f,	0.0f,		// v0
            0.5000f,	-0.2500f,	0.0000f,		0.0f,	1.0f,	0.0f,		1.0f,	0.0f,		// v2
            0.5000f,	0.2500f,	0.0000f,		0.0f,	1.0f,	0.0f,		1.0f,	1.0f,		// v6
            0.5000f,	-0.2500f,	0.0000f,		0.0f,	1.0f,	0.0f,		1.0f,	0.0f,		// v2
            -0.5000f,	0.2500f,	0.0000f,		0.0f,	1.0f,	0.0f,		0.0f,	1.0f,		// v4

            // bottom
            -0.5000f,	0.2500f,	0.0130f,		0.0f,	-1.0f,	0.0f,		0.0f,	1.0f,		// v5
            -0.5000f,	-0.2500f,	0.0130f,		0.0f,	-1.0f,	0.0f,		0.0f,	0.0f,		// v1
            0.5000f,	-0.2500f,	0.0130f,		0.0f,	-1.0f,	0.0f,		1.0f,	0.0f,		// v3
            0.5000f,	0.2500f,	0.0130f,		0.0f,	-1.0f,	0.0f,		1.0f,	1.0f,		// v7
            0.5000f,	-0.2500f,	0.0130f,		0.0f,	-1.0f,	0.0f,		1.0f,	0.0f,		// v3
            -0.5000f,	0.2500f,	0.0130f,		0.0f,	-1.0f,	0.0f,		0.0f,	1.0f,		// v5

            // front border
            // front
            -0.4740f,	-0.2240f,	0.0130f,		0.0f,	0.0f,	1.0f,		0.0f,	1.0f,		// v0
            -0.4740f,	-0.2240f,	0.0520f,		0.0f,	0.0f,	1.0f,		0.0f,	0.0f,		// v1
            0.4740f,	-0.2240f,	0.0520f,		0.0f,	0.0f,	1.0f,		1.0f,	0.0f,		// v3
            0.4740f,	-0.2240f,	0.0130f,		0.0f,	0.0f,	1.0f,		1.0f,	1.0f,		// v2
            0.4740f,	-0.2240f,	0.0520f,		0.0f,	0.0f,	1.0f,		1.0f,	0.0f,		// v3
            -0.4740f,	-0.2240f,	0.0130f,		0.0f,	0.0f,	1.0f,		0.0f,	1.0f,		// v0

            // back					0.0f,	0.0f,	-1.0f,
            -0.4740f,	-0.2110f,	0.0130f,		0.0f,	0.0f,	-1.0f,		0.0f,	1.0f,		// v4
            -0.4740f,	-0.2110f,	0.0520f,		0.0f,	0.0f,	-1.0f,		0.0f,	0.0f,		// v5
            0.4740f,	-0.2110f,	0.0520f,		0.0f,	0.0f,	-1.0f,		1.0f,	0.0f,		// v7
            0.4740f,	-0.2110f,	0.0130f,		0.0f,	0.0f,	-1.0f,		1.0f,	1.0f,		// v6
            0.4740f,	-0.2110f,	0.0520f,		0.0f,	0.0f,	-1.0f,		1.0f,	0.0f,		// v7
            -0.4740f,	-0.2110f,	0.0130f,		0.0f,	0.0f,	-1.0f,		0.0f,	1.0f,		// v4

            // left
            -0.4740f,	-0.2110f,	0.0130f,		-1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v4
            -0.4740f,	-0.2110f,	0.0520f,		-1.0f,	0.0f,	0.0f,		0.0f,	0.0f,		// v5
            -0.4740f,	-0.2240f,	0.0520f,		-1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v1
            -0.4740f,	-0.2240f,	0.0130f,		-1.0f,	0.0f,	0.0f,		1.0f,	1.0f,		// v0
            -0.4740f,	-0.2240f,	0.0520f,		-1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v1
            -0.4740f,	-0.2110f,	0.0130f,		-1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v4

            // right
            0.4740f,	-0.2240f,	0.0130f,		1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v2
            0.4740f,	-0.2240f,	0.0520f,		1.0f,	0.0f,	0.0f,		0.0f,	0.0f,		// v3
            0.4740f,	-0.2110f,	0.0520f,		1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v7
            0.4740f,	-0.2110f,	0.0130f,		1.0f,	0.0f,	0.0f,		1.0f,	1.0f,		// v6
            0.4740f,	-0.2110f,	0.0520f,		1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v7
            0.4740f,	-0.2240f,	0.0130f,		1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v2

            // top
            -0.4740f,	-0.2110f,	0.0130f,		0.0f,	1.0f,	0.0f,		0.0f,	1.0f,		// v4
            -0.4740f,	-0.2240f,	0.0130f,		0.0f,	1.0f,	0.0f,		0.0f,	0.0f,		// v0
            0.4740f,	-0.2240f,	0.0130f,		0.0f,	1.0f,	0.0f,		1.0f,	0.0f,		// v2
            0.4740f,	-0.2110f,	0.0130f,		0.0f,	1.0f,	0.0f,		1.0f,	1.0f,		// v6
            0.4740f,	-0.2240f,	0.0130f,		0.0f,	1.0f,	0.0f,		1.0f,	0.0f,		// v2
            -0.4740f,	-0.2110f,	0.0130f,		0.0f,	1.0f,	0.0f,		0.0f,	1.0f,		// v4

            // bottom
            -0.4740f,	-0.2110f,	0.0520f,		0.0f,	-1.0f,	0.0f,		0.0f,	1.0f,		// v5
            -0.4740f,	-0.2240f,	0.0520f,		0.0f,	-1.0f,	0.0f,		0.0f,	0.0f,		// v1
            0.4740f,	-0.2240f,	0.0520f,		0.0f,	-1.0f,	0.0f,		1.0f,	0.0f,		// v3
            0.4740f,	-0.2110f,	0.0520f,		0.0f,	-1.0f,	0.0f,		1.0f,	1.0f,		// v7
            0.4740f,	-0.2240f,	0.0520f,		0.0f,	-1.0f,	0.0f,		1.0f,	0.0f,		// v3
            -0.4740f,	-0.2110f,	0.0520f,		0.0f,	-1.0f,	0.0f,		0.0f,	1.0f,		// v5

            // back border
            // front
            -0.4740f,	0.2110f,	0.0130f,		0.0f,	0.0f,	1.0f,		0.0f,	1.0f,		// v0
            -0.4740f,	0.2110f,	0.0520f,		0.0f,	0.0f,	1.0f,		0.0f,	0.0f,		// v1
            0.4740f,	0.2110f,	0.0520f,		0.0f,	0.0f,	1.0f,		1.0f,	0.0f,		// v3
            0.4740f,	0.2110f,	0.0130f,		0.0f,	0.0f,	1.0f,		1.0f,	1.0f,		// v2
            0.4740f,	0.2110f,	0.0520f,		0.0f,	0.0f,	1.0f,		1.0f,	0.0f,		// v3
            -0.4740f,	0.2110f,	0.0130f,		0.0f,	0.0f,	1.0f,		0.0f,	1.0f,		// v0

            // back					0.0f,	0.0f,	-1.0f,
            -0.4740f,	0.2240f,	0.0130f,		0.0f,	0.0f,	-1.0f,		0.0f,	1.0f,		// v4
            -0.4740f,	0.2240f,	0.0520f,		0.0f,	0.0f,	-1.0f,		0.0f,	0.0f,		// v5
            0.4740f,	0.2240f,	0.0520f,		0.0f,	0.0f,	-1.0f,		1.0f,	0.0f,		// v7
            0.4740f,	0.2240f,	0.0130f,		0.0f,	0.0f,	-1.0f,		1.0f,	1.0f,		// v6
            0.4740f,	0.2240f,	0.0520f,		0.0f,	0.0f,	-1.0f,		1.0f,	0.0f,		// v7
            -0.4740f,	0.2240f,	0.0130f,		0.0f,	0.0f,	-1.0f,		0.0f,	1.0f,		// v4

            // left
            -0.4740f,	0.2240f,	0.0130f,		-1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v4
            -0.4740f,	0.2240f,	0.0520f,		-1.0f,	0.0f,	0.0f,		0.0f,	0.0f,		// v5
            -0.4740f,	0.2110f,	0.0520f,		-1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v1
            -0.4740f,	0.2110f,	0.0130f,		-1.0f,	0.0f,	0.0f,		1.0f,	1.0f,		// v0
            -0.4740f,	0.2110f,	0.0520f,		-1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v1
            -0.4740f,	0.2240f,	0.0130f,		-1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v4

            // right
            0.4740f,	0.2110f,	0.0130f,		1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v2
            0.4740f,	0.2110f,	0.0520f,		1.0f,	0.0f,	0.0f,		0.0f,	0.0f,		// v3
            0.4740f,	0.2240f,	0.0520f,		1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v7
            0.4740f,	0.2240f,	0.0130f,		1.0f,	0.0f,	0.0f,		1.0f,	1.0f,		// v6
            0.4740f,	0.2240f,	0.0520f,		1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v7
            0.4740f,	0.2110f,	0.0130f,		1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v2

            // top
            -0.4740f,	0.2240f,	0.0130f,		0.0f,	1.0f,	0.0f,		0.0f,	1.0f,		// v4
            -0.4740f,	0.2110f,	0.0130f,		0.0f,	1.0f,	0.0f,		0.0f,	0.0f,		// v0
            0.4740f,	0.2110f,	0.0130f,		0.0f,	1.0f,	0.0f,		1.0f,	0.0f,		// v2
            0.4740f,	0.2240f,	0.0130f,		0.0f,	1.0f,	0.0f,		1.0f,	1.0f,		// v6
            0.4740f,	0.2110f,	0.0130f,		0.0f,	1.0f,	0.0f,		1.0f,	0.0f,		// v2
            -0.4740f,	0.2240f,	0.0130f,		0.0f,	1.0f,	0.0f,		0.0f,	1.0f,		// v4

            // bottom
            -0.4740f,	0.2240f,	0.0520f,		0.0f,	-1.0f,	0.0f,		0.0f,	1.0f,		// v5
            -0.4740f,	0.2110f,	0.0520f,		0.0f,	-1.0f,	0.0f,		0.0f,	0.0f,		// v1
            0.4740f,	0.2110f,	0.0520f,		0.0f,	-1.0f,	0.0f,		1.0f,	0.0f,		// v3
            0.4740f,	0.2240f,	0.0520f,		0.0f,	-1.0f,	0.0f,		1.0f,	1.0f,		// v7
            0.4740f,	0.2110f,	0.0520f,		0.0f,	-1.0f,	0.0f,		1.0f,	0.0f,		// v3
            -0.4740f,	0.2240f,	0.0520f,		0.0f,	-1.0f,	0.0f,		0.0f,	1.0f,		// v5

            // left border
            // front
            -0.4740f,	-0.2110f,	0.0130f,		0.0f,	0.0f,	1.0f,		0.0f,	1.0f,		// v0
            -0.4740f,	-0.2110f,	0.0520f,		0.0f,	0.0f,	1.0f,		0.0f,	0.0f,		// v1
            -0.4610f,	-0.2110f,	0.0520f,		0.0f,	0.0f,	1.0f,		1.0f,	0.0f,		// v3
            -0.4610f,	-0.2110f,	0.0130f,		0.0f,	0.0f,	1.0f,		1.0f,	1.0f,		// v2
            -0.4610f,	-0.2110f,	0.0520f,		0.0f,	0.0f,	1.0f,		1.0f,	0.0f,		// v3
            -0.4740f,	-0.2110f,	0.0130f,		0.0f,	0.0f,	1.0f,		0.0f,	1.0f,		// v0

            // back					0.0f,	0.0f,	-1.0f,
            -0.4740f,	0.2110f,	0.0130f,		0.0f,	0.0f,	-1.0f,		0.0f,	1.0f,		// v4
            -0.4740f,	0.2110f,	0.0520f,		0.0f,	0.0f,	-1.0f,		0.0f,	0.0f,		// v5
            -0.4610f,	0.2110f,	0.0520f,		0.0f,	0.0f,	-1.0f,		1.0f,	0.0f,		// v7
            -0.4610f,	0.2110f,	0.0130f,		0.0f,	0.0f,	-1.0f,		1.0f,	1.0f,		// v6
            -0.4610f,	0.2110f,	0.0520f,		0.0f,	0.0f,	-1.0f,		1.0f,	0.0f,		// v7
            -0.4740f,	0.2110f,	0.0130f,		0.0f,	0.0f,	-1.0f,		0.0f,	1.0f,		// v4

            // left
            -0.4740f,	0.2110f,	0.0130f,		-1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v4
            -0.4740f,	0.2110f,	0.0520f,		-1.0f,	0.0f,	0.0f,		0.0f,	0.0f,		// v5
            -0.4740f,	-0.2110f,	0.0520f,		-1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v1
            -0.4740f,	-0.2110f,	0.0130f,		-1.0f,	0.0f,	0.0f,		1.0f,	1.0f,		// v0
            -0.4740f,	-0.2110f,	0.0520f,		-1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v1
            -0.4740f,	0.2110f,	0.0130f,		-1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v4

            // right
            -0.4610f,	-0.2110f,	0.0130f,		1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v2
            -0.4610f,	-0.2110f,	0.0520f,		1.0f,	0.0f,	0.0f,		0.0f,	0.0f,		// v3
            -0.4610f,	0.2110f,	0.0520f,		1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v7
            -0.4610f,	0.2110f,	0.0130f,		1.0f,	0.0f,	0.0f,		1.0f,	1.0f,		// v6
            -0.4610f,	0.2110f,	0.0520f,		1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v7
            -0.4610f,	-0.2110f,	0.0130f,		1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v2

            // top
            -0.4740f,	0.2110f,	0.0130f,		0.0f,	1.0f,	0.0f,		0.0f,	1.0f,		// v4
            -0.4740f,	-0.2110f,	0.0130f,		0.0f,	1.0f,	0.0f,		0.0f,	0.0f,		// v0
            -0.4610f,	-0.2110f,	0.0130f,		0.0f,	1.0f,	0.0f,		1.0f,	0.0f,		// v2
            -0.4610f,	0.2110f,	0.0130f,		0.0f,	1.0f,	0.0f,		1.0f,	1.0f,		// v6
            -0.4610f,	-0.2110f,	0.0130f,		0.0f,	1.0f,	0.0f,		1.0f,	0.0f,		// v2
            -0.4740f,	0.2110f,	0.0130f,		0.0f,	1.0f,	0.0f,		0.0f,	1.0f,		// v4

            // bottom
            -0.4740f,	0.2110f,	0.0520f,		0.0f,	-1.0f,	0.0f,		0.0f,	1.0f,		// v5
            -0.4740f,	-0.2110f,	0.0520f,		0.0f,	-1.0f,	0.0f,		0.0f,	0.0f,		// v1
            -0.4610f,	-0.2110f,	0.0520f,		0.0f,	-1.0f,	0.0f,		1.0f,	0.0f,		// v3
            -0.4610f,	0.2110f,	0.0520f,		0.0f,	-1.0f,	0.0f,		1.0f,	1.0f,		// v7
            -0.4610f,	-0.2110f,	0.0520f,		0.0f,	-1.0f,	0.0f,		1.0f,	0.0f,		// v3
            -0.4740f,	0.2110f,	0.0520f,		0.0f,	-1.0f,	0.0f,		0.0f,	1.0f,		// v5

            // right border
            // front
            0.4610f,	-0.2110f,	0.0130f,		0.0f,	0.0f,	1.0f,		0.0f,	1.0f,		// v0
            0.4610f,	-0.2110f,	0.0520f,		0.0f,	0.0f,	1.0f,		0.0f,	0.0f,		// v1
            0.4740f,	-0.2110f,	0.0520f,		0.0f,	0.0f,	1.0f,		1.0f,	0.0f,		// v3
            0.4740f,	-0.2110f,	0.0130f,		0.0f,	0.0f,	1.0f,		1.0f,	1.0f,		// v2
            0.4740f,	-0.2110f,	0.0520f,		0.0f,	0.0f,	1.0f,		1.0f,	0.0f,		// v3
            0.4610f,	-0.2110f,	0.0130f,		0.0f,	0.0f,	1.0f,		0.0f,	1.0f,		// v0

            // back					0.0f,	0.0f,	-1.0f,
            0.4610f,	0.2110f,	0.0130f,		0.0f,	0.0f,	-1.0f,		0.0f,	1.0f,		// v4
            0.4610f,	0.2110f,	0.0520f,		0.0f,	0.0f,	-1.0f,		0.0f,	0.0f,		// v5
            0.4740f,	0.2110f,	0.0520f,		0.0f,	0.0f,	-1.0f,		1.0f,	0.0f,		// v7
            0.4740f,	0.2110f,	0.0130f,		0.0f,	0.0f,	-1.0f,		1.0f,	1.0f,		// v6
            0.4740f,	0.2110f,	0.0520f,		0.0f,	0.0f,	-1.0f,		1.0f,	0.0f,		// v7
            0.4610f,	0.2110f,	0.0130f,		0.0f,	0.0f,	-1.0f,		0.0f,	1.0f,		// v4

            // left
            0.4610f,	0.2110f,	0.0130f,		-1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v4
            0.4610f,	0.2110f,	0.0520f,		-1.0f,	0.0f,	0.0f,		0.0f,	0.0f,		// v5
            0.4610f,	-0.2110f,	0.0520f,		-1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v1
            0.4610f,	-0.2110f,	0.0130f,		-1.0f,	0.0f,	0.0f,		1.0f,	1.0f,		// v0
            0.4610f,	-0.2110f,	0.0520f,		-1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v1
            0.4610f,	0.2110f,	0.0130f,		-1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v4

            // right
            0.4740f,	-0.2110f,	0.0130f,		1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v2
            0.4740f,	-0.2110f,	0.0520f,		1.0f,	0.0f,	0.0f,		0.0f,	0.0f,		// v3
            0.4740f,	0.2110f,	0.0520f,		1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v7
            0.4740f,	0.2110f,	0.0130f,		1.0f,	0.0f,	0.0f,		1.0f,	1.0f,		// v6
            0.4740f,	0.2110f,	0.0520f,		1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v7
            0.4740f,	-0.2110f,	0.0130f,		1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v2

            // top
            0.4610f,	0.2110f,	0.0130f,		0.0f,	1.0f,	0.0f,		0.0f,	1.0f,		// v4
            0.4610f,	-0.2110f,	0.0130f,		0.0f,	1.0f,	0.0f,		0.0f,	0.0f,		// v0
            0.4740f,	-0.2110f,	0.0130f,		0.0f,	1.0f,	0.0f,		1.0f,	0.0f,		// v2
            0.4740f,	0.2110f,	0.0130f,		0.0f,	1.0f,	0.0f,		1.0f,	1.0f,		// v6
            0.4740f,	-0.2110f,	0.0130f,		0.0f,	1.0f,	0.0f,		1.0f,	0.0f,		// v2
            0.4610f,	0.2110f,	0.0130f,		0.0f,	1.0f,	0.0f,		0.0f,	1.0f,		// v4

            // bottom
            0.4610f,	0.2110f,	0.0520f,		0.0f,	-1.0f,	0.0f,		0.0f,	1.0f,		// v5
            0.4610f,	-0.2110f,	0.0520f,		0.0f,	-1.0f,	0.0f,		0.0f,	0.0f,		// v1
            0.4740f,	-0.2110f,	0.0520f,		0.0f,	-1.0f,	0.0f,		1.0f,	0.0f,		// v3
            0.4740f,	0.2110f,	0.0520f,		0.0f,	-1.0f,	0.0f,		1.0f,	1.0f,		// v7
            0.4740f,	-0.2110f,	0.0520f,		0.0f,	-1.0f,	0.0f,		1.0f,	0.0f,		// v3
            0.4610f,	0.2110f,	0.0520f,		0.0f,	-1.0f,	0.0f,		0.0f,	1.0f,		// v5

            // front left leg
            // front
            -0.4285f,	-0.2110f,	0.0130f,		0.0f,	0.0f,	1.0f,		0.0f,	1.0f,		// v0
            -0.4285f,	-0.2110f,	0.5000f,		0.0f,	0.0f,	1.0f,		0.0f,	0.0f,		// v1
            -0.4610f,	-0.2110f,	0.5000f,		0.0f,	0.0f,	1.0f,		1.0f,	0.0f,		// v3
            -0.4610f,	-0.2110f,	0.0130f,		0.0f,	0.0f,	1.0f,		1.0f,	1.0f,		// v2
            -0.4610f,	-0.2110f,	0.5000f,		0.0f,	0.0f,	1.0f,		1.0f,	0.0f,		// v3
            -0.4285f,	-0.2110f,	0.0130f,		0.0f,	0.0f,	1.0f,		0.0f,	1.0f,		// v0

            // back					0.0f,	0.0f,	-1.0f,
            -0.4285f,	-0.1785f,	0.0130f,		0.0f,	0.0f,	-1.0f,		0.0f,	1.0f,		// v4
            -0.4285f,	-0.1785f,	0.5000f,		0.0f,	0.0f,	-1.0f,		0.0f,	0.0f,		// v5
            -0.4610f,	-0.1785f,	0.5000f,		0.0f,	0.0f,	-1.0f,		1.0f,	0.0f,		// v7
            -0.4610f,	-0.1785f,	0.0130f,		0.0f,	0.0f,	-1.0f,		1.0f,	1.0f,		// v6
            -0.4610f,	-0.1785f,	0.5000f,		0.0f,	0.0f,	-1.0f,		1.0f,	0.0f,		// v7
            -0.4285f,	-0.1785f,	0.0130f,		0.0f,	0.0f,	-1.0f,		0.0f,	1.0f,		// v4

            // left
            -0.4285f,	-0.1785f,	0.0130f,		-1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v4
            -0.4285f,	-0.1785f,	0.5000f,		-1.0f,	0.0f,	0.0f,		0.0f,	0.0f,		// v5
            -0.4285f,	-0.2110f,	0.5000f,		-1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v1
            -0.4285f,	-0.2110f,	0.0130f,		-1.0f,	0.0f,	0.0f,		1.0f,	1.0f,		// v0
            -0.4285f,	-0.2110f,	0.5000f,		-1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v1
            -0.4285f,	-0.1785f,	0.0130f,		-1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v4

            // right
            -0.4610f,	-0.2110f,	0.0130f,		1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v2
            -0.4610f,	-0.2110f,	0.5000f,		1.0f,	0.0f,	0.0f,		0.0f,	0.0f,		// v3
            -0.4610f,	-0.1785f,	0.5000f,		1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v7
            -0.4610f,	-0.1785f,	0.0130f,		1.0f,	0.0f,	0.0f,		1.0f,	1.0f,		// v6
            -0.4610f,	-0.1785f,	0.5000f,		1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v7
            -0.4610f,	-0.2110f,	0.0130f,		1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v2

            // top
            -0.4285f,	-0.1785f,	0.0130f,		0.0f,	1.0f,	0.0f,		0.0f,	1.0f,		// v4
            -0.4285f,	-0.2110f,	0.0130f,		0.0f,	1.0f,	0.0f,		0.0f,	0.0f,		// v0
            -0.4610f,	-0.2110f,	0.0130f,		0.0f,	1.0f,	0.0f,		1.0f,	0.0f,		// v2
            -0.4610f,	-0.1785f,	0.0130f,		0.0f,	1.0f,	0.0f,		1.0f,	1.0f,		// v6
            -0.4610f,	-0.2110f,	0.0130f,		0.0f,	1.0f,	0.0f,		1.0f,	0.0f,		// v2
            -0.4285f,	-0.1785f,	0.0130f,		0.0f,	1.0f,	0.0f,		0.0f,	1.0f,		// v4

            // bottom
            -0.4285f,	-0.1785f,	0.5000f,		0.0f,	-1.0f,	0.0f,		0.0f,	1.0f,		// v5
            -0.4285f,	-0.2110f,	0.5000f,		0.0f,	-1.0f,	0.0f,		0.0f,	0.0f,		// v1
            -0.4610f,	-0.2110f,	0.5000f,		0.0f,	-1.0f,	0.0f,		1.0f,	0.0f,		// v3
            -0.4610f,	-0.1785f,	0.5000f,		0.0f,	-1.0f,	0.0f,		1.0f,	1.0f,		// v7
            -0.4610f,	-0.2110f,	0.5000f,		0.0f,	-1.0f,	0.0f,		1.0f,	0.0f,		// v3
            -0.4285f,	-0.1785f,	0.5000f,		0.0f,	-1.0f,	0.0f,		0.0f,	1.0f,		// v5

            // front right leg
            // front
            0.4285f,	-0.2110f,	0.0130f,		0.0f,	0.0f,	1.0f,		0.0f,	1.0f,		// v0
            0.4285f,	-0.2110f,	0.5000f,		0.0f,	0.0f,	1.0f,		0.0f,	0.0f,		// v1
            0.4610f,	-0.2110f,	0.5000f,		0.0f,	0.0f,	1.0f,		1.0f,	0.0f,		// v3
            0.4610f,	-0.2110f,	0.0130f,		0.0f,	0.0f,	1.0f,		1.0f,	1.0f,		// v2
            0.4610f,	-0.2110f,	0.5000f,		0.0f,	0.0f,	1.0f,		1.0f,	0.0f,		// v3
            0.4285f,	-0.2110f,	0.0130f,		0.0f,	0.0f,	1.0f,		0.0f,	1.0f,		// v0

            // back					0.0f,	0.0f,	-1.0f,
            0.4285f,	-0.1785f,	0.0130f,		0.0f,	0.0f,	-1.0f,		0.0f,	1.0f,		// v4
            0.4285f,	-0.1785f,	0.5000f,		0.0f,	0.0f,	-1.0f,		0.0f,	0.0f,		// v5
            0.4610f,	-0.1785f,	0.5000f,		0.0f,	0.0f,	-1.0f,		1.0f,	0.0f,		// v7
            0.4610f,	-0.1785f,	0.0130f,		0.0f,	0.0f,	-1.0f,		1.0f,	1.0f,		// v6
            0.4610f,	-0.1785f,	0.5000f,		0.0f,	0.0f,	-1.0f,		1.0f,	0.0f,		// v7
            0.4285f,	-0.1785f,	0.0130f,		0.0f,	0.0f,	-1.0f,		0.0f,	1.0f,		// v4

            // left
            0.4285f,	-0.1785f,	0.0130f,		-1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v4
            0.4285f,	-0.1785f,	0.5000f,		-1.0f,	0.0f,	0.0f,		0.0f,	0.0f,		// v5
            0.4285f,	-0.2110f,	0.5000f,		-1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v1
            0.4285f,	-0.2110f,	0.0130f,		-1.0f,	0.0f,	0.0f,		1.0f,	1.0f,		// v0
            0.4285f,	-0.2110f,	0.5000f,		-1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v1
            0.4285f,	-0.1785f,	0.0130f,		-1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v4

            // right
            0.4610f,	-0.2110f,	0.0130f,		1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v2
            0.4610f,	-0.2110f,	0.5000f,		1.0f,	0.0f,	0.0f,		0.0f,	0.0f,		// v3
            0.4610f,	-0.1785f,	0.5000f,		1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v7
            0.4610f,	-0.1785f,	0.0130f,		1.0f,	0.0f,	0.0f,		1.0f,	1.0f,		// v6
            0.4610f,	-0.1785f,	0.5000f,		1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v7
            0.4610f,	-0.2110f,	0.0130f,		1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v2

            // top
            0.4285f,	-0.1785f,	0.0130f,		0.0f,	1.0f,	0.0f,		0.0f,	1.0f,		// v4
            0.4285f,	-0.2110f,	0.0130f,		0.0f,	1.0f,	0.0f,		0.0f,	0.0f,		// v0
            0.4610f,	-0.2110f,	0.0130f,		0.0f,	1.0f,	0.0f,		1.0f,	0.0f,		// v2
            0.4610f,	-0.1785f,	0.0130f,		0.0f,	1.0f,	0.0f,		1.0f,	1.0f,		// v6
            0.4610f,	-0.2110f,	0.0130f,		0.0f,	1.0f,	0.0f,		1.0f,	0.0f,		// v2
            0.4285f,	-0.1785f,	0.0130f,		0.0f,	1.0f,	0.0f,		0.0f,	1.0f,		// v4

            // bottom
            0.4285f,	-0.1785f,	0.5000f,		0.0f,	-1.0f,	0.0f,		0.0f,	1.0f,		// v5
            0.4285f,	-0.2110f,	0.5000f,		0.0f,	-1.0f,	0.0f,		0.0f,	0.0f,		// v1
            0.4610f,	-0.2110f,	0.5000f,		0.0f,	-1.0f,	0.0f,		1.0f,	0.0f,		// v3
            0.4610f,	-0.1785f,	0.5000f,		0.0f,	-1.0f,	0.0f,		1.0f,	1.0f,		// v7
            0.4610f,	-0.2110f,	0.5000f,		0.0f,	-1.0f,	0.0f,		1.0f,	0.0f,		// v3
            0.4285f,	-0.1785f,	0.5000f,		0.0f,	-1.0f,	0.0f,		0.0f,	1.0f,		// v5

            // back left leg
            // front
            -0.4610f,	0.1785f,	0.0130f,		0.0f,	0.0f,	1.0f,		0.0f,	1.0f,		// v0
            -0.4610f,	0.1785f,	0.5000f,		0.0f,	0.0f,	1.0f,		0.0f,	0.0f,		// v1
            -0.4285f,	0.1785f,	0.5000f,		0.0f,	0.0f,	1.0f,		1.0f,	0.0f,		// v3
            -0.4285f,	0.1785f,	0.0130f,		0.0f,	0.0f,	1.0f,		1.0f,	1.0f,		// v2
            -0.4285f,	0.1785f,	0.5000f,		0.0f,	0.0f,	1.0f,		1.0f,	0.0f,		// v3
            -0.4610f,	0.1785f,	0.0130f,		0.0f,	0.0f,	1.0f,		0.0f,	1.0f,		// v0

            // back					0.0f,	0.0f,	-1.0f,
            -0.4610f,	0.2110f,	0.0130f,		0.0f,	0.0f,	-1.0f,		0.0f,	1.0f,		// v4
            -0.4610f,	0.2110f,	0.5000f,		0.0f,	0.0f,	-1.0f,		0.0f,	0.0f,		// v5
            -0.4285f,	0.2110f,	0.5000f,		0.0f,	0.0f,	-1.0f,		1.0f,	0.0f,		// v7
            -0.4285f,	0.2110f,	0.0130f,		0.0f,	0.0f,	-1.0f,		1.0f,	1.0f,		// v6
            -0.4285f,	0.2110f,	0.5000f,		0.0f,	0.0f,	-1.0f,		1.0f,	0.0f,		// v7
            -0.4610f,	0.2110f,	0.0130f,		0.0f,	0.0f,	-1.0f,		0.0f,	1.0f,		// v4

            // left
            -0.4610f,	0.2110f,	0.0130f,		-1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v4
            -0.4610f,	0.2110f,	0.5000f,		-1.0f,	0.0f,	0.0f,		0.0f,	0.0f,		// v5
            -0.4610f,	0.1785f,	0.5000f,		-1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v1
            -0.4610f,	0.1785f,	0.0130f,		-1.0f,	0.0f,	0.0f,		1.0f,	1.0f,		// v0
            -0.4610f,	0.1785f,	0.5000f,		-1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v1
            -0.4610f,	0.2110f,	0.0130f,		-1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v4

            // right
            -0.4285f,	0.1785f,	0.0130f,		1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v2
            -0.4285f,	0.1785f,	0.5000f,		1.0f,	0.0f,	0.0f,		0.0f,	0.0f,		// v3
            -0.4285f,	0.2110f,	0.5000f,		1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v7
            -0.4285f,	0.2110f,	0.0130f,		1.0f,	0.0f,	0.0f,		1.0f,	1.0f,		// v6
            -0.4285f,	0.2110f,	0.5000f,		1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v7
            -0.4285f,	0.1785f,	0.0130f,		1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v2

            // top
            -0.4610f,	0.2110f,	0.0130f,		0.0f,	1.0f,	0.0f,		0.0f,	1.0f,		// v4
            -0.4610f,	0.1785f,	0.0130f,		0.0f,	1.0f,	0.0f,		0.0f,	0.0f,		// v0
            -0.4285f,	0.1785f,	0.0130f,		0.0f,	1.0f,	0.0f,		1.0f,	0.0f,		// v2
            -0.4285f,	0.2110f,	0.0130f,		0.0f,	1.0f,	0.0f,		1.0f,	1.0f,		// v6
            -0.4285f,	0.1785f,	0.0130f,		0.0f,	1.0f,	0.0f,		1.0f,	0.0f,		// v2
            -0.4610f,	0.2110f,	0.0130f,		0.0f,	1.0f,	0.0f,		0.0f,	1.0f,		// v4

            // bottom	// bottom
            -0.4610f,	0.2110f,	0.5000f,		0.0f,	-1.0f,	0.0f,		0.0f,	1.0f,		// v5
            -0.4610f,	0.1785f,	0.5000f,		0.0f,	-1.0f,	0.0f,		0.0f,	0.0f,		// v1
            -0.4285f,	0.1785f,	0.5000f,		0.0f,	-1.0f,	0.0f,		1.0f,	0.0f,		// v3
            -0.4285f,	0.2110f,	0.5000f,		0.0f,	-1.0f,	0.0f,		1.0f,	1.0f,		// v7
            -0.4285f,	0.1785f,	0.5000f,		0.0f,	-1.0f,	0.0f,		1.0f,	0.0f,		// v3
            -0.4610f,	0.2110f,	0.5000f,		0.0f,	-1.0f,	0.0f,		0.0f,	1.0f,		// v5

            // back right leg
            // front
            0.4285f,	0.1785f,	0.0130f,		0.0f,	0.0f,	1.0f,		0.0f,	1.0f,		// v0
            0.4285f,	0.1785f,	0.5000f,		0.0f,	0.0f,	1.0f,		0.0f,	0.0f,		// v1
            0.4610f,	0.1785f,	0.5000f,		0.0f,	0.0f,	1.0f,		1.0f,	0.0f,		// v3
            0.4610f,	0.1785f,	0.0130f,		0.0f,	0.0f,	1.0f,		1.0f,	1.0f,		// v2
            0.4610f,	0.1785f,	0.5000f,		0.0f,	0.0f,	1.0f,		1.0f,	0.0f,		// v3
            0.4285f,	0.1785f,	0.0130f,		0.0f,	0.0f,	1.0f,		0.0f,	1.0f,		// v0

            // back					0.0f,	0.0f,	-1.0f,
            0.4285f,	0.2110f,	0.0130f,		0.0f,	0.0f,	-1.0f,		0.0f,	1.0f,		// v4
            0.4285f,	0.2110f,	0.5000f,		0.0f,	0.0f,	-1.0f,		0.0f,	0.0f,		// v5
            0.4610f,	0.2110f,	0.5000f,		0.0f,	0.0f,	-1.0f,		1.0f,	0.0f,		// v7
            0.4610f,	0.2110f,	0.0130f,		0.0f,	0.0f,	-1.0f,		1.0f,	1.0f,		// v6
            0.4610f,	0.2110f,	0.5000f,		0.0f,	0.0f,	-1.0f,		1.0f,	0.0f,		// v7
            0.4285f,	0.2110f,	0.0130f,		0.0f,	0.0f,	-1.0f,		0.0f,	1.0f,		// v4

            // left
            0.4285f,	0.2110f,	0.0130f,		-1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v4
            0.4285f,	0.2110f,	0.5000f,		-1.0f,	0.0f,	0.0f,		0.0f,	0.0f,		// v5
            0.4285f,	0.1785f,	0.5000f,		-1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v1
            0.4285f,	0.1785f,	0.0130f,		-1.0f,	0.0f,	0.0f,		1.0f,	1.0f,		// v0
            0.4285f,	0.1785f,	0.5000f,		-1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v1
            0.4285f,	0.2110f,	0.0130f,		-1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v4

            // right
            0.4610f,	0.1785f,	0.0130f,		1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v2
            0.4610f,	0.1785f,	0.5000f,		1.0f,	0.0f,	0.0f,		0.0f,	0.0f,		// v3
            0.4610f,	0.2110f,	0.5000f,		1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v7
            0.4610f,	0.2110f,	0.0130f,		1.0f,	0.0f,	0.0f,		1.0f,	1.0f,		// v6
            0.4610f,	0.2110f,	0.5000f,		1.0f,	0.0f,	0.0f,		1.0f,	0.0f,		// v7
            0.4610f,	0.1785f,	0.0130f,		1.0f,	0.0f,	0.0f,		0.0f,	1.0f,		// v2

            // top
            0.4285f,	0.2110f,	0.0130f,		0.0f,	1.0f,	0.0f,		0.0f,	1.0f,		// v4
            0.4285f,	0.1785f,	0.0130f,		0.0f,	1.0f,	0.0f,		0.0f,	0.0f,		// v0
            0.4610f,	0.1785f,	0.0130f,		0.0f,	1.0f,	0.0f,		1.0f,	0.0f,		// v2
            0.4610f,	0.2110f,	0.0130f,		0.0f,	1.0f,	0.0f,		1.0f,	1.0f,		// v6
            0.4610f,	0.1785f,	0.0130f,		0.0f,	1.0f,	0.0f,		1.0f,	0.0f,		// v2
            0.4285f,	0.2110f,	0.0130f,		0.0f,	1.0f,	0.0f,		0.0f,	1.0f,		// v4

            // bottom
            0.4285f,	0.2110f,	0.5000f,		0.0f,	-1.0f,	0.0f,		0.0f,	1.0f,		// v5
            0.4285f,	0.1785f,	0.5000f,		0.0f,	-1.0f,	0.0f,		0.0f,	0.0f,		// v1
            0.4610f,	0.1785f,	0.5000f,		0.0f,	-1.0f,	0.0f,		1.0f,	0.0f,		// v3
            0.4610f,	0.2110f,	0.5000f,		0.0f,	-1.0f,	0.0f,		1.0f,	1.0f,		// v7
            0.4610f,	0.1785f,	0.5000f,		0.0f,	-1.0f,	0.0f,		1.0f,	0.0f,		// v3
            0.4285f,	0.2110f,	0.5000f,		0.0f,	-1.0f,	0.0f,		0.0f,	1.0f,		// v5




    };

    // Generate buffer ids
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Activate the VAO before binding and setting VBOs and VAPs
    glBindVertexArray(VAO);

    // Activate the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Copy vertices to VBO

    // set attribute pointer 0 to hold position data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0); // Enables vertex attribute

    // Set attribute pointer 1 to hold Normal data
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Set attribute pointer 2 to hold Texture coordinate data
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) (6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0); // deactivates the vertex array object

}
// Destroy VBO properly
void DestroyVBO(void){
    GLenum ErrorCheckValue = glGetError();

    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &VBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);

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

/* Generate and load the texture */
void UGenerateTexture() {

    int width, height;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char *image = SOIL_load_image("wood003.jpg", &width, &height, 0, SOIL_LOAD_RGBA);// loads texture file

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture
}


/*
 * When a key is pressed in the window then process this keystroke
 * and perform a behavior.
 */
void KeyboardFunction(unsigned char key, int X, int Y)
{
    // 2D orthographic display
    if (key == '2'){
        // Creates an Orthographic projection (3D objects represented in 2D)
        projection =  glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
    }
    // 3D perspective
    if (key == '3'){
        // Creates a 3D projection
        projection = glm::perspective(45.0f, (GLfloat)WindowWidth / (GLfloat)WindowHeight, 0.1f, 100.0f);
    }

}

void UKeyRelease(unsigned char key, int x, int y){
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
        orbit = true;
    }

    // reset orbit/zoom variables when mouse press is released
    if (state == GLUT_UP) {
        orbit = false;
    }
}

void SetView(){
    // clamp azimuth and altitude to prevent irregular camera angles
    // e.g., a 90-degree camera rotation clamp on the pitch axis
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    if (yaw > 180)
        yaw -= 360;
    if (yaw < -180)
        yaw += 180;

    // Sets the front variable that controls CameraForwardZ
    front.x = 10.0f * cos(glm::radians(yaw));
    front.y = 10.0f * sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw)) * 10.0f;
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

        cout<<"yaw:"<<yaw<<", pitch:"<<pitch<<endl;

    }

    SetView();
}

// Processes the input received from the mouse scroll wheel.
// Only requires y axis input, although some mice have an x-axis on scroll wheels
void UMouseScroll(int wheel, int direction, int x, int y){
    // if current mouse position y vector is less than the previous vector
    // zoom out, otherwise current mouse y vector is greater so zoom in
    if (direction == -1) {
        // zoom out
        // if while zooming out the image will reverse and start zooming in again, so let's
        // control this to not surpass zoomSpeed value
        if (scaleY <= zoomSpeed) {
            scaleX = scaleY = scaleZ = zoomSpeed;
        }else {
            scaleX = scaleY = scaleZ -= zoomSpeed;
        }
    }else {
        // zoom in
        scaleX += zoomSpeed;
        scaleY += zoomSpeed;
        scaleZ += zoomSpeed;
    }

    // Updates with the new mouse coordinates
    lastMouseX = x;
    lastMouseY = y;

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
