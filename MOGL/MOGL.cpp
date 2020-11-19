#include <iostream>
#include <GL/glew.h> // glew header
#include <GL/freeglut.h> // freeglut header

using namespace std; // uses the standard namespace

#define WINDOW_TITLE "Modern OpenGL" // Macro for window title

//vertex and fragment shader source macro
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version "\n" #Source
#endif

// Variables for window width and height
int WindowWidth = 800, WindowHeight = 600;

/*
 * User defined Function prototypes to:
 * initialize the program, set the window size,
 * redraw graphics on the window when resized,
 * and render the graphics on the screen
 */
void UInitialize(int, char*[]);
void UInitWindow(int, char*[]);
void UResizeWindow(int, int);
void URenderGraphics(void);
void UCreateVBO();
void UCreateShaders(void);

/*Vertex Shader Program Source Code*/
const GLchar * VertexShader = GLSL(440,
in layout(location=0) vec4 vertex_Position; // Receive vertex coordinates from attribute 0. i.e. 2 floats per vertex
    /*Get the vertex colors from the Vertex Buffer object*/
    in layout(location=1) vec4 colorFromVBO; // for attribute 1 expect vec(4) floats passed into the vertex shader
    out vec4 colorFromVShader; // declare a vec 4 variable that will referencethe vertex colors passed into the Vertex shader from the buffer
        void main(){
            gl_Position = vertex_Position; // Send the vertex positions to gl_Position vec 4
            colorFromVShader = colorFromVBO; // references vertex colors sent from the buffer
        }
);

/*  Fragment Shader Program Source Code*/
const GLchar * FragmentShader = GLSL(440,
    in vec4 colorFromVShader; // Vertex colors from the vertex shader
    out vec4 vertex_Color; // vec 4 variable that will reference the vertex colors passed into the fragment shader from the vertex shader
        void main(){
            //gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0); // Sets the pixels / fragments of the triangle to green
            vertex_Color = colorFromVShader; // Send the vertex colors to the GPU
        }
);

// main function, Entry point to the OpenGL program
int main(int argc, char* argv[]){
    UInitialize(argc, argv); // Initialize the OpenGL program
    glutMainLoop(); // Starts the OpenGL loop in the background
    exit(EXIT_SUCCESS); // Terminates the program successfully
}

// Implements the UInitialize function
void UInitialize(int argc, char* argv[]){
    // glew status variable
    GLenum GlewInitResult;

    UInitWindow(argc, argv); // Creates the window

    // checks glew status
    GlewInitResult = glewInit();

    if (GLEW_OK != GlewInitResult){
        fprintf(stderr, "ERROR: %s\n", glewGetErrorString(GlewInitResult));
        exit(EXIT_FAILURE);
    }

    // Displays GPU OpenGL version
    fprintf(stderr, "INFO: OpenGL Version: %s\n", glGetString(GL_VERSION));

    UCreateVBO(); // Calls the function to create the vertex buffer object

    UCreateShaders(); // Calls the function to create the shader program

    // Sets the background color of the window to black, Optional
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

// Implements the UInitWindow function
void UInitWindow(int argc, char* argv[]){
    // Initialize freeglut
    glutInit(&argc, argv);

    // Sets the window size
    glutInitWindowSize(WindowWidth, WindowHeight);

    // Memory buffer setup for display
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    // Creates a window with the macro placeholder title
    glutCreateWindow(WINDOW_TITLE);

    glutReshapeFunc(UResizeWindow); // Called when the window is resized
    glutDisplayFunc(URenderGraphics); // Renders graphics on the screen
}

// Implements the UResizeWindow function
void UResizeWindow(int width, int height){
    glViewport(0, 0, width, height);
}

// Implements the URenderGraphics function
void URenderGraphics(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clears the screen

    /* Creates the triangle */
    GLuint totalVerticies = 6; // Specifies the number of verticies for the triangle i.e. 3
    //glDrawArrays(GL_TRIANGLES, 0, totalVerticies); // Draws the triangles

    // Draw the triangle using the indices
    glDrawElements(GL_TRIANGLES, totalVerticies, GL_UNSIGNED_SHORT, NULL);

    glutSwapBuffers(); // Flips the back buffer with the front buffer every frame. Similar to GLFlush
}

// Implements the CreateVBO function
void UCreateVBO(void){
    // Specifies coordinates for triangle verticies on x and y
    GLfloat verts[]= {
        /*
         * indicies will be 0, 1, 2, 3, 4, 1
         * index 1 will be shared by both triangles
         */
        // index 0
        -0.5f, 0.0f, // top-center of the screen
        1.0f, 0.0f, 0.0f, 1.0f, // Red Vertex

        // index 1
        0.0f, -1.0f, //bottom-left of the screen
        0.0f, 1.0f, 0.0f, 1.0f, // Green Vertex

        // index 2
        -1.0f, -1.0f, // bottom-right of the screen
        0.0f, 0.0f, 1.0f, 1.0f, // Blue Vertex

        // index 3
        0.5f, 0.0f, // top-center of the screen
        1.0f, 0.0f, 0.0f, 1.0f, // Red Vertex

        // index 4
        1.0f, -1.0f, //bottom-left of the screen
        0.0f, 1.0f, 0.0f, 1.0f, // Green Vertex

//        0.0f, -1.0f, // bottom-right of the screen
//        0.0f, 0.0f, 1.0f, 1.0f, // Blue Vertex
    };

    // Stores the size of the verts array / number of coordinates needed for the triangle i.e. 6
    float numVerticies = sizeof(verts);

    GLuint myBufferID; // variable for vertex buffer object id
    glGenBuffers(1, &myBufferID); // Creates 1 buffer
    glBindBuffer(GL_ARRAY_BUFFER, myBufferID); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, numVerticies, verts, GL_STATIC_DRAW); // sends vertex or coordinate data to the GPU

    /* Creates the vertex Attribute Pointer */
    GLuint floatsPerVertex = 2; // Number of coordinates per vertex
    glEnableVertexAttribArray(0); // Specifies the initial position of the coordinatesin the buffer

    /* Strides between vertex coordinates is 6 (x, y, r, g, b, a). A tightly packed stride is 0 */
    GLint vertexStride = sizeof(float) * 6; // The number of floats before each vertex position is 6 i.e xy rgba

    /*
     * Instructs the GPU on how to handle the vertex buffer object data.
     * Parameters: attribPointerPosition | coordinates per vertex | data type | deactivate normalization | 0 strides | o offset
     */
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, vertexStride, 0); // stride of 6

    /*Sets an attribute pointer position for the vertex colors i.e. Attribute 1 for rgba floats. Attribute 0 was for position x, y */
    glEnableVertexAttribArray(1); // Specifies position 1 for the color values in the buffer

    GLint colorStride = sizeof(float) * 6; // The number of floats before each color is 6 i.e rgba xy

    // Parameters: attribPosition 1 | floats per color is 4 i.e. rgba | data type | deactivaate normalization |
    // 6 strides until the next color i.e. rgbaxy | 2 floats until the beginning of each color
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, colorStride, (char*)(sizeof(float) * 2));

    /*
     * Create a buffer object for the indicies
     */
    GLushort indicies[] = {0, 1, 2, 3, 4, 1};
    float numIndicies = sizeof(indicies);
    GLuint indexBufferID;
    glGenBuffers(1, &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndicies, indicies, GL_STATIC_DRAW);
 }

 // Implements the UCreateShaders function
void UCreateShaders(void){
    // Create a shader program object
    GLuint ProgramId = glCreateProgram();

     GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER); // Create a Vertex Shader Object
     GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER); // Create a vertex shader object

     glShaderSource(vertexShaderId, 1, &VertexShader, NULL); // retrieves the vertex shader source code
     glShaderSource(fragmentShaderId, 1, &FragmentShader, NULL); // retrieves the fragment shader source code

     glCompileShader(vertexShaderId); // Compile the vertex shader
     glCompileShader(fragmentShaderId); // Compile the fragment shader

     // Attaches the vertex and fragment shader to the shader program
     glAttachShader(ProgramId, vertexShaderId);
     glAttachShader(ProgramId, fragmentShaderId);

     glLinkProgram(ProgramId); // Links the shader program
     glUseProgram(ProgramId); // Uses the shader program
}
