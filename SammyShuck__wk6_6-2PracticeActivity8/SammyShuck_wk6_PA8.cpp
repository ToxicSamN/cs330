/*
* Author: Sammy Shuck
* Name: CS-330 6-2 Practice Activity 8
* Week: Six
* Assignment: 6-2 Practice Activity 8: Creating a Light Scene
* Course: CS-330-J2956 Comp Graphic and Visualization 20EW2
* Date: 12/6/2020
* Version: 1.0
* Copyright: Copyright © 2017 SNHU COCE
*/

// header inclusions
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

// GLM math header inclusions
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// SOIL image loader inclusion
#include "SOIL2/SOIL2.h"

using namespace std; // standard namespace

#define WINDOW_TITLE "Modern OpenGL" // Window Title Macro

/* Shader program macro */
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version "\n" #Source
#endif

// Variable declaration for shader, window size initialization, buffer and array objects
GLint shaderProgram, WindowWidth = 800, WindowHeight = 600;
GLuint VBO, VAO, texture;
GLfloat degrees = glm::radians(0.0f); // converts float to degrees

// key and fill light colors
glm::vec3 keyLightColor(0.0f, 1.0f, 0.0f); // green light
glm::vec3 fillLightColor(1.0f, 1.0f, 1.0f); // white light

// Light position
glm::vec3 keyLightPos(1.0f, 0.5f, -3.0f);
glm::vec3 fillLightPos(6.0f, 0.0f, -3.0f);

// Camera position
glm::vec3 cameraPosition(-6.0f, 0.0f, -6.0f);

// Camera rotation
float cameraRotation = glm::radians(-45.0f);

/* function prototypes */
void UResizeWindow(int, int);

void URenderGraphics(void);

void UCreateShader(void);

void UCreateBuffers(void);

void UGenerateTexture(void);


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
	   float keySpecularIntensity = 1.0f;
	   float fillSpecularIntensity = 0.1f;

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

// Main Program
int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WindowWidth, WindowHeight);
    glutCreateWindow(WINDOW_TITLE);

    glutReshapeFunc(UResizeWindow);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    UCreateShader();

    UCreateBuffers();

    UGenerateTexture();

    // Use the Shader Program
    glUseProgram(shaderProgram);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color

    glutDisplayFunc(URenderGraphics);

    glutMainLoop();

    // Destroy buffer objects once used
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    return 0;
}

// Resize the window
void UResizeWindow(int w, int h) {
    WindowWidth = w;
    WindowHeight = h;
    glViewport(0, 0, WindowWidth, WindowHeight);
}

// Renders Graphics
void URenderGraphics(void) {
    glEnable(GL_DEPTH_TEST); // Enable z-depth
    // Enable Alpha support
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clears the screen

    glBindVertexArray(VAO); // Activate the vertex array object before rendering and transforming them

    // Transforms the object
    glm::mat4 model;
    model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f)); // Increase the object size by a scale of 2
    model = glm::rotate(model, degrees, glm::vec3(0.0, 1.0f, 0.0f)); // Rotate the object y -45 degrees
    model = glm::translate(model, glm::vec3(0.0, 0.0f, 0.0f)); // Place the object at the center of the viewport

    // Transforms the camera
    glm::mat4 view;
    view = glm::rotate(view, cameraRotation, glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::translate(view, cameraPosition);

    // creates a perspective projection
    glm::mat4 projection;
    projection = glm::perspective(45.0f, (GLfloat) WindowWidth / (GLfloat) WindowHeight, 0.1f, 100.0f);

    // Retrieves and passes transform matrices to the shader program
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    /* Pass color, light position, and camera position to the shader program
    *
    * uniform vec3 keyLightColor
    * uniform vec3 fillLightColor
    * uniform vec3 keyLightPos
    * uniform vec3 fillLightPos
    * uniform vec3 viewPosition
    */
    GLint keyLightColorLoc = glGetUniformLocation(shaderProgram, "keyLightColor");
    GLint fillLightColorLoc = glGetUniformLocation(shaderProgram, "fillLightColor");
    GLint keyLightPosLoc = glGetUniformLocation(shaderProgram, "keyLightPos");
    GLint fillLightPosLoc = glGetUniformLocation(shaderProgram, "fillLightPos");
    GLint viewPositionLoc = glGetUniformLocation(shaderProgram, "viewPosition");

    // pass color, light, and camera data to the cube shader programs corresponding uniforms
    glUniform3f(keyLightColorLoc, keyLightColor.r, keyLightColor.g, keyLightColor.b);
    glUniform3f(fillLightColorLoc, fillLightColor.r, fillLightColor.g, fillLightColor.b);
    glUniform3f(keyLightPosLoc, keyLightPos.x, keyLightPos.y, keyLightPos.z);
    glUniform3f(fillLightPosLoc, fillLightPos.x, fillLightPos.y, fillLightPos.z);
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    glutPostRedisplay();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, 18);

    glBindVertexArray(0); // Deactivates the vertex array object

    glutSwapBuffers(); // Flips the back buffer with the front buffer every frame. similar to gl flush
}

// Creates the shader program
void UCreateShader() {
    // Vertex shader
    GLint vertexShader = glCreateShader(GL_VERTEX_SHADER); // creates the vertex shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Attaches the vertex shader to the source code
    glCompileShader(vertexShader); // compiles the vertex shader

    // Fragment shader
    GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // Creates the fragment shader
    glShaderSource(fragmentShader, 1, &fragmentShaderSource,
                   NULL); // Attaches the fragment shader to the source code
    glCompileShader(fragmentShader); // compiles the fragment shader

    // Shader Program
    shaderProgram = glCreateProgram(); // Creates the shader program and returns an id
    glAttachShader(shaderProgram, vertexShader); // Attach vertex shader to the shader program
    glAttachShader(shaderProgram, fragmentShader); // Attach fragment shader to the shader program
    glLinkProgram(shaderProgram); // link vertex and fragment shader to shader program

    // delete the vertex and fragment shaders once linked
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

// Creates the buffer and array objects
void UCreateBuffers() {
    // position and texture coordinate data for 36 triangles
    GLfloat vertices[] = {
            // Positions				// Normals				// Texture Coordinates

    		// Back Face
            -0.5f, -0.5f, -0.5f, 		 0.0f,  0.0f, -1.0f, 		0.0f, 0.0f,
             0.5f, -0.5f, -0.5f, 		 0.0f,  0.0f, -1.0f, 		1.0f, 0.0f,
             0.0f,  0.5f,  0.0f, 		 0.0f,  0.0f, -1.0f, 		0.5f, 1.0f,

            // Left Face
             0.0f,  0.5f,  0.0f, 		-1.0f,  0.0f,  0.0f, 		0.5f, 1.0f,
			-0.5f, -0.5f, -0.5f, 		-1.0f,  0.0f,  0.0f, 		0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f, 		-1.0f,  0.0f,  0.0f, 		1.0f, 0.0f,

            // Front Face
            -0.5f, -0.5f,  0.5f, 		 0.0f,  0.0f,  1.0f, 		0.0f, 0.0f,
             0.5f, -0.5f,  0.5f, 		 0.0f,  0.0f,  1.0f, 		1.0f, 0.0f,
             0.0f,  0.5f,  0.0f, 		 0.0f,  0.0f,  1.0f, 		0.5f, 1.0f,

            // Right Face
             0.0f,  0.5f,  0.0f, 		 1.0f,  0.0f,  0.0f, 		0.5f, 1.0f,
             0.5f, -0.5f,  0.5f, 		 1.0f,  0.0f,  0.0f, 		0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f, 		 1.0f,  0.0f,  0.0f, 		1.0f, 0.0f,
            // Bottom Face
             0.5f, -0.5f, -0.5f, 		 0.0f, -1.0f,  0.0f, 		0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 		 0.0f, -1.0f,  0.0f, 		1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f, 		 0.0f, -1.0f,  0.0f, 		1.0f, 1.0f,
             0.5f, -0.5f,  0.5f, 		 0.0f, -1.0f,  0.0f, 		0.0f, 1.0f
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

/* Generate and load the texture */
void UGenerateTexture() {

	int width, height;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char *image = SOIL_load_image("snhu.jpg", &width, &height, 0, SOIL_LOAD_RGBA);// loads texture file

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture
}

