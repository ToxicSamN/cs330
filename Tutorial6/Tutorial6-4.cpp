/*
* Author: Sammy Shuck
* Name: CS-330 Tutorial 6-4
* Week: Six
* Assignment: NA
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
GLint cubeShaderProgram, lampShaderProgram, WindowWidth = 800, WindowHeight = 600;
GLuint VBO, CubeVAO, LightVAO, texture;
GLfloat degrees = glm::radians(0.0f); // converts float to degrees

// Subject position and scale
glm::vec3 cubePosition(0.0f, 0.0f, 0.0f);
glm::vec3 cubeScale(2.0f);

// Cube and Light color
glm::vec3 objectColor(0.6f, 0.5f, 0.75f); 
glm::vec3 lightColor(1.0f, 1.0f, 1.0f); 


// Light position and scale
glm::vec3 lightPosition(0.5f, 0.5f, -3.0f);
glm::vec3 lightScale(0.3f);

// Camera position
glm::vec3 cameraPosition(0.0f, 0.0f, -6.0f);

// Camera rotation
float cameraRotation = glm::radians(-25.0f);

/* function prototypes */
void UResizeWindow(int, int);

void URenderGraphics(void);

void UCreateShader(void);

void UCreateBuffers(void);

void UGenerateTexture(void);

/* Vertex Shader source code */
const GLchar * cubeVertexShaderSource = GLSL(330,
	layout (location = 0) in vec3 position; // VAP position 0 for vertex position data
	layout (location = 1) in vec3 normal; // VAP position 1 for normals

	out vec3 Normal; // For outgoing normals to fragment shader
	out vec3 FragmentPos; // for outgoing color / pixels to fragment shader

	//global variables for the transform matrices
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main(){
		gl_Position = projection * view * model * vec4(position, 1.0f); //transforms vertices to clip coordinates

		FragmentPos = vec3(model * vec4(position, 1.0f)); // gets fragment / pixel position in world space only

		Normal = mat3(transpose(inverse(model))) * normal; // get normal vectors in world space only and exclude normal translation properties
	}
);

/* fragment shader source code */
const GLchar * cubeFragmentShaderSource = GLSL(330,
	   in vec3 Normal; //For incoming normals
	   in vec3 FragmentPos; //for incoming fragment position

	   out vec4 cubeColor; //for outgoing light color to the GPU

	   //Uniform / Global variables for object color, light color, light position and camera/view position
	   uniform vec3 objectColor;
	   uniform vec3 lightColor;
	   uniform vec3 lightPos;
	   uniform vec3 viewPosition;

	   void main() {

		   /* Calculate Ambient Lighting */
		   float ambientStrength = 0.1f;
		   vec3 ambient = ambientStrength * lightColor; // Generate ambient light color

		   /* Calculate Diffuse Lighting */
		   vec3 norm = normalize(Normal); // Normalize vectors to 1 unit
		   vec3 lightDirection = normalize(lightPos - FragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on
		   float impact = max(dot(norm, lightDirection), 0.0); // Calculate diffuse impact by generating dot product of normal and light
		   vec3 diffuse = impact * lightColor; // Generate diffuse light color

		   /* Calculate Specular Lighting */
		   float specularIntensity = 1.8f;
		   float highlightSize = 16.0f;
		   vec3 viewDir = normalize(viewPosition - FragmentPos); // Calculate view direction
		   vec3 reflectDir = reflect(-lightDirection, norm); // Calculate reflection vector
			   // Calculate specular component
			   float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
		   vec3 specular = specularIntensity * specularComponent * lightColor;

		   /* Calculate phong result */
		   vec3 phong = (ambient + diffuse + specular) * objectColor;

		   cubeColor = vec4(phong, 1.0f); // Send lighting results to GPU
);

/* Light Vertex Shader source code */
const GLchar * lampVertexShaderSource = GLSL(330,
	 layout(location = 0) in vec3 position; // VAP position 0 for vertex position data
	 
	 // uniform / global variables for the transform matrices
	 uniform mat4 model;
	 uniform mat4 view;
	 uniform mat4 projection;

	 void main() {
		 gl_Position = projection * view * model * vec4(position, 1.0f);// Transforms vertices into clip coordinates
	 }
);

/* Light Fragment Shader source code */
const GLchar * lampFragmentShaderSource = GLSL(330,
   
	out vec4 color; // For outgoing lamp color (smaller cube) to the GPU

	void main(){
		color = vec4(1.0f); // set color to white (1.0f, 1.0f, 1.0f) with alpha 1.0
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

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color

    glutDisplayFunc(URenderGraphics);

    glutMainLoop();

    // Destroy buffer objects once used
    glDeleteVertexArrays(1, &CubeVAO);
    glDeleteVertexArrays(1, &LightVAO);
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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clears the screen
    
    /****** Use the Cube Shader and activate the Cube Vertes Array Object for rendering and transformin *****/
    glUseProgram(cubeShaderProgram);
    glBindVertexArray(CubeVAO);

    // Transforms the cube
    glm::mat4 model;
    model = glm::translate(model, cubePosition);
    model = glm::scale(model, cubeScale);

    // Transforms the camera
    glm::mat4 view;
    view = glm::translate(view, cameraPosition);
    view = glm::rotate(view, cameraRotation, glm::vec3(0.0f, 1.0f, 0.0f));

    // creates a perspective projection
    glm::mat4 projection;
    projection = glm::perspective(45.0f, (GLfloat) WindowWidth / (GLfloat) WindowHeight, 0.1f, 100.0f);

    // Retrieves and passes transform matrices to the shader program
    GLint modelLoc = glGetUniformLocation(cubeShaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(cubeShaderProgram, "view");
    GLint projLoc = glGetUniformLocation(cubeShaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    /*
     * uniform vec3 objectColor;
	   uniform vec3 lightColor;
	   uniform vec3 lightPos;
	   uniform vec3 viewPosition;
     */
    // Reference matrix uniforms from the Cube Shader Program
    GLint objectColorLoc = glGetUniformLocation(cubeShaderProgram, "objectColor");
    GLint lightColorLoc = glGetUniformLocation(cubeShaderProgram, "lightColor");
    GLint lightPositionLoc = glGetUniformLocation(cubeShaderProgram, "lightPos");
    GLint viewPositionLoc = glGetUniformLocation(cubeShaderProgram, "viewPosition");

    // pass color, light, and camera data to the cube shader programs corresponding uniforms
    glUniform3f(objectColorLoc, objectColor.r, objectColor.g, objectColor.b);
    glUniform3f(lightColorLoc, lightColor.r, lightColor.g, lightColor.b);
    glUniform3f(lightPositionLoc, lightPosition.x, lightPosition.y, lightPosition.z);
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0); // Deactivates the vertex array object
    
    /****** Use the Cube Shader and activate the Cube Vertes Array Object for rendering and transformin *****/
	glUseProgram(lampShaderProgram);
	glBindVertexArray(LightVAO);

	// Transforms the lamp
	model = glm::translate(model, lightPosition);
	model = glm::scale(model, lightScale);
	
	// Retrieves and passes transform matrices to the shader program
	modelLoc = glGetUniformLocation(lampShaderProgram, "model");
	viewLoc = glGetUniformLocation(lampShaderProgram, "view");
	projLoc = glGetUniformLocation(lampShaderProgram, "projection");
	
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	
	// Draws the triangles
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0); // Deactivates the vertex array object
	
	glutPostRedisplay();
    glutSwapBuffers(); // Flips the back buffer with the front buffer every frame. similar to gl flush
}

// Creates the shader program
void UCreateShader() {
    // Cube Vertex shader
    GLint cubeVertexShader = glCreateShader(GL_VERTEX_SHADER); // creates the vertex shader
    glShaderSource(cubeVertexShader, 1, &cubeVertexShaderSource, NULL); // Attaches the vertex shader to the source code
    glCompileShader(cubeVertexShader); // compiles the vertex shader

    // Cube Fragment shader
    GLint cubeFragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // Creates the fragment shader
    glShaderSource(cubeFragmentShader, 1, &cubeFragmentShaderSource, NULL); // Attaches the fragment shader to the source code
    glCompileShader(cubeFragmentShader); // compiles the fragment shader

    // Cube Shader Program
    cubeShaderProgram = glCreateProgram(); // Creates the shader program and returns an id
    glAttachShader(cubeShaderProgram, cubeVertexShader); // Attach vertex shader to the shader program
    glAttachShader(cubeShaderProgram, cubeFragmentShader);; // Attach fragment shader to the shader program
    glLinkProgram(cubeShaderProgram); // link vertex and fragment shader to shader program

    // delete the vertex and fragment shaders once linked
    glDeleteShader(cubeVertexShader);
    glDeleteShader(cubeFragmentShader);
    
    // Lamp Vertex shader
    GLint lampVertexShader = glCreateShader(GL_VERTEX_SHADER); // creates the vertex shader
    glShaderSource(lampVertexShader, 1, &lampVertexShaderSource, NULL); // Attaches the vertex shader to the source code
    glCompileShader(lampVertexShader); // compiles the vertex shader

    // Lamp Fragment shader
    GLint lampFragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // Creates the fragment shader
    glShaderSource(lampFragmentShader, 1, &lampFragmentShaderSource, NULL); // Attaches the fragment shader to the source code
    glCompileShader(lampFragmentShader); // compiles the fragment shader

    // Lamp Shader Program
    lampShaderProgram = glCreateProgram(); // Creates the shader program and returns an id
    glAttachShader(lampShaderProgram, lampVertexShader); // Attach vertex shader to the shader program
    glAttachShader(lampShaderProgram, lampFragmentShader);; // Attach fragment shader to the shader program
    glLinkProgram(lampShaderProgram); // link vertex and fragment shader to shader program

    // delete the vertex and fragment shaders once linked
    glDeleteShader(lampVertexShader);
    glDeleteShader(lampFragmentShader);
}

// Creates the buffer and array objects
void UCreateBuffers() {
    // position and texture coordinate data for 36 triangles
    GLfloat vertices[] = {
    		// Position Vec         // Normals Vec
    		
    		// Back Face			// Negative z Normals
			-0.5f, -0.5f, -0.5f, 	 0.0f,  0.0f, -1.0f,
			 0.5f, -0.5f, -0.5f, 	 0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f, 	 0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f, 	 0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f, 	 0.0f,  0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f, 	 0.0f,  0.0f, -1.0f,

			// Front Face			// Positive z Normals
			-0.5f, -0.5f,  0.5f, 	 0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f,  0.5f, 	 0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f,  0.5f, 	 0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f,  0.5f, 	 0.0f,  0.0f,  1.0f,
			-0.5f,  0.5f,  0.5f, 	 0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f, 	 0.0f,  0.0f,  1.0f,

			// Left Face			// Negative x Normals
			-0.5f,  0.5f,  0.5f, 	-1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, 	-1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, 	-1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, 	-1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f, 	-1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, 	-1.0f,  0.0f,  0.0f,

			// Right Face			// Positive x Normals
			 0.5f,  0.5f,  0.5f, 	 1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f, -0.5f, 	 1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f, 	 1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f, 	 1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f, 	 1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f, 	 1.0f,  0.0f,  0.0f,

			 // Bottom Face			// Negative y Normals
			-0.5f, -0.5f, -0.5f, 	 0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f, -0.5f, 	 0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f, 	 0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f, 	 0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f, 	 0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, 	 0.0f, -1.0f,  0.0f,

			// Top Face				// Positive y Normals
			-0.5f,  0.5f, -0.5f, 	 0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f, 	 0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f, 	 0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f, 	 0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, 	 0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, 	 0.0f,  1.0f,  0.0f,
    };

    // Generate buffer ids
    glGenVertexArrays(1, &CubeVAO);
    glGenBuffers(1, &VBO);

    // Activate the VAO before binding and setting VBOs and VAPs
    glBindVertexArray(CubeVAO);

    // Activate the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Copy vertices to VBO

    // set attribute pointer 0 to hold position data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0); // Enables vertex attribute

    // Set attribute pointer 1 to hold Normal data
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // deactivates the vertex array object
    
    // Generate buffer ids for lamp (smaller cube)
    glGenVertexArrays(1, &LightVAO); // Vertex Array Object for Cube vertex copies to serve as light source
    glBindVertexArray(LightVAO);

    // Referencing the same VBO for its vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Set attribute pointer 0 to hold position coordinate data (used for the lamp)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // deactivates the vertex array object
}

// Generate and load the texture
void UGenerateTexture() {

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    int width, height;

    unsigned char *image = SOIL_load_image("snhu.jpg", &width, &height, 0, SOIL_LOAD_RGB);// loads texture file

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture
}
