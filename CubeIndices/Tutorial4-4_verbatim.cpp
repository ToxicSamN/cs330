#include <iostream>
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

/*Variable declarations for shader, window size initialization, buffer and array objects*/
GLint shaderProgram, WindowWidth=800, WindowHeight=600;
GLuint VBO, VAO, EBO, texture;

void UResizeWindow(int,int);
void URenderGraphics(void);
void UCreateShader(void);
void UCreateBuffers(void);

/*Vertex Shader Program Source Code*/
const GLchar * vertexShaderSource = GLSL(330,
                                         layout(location=0) in vec3 position; // Receive vertex coordinates from attribute 0. i.e. 2 floats per vertex

                                                 /*Get the vertex colors from the Vertex Buffer object*/
                                                 layout(location=1) in vec3 color; // for attribute 1 expect vec(4) floats passed into the vertex shader

                                                 out vec3 mobileColor; // declare a vec 4 variable that will referencethe vertex colors passed into the Vertex shader from the buffer

                                                 // Global variable for the transform matrices
                                                 uniform mat4 model;
                                                 uniform mat4 view;
                                                 uniform mat4 projection;

                                                 void main(){
                                                     gl_Position = projection * view * model * vec4(position, 1.0f); // Send the vertex positions to gl_Position vec 4
                                                     mobileColor = color; // references vertex colors sent from the buffer
                                                 }
                                    );

/*  Fragment Shader Program Source Code*/
const GLchar * fragmentShaderSource = GLSL(330,
                                           in vec3 mobileColor; // Vertex colors from the vertex shader

                                                   out vec4 gpuColor; // vec 4 variable that will reference the vertex colors passed into the fragment shader from the vertex shader

                                                   void main(){
                                                       gpuColor = vec4(mobileColor, 1.0f); // Send the vertex colors to the GPU
                                                   }
                                      );

/* Main Program */
int main(int argc, char* argv[]) {
    glutInit(&argc, argv); // Initialize the OpenGL program
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WindowWidth, WindowHeight);
    glutCreateWindow(WINDOW_TITLE);

    glutReshapeFunc(UResizeWindow);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK){
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    UCreateShader();
    UCreateBuffers();

    glUseProgram(shaderProgram);

    glClearColor(0.0f,0.0f,0.0f,1.0f);

    glutDisplayFunc(URenderGraphics);

    glutMainLoop(); // Starts the OpenGL loop in the background

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    //exit(EXIT_SUCCESS); // Terminates the program successfully
    return 0;
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

    glBindVertexArray(VAO);

    // Transforms the object
    glm::mat4  model;
    // place object at the center of the viewport
    model = glm::translate(model, glm::vec3(0.0f,0.0f,0.0f));
    // rotate object 45 degrees
    model = glm::rotate(model, 45.0f, glm::vec3(1.0f,1.0f,1.0f));
    // increase object size by scale of 2
    model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

    // Transforms the camera
    glm::mat4 view;
    // Move the world 0.5 units on x axis and -5 units on z axis
    view = glm::translate(view, glm::vec3(0.5f,0.0f,-5.0f));

    // Creates a perspective projection
    glm::mat4 projection;
    //projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f,100.0f);
    projection = glm::perspective(45.0f, (GLfloat)WindowWidth / (GLfloat)WindowHeight, 0.1f, 100.0f);

    // Retrieves and passes transform matrices to the shader program
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glutPostRedisplay();

    /* Creates the triangle */
    // Draw the triangle using the indices
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0); // deactivates the vertex array object

    glutSwapBuffers(); // flips the back buffer with the front buffer every frame
}

/* Creates the shader program */
void UCreateShader(void){

    // Vertex Shader
    GLint vertexShader = glCreateShader(GL_VERTEX_SHADER); // Create a Vertex Shader Object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // retrieves the vertex shader source code
    glCompileShader(vertexShader); // Compile the vertex shader

    // Fragrment Shader
    GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // Create a vertex shader object
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); // retrieves the fragment shader source code
    glCompileShader(fragmentShader); // Compile the fragment shader

    // Create a shader program object
    shaderProgram = glCreateProgram();

    // Attaches the vertex and fragment shader to the shader program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram); // Links the shader program

    // delete the vertex and fragment shaders once linked
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

/* Creates the buffer and array objects */
void UCreateBuffers(void){
    GLfloat vertices[] = {
            // Vertex Positions   // Colors
             0.5f,  0.5f,  0.0f,   1.0f,  0.0f,  0.0f, // Top Right Vertex 0
             0.5f, -0.5f,  0.0f,   0.0f,  1.0f,  0.0f, // Bottom Right Vertex 1
            -0.5f, -0.5f,  0.0f,   0.0f,  0.0f,  1.0f, // Bottom Left Vertex 2
            -0.5f,  0.5f,  0.0f,   1.0f,  0.0f,  1.0f, // Top Left Vertex 3

              0.5f, -0.5f, -1.0f,   0.5f,  0.5f,  1.0f, // Bottom Right Right Vertex 4
              0.5f,  0.5f, -1.0f,   1.0f,  1.0f,  0.5f, // Top Left Right Vertex 5
             -0.5f,  0.5f, -1.0f,   0.2f,  0.2f,  0.5f, // Top Left Top Vertex 6
             -0.5f, -0.5f, -1.0f,   1.0f,  0.0f,  1.0f, // Bottom Left back Vertex 7
    };

    // Index data to share position data
    GLuint indices[] = {
            0,1,3, // Triangle 1
            1,2,3, // Triangle 2
            0,1,4, // Triangle 3
            0,4,5, // Triangle 4
            0,5,6, // Triangle 5
            0,3,6, // Triangle 6
            4,5,6, // Triangle 7
            4,6,7, // Triangle 8
            2,3,6, // Triangle 9
            2,6,7, // Triangle 10
            1,4,7, // Triangle 11
            1,2,7  // Triangle 12
    };

    // Generate buffer ids
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1,&EBO);

    // Activate the vertex array object befre build and setting any VBOs and Vertex Attriibute Pointers
    glBindVertexArray(VAO);

    // Activate VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Activate EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set attribute pointer 0 to hold position data
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Set attribute pointer 1 to hold color data
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // deactivates he VAO which is good practice
}