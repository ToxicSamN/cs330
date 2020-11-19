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
void UCreateVBO(void);
void DestroyVBO(void);
void UCreateShaders(void);
void DestroyShaders(void);
void Cleanup(void);
void KeyboardFunction(unsigned char, int, int);
void IdleFunction(void);

GLuint
    VertexShaderId,
    FragmentShaderId,
    ProgramId,
    VaoId,
    VboId,
    ColorBufferId,
    IndexBufferId[2],
    ActiveIndexBuffer = 0;

class VertexMeta {
private:
    GLuint numDimensions;

    // vertex num floats is r, g, b, a = 4
    const size_t VertexColorOffset = 4;

public:
    VertexMeta(); // default constructor
    explicit VertexMeta(GLuint d); // constructor setting number of dimensions
    void setDimension(GLuint d);
    GLuint getDimension();
    size_t getColorStride();
    size_t getColorOffset();
    size_t getVertexStride();
    char* getRGBAOffset();
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
char* VertexMeta::getRGBAOffset() {
    return (char*)(sizeof(float) * this->numDimensions);
}

//const VertexMeta VertexMeta;

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
    glutKeyboardFunc(KeyboardFunction); // changes the graphics based upon a keystroke
    glutIdleFunc(IdleFunction);
    glutCloseFunc(Cleanup); // properly cleans up system resources
}

// Implements the UResizeWindow function
void UResizeWindow(int width, int height){
    glViewport(0, 0, width, height);
}

// Implements the URenderGraphics function
void URenderGraphics(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clears the screen

    /* Creates the triangle */
    //GLuint totalVerticies = 48; // Specifies the number of verticies for the triangle i.e. 3
    //glDrawArrays(GL_TRIANGLES, 0, totalVerticies); // Draws the triangles

    // Draw the triangle using the indices
    //glDrawElements(GL_TRIANGLES, totalVerticies, GL_UNSIGNED_SHORT, NULL);
    if (ActiveIndexBuffer == 0) {
        glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_SHORT, NULL);
    } else {
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, NULL);
    }

    glutSwapBuffers(); // Flips the back buffer with the front buffer every frame. Similar to GLFlush
}

// Implements the CreateVBO function
void UCreateVBO(void){
    // define the number of dimension 2D (x, y) coordinates or 3D (x, y, z) coordinates
    VertexMeta VertexInfo = VertexMeta(3);

    // Specifies coordinates for triangle verticies on x and y
    GLfloat verts[]= {

            // 0
            0.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f,

            // Top
            // 1
            -0.2f, 0.8f, 0.0f,
            0.0f, 1.0f, 0.0f, 1.0f,
            // 2
            0.2f, 0.8f, 0.0f,
            0.0f, 0.0f, 1.0f, 1.0f,
            // 3
            0.0f, 0.8f, 0.0f,
            0.0f, 1.0f, 1.0f, 1.0f,
            // 4
            0.0f, 1.0f, 0.0f,
            1.0f, 0.0f, 0.0f, 1.0f,

            // Bottom
            // 5
            -0.2f, -0.8f, 0.0f,
            0.0f, 0.0f, 1.0f, 1.0f,
            // 6
            0.2f, -0.8f, 0.0f,
            0.0f, 1.0f, 0.0f, 1.0f,
            // 7
            0.0f, -0.8f, 0.0f,
            0.0f, 1.0f, 1.0f, 1.0f,
            // 8
            0.0f, -1.0f, 0.0f,
            1.0f, 0.0f, 0.0f, 1.0f,

            // Left
            // 9
            -0.8f, -0.2f, 0.0f,
            0.0f, 1.0f, 0.0f, 1.0f,
            // 10
            -0.8f, 0.2f, 0.0f,
            0.0f, 0.0f, 1.0f, 1.0f,
            // 11
            -0.8f, 0.0f, 0.0f,
            0.0f, 1.0f, 1.0f, 1.0f,
            // 12
            -1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f, 1.0f,

            // Right
            // 13
            0.8f, -0.2f, 0.0f,
            0.0f, 0.0f, 1.0f, 1.0f,
            // 14
            0.8f, 0.2f, 0.0f,
            0.0f, 1.0f, 0.0f, 1.0f,
            // 15
            0.8f, 0.0f, 0.0f,
            0.0f, 1.0f, 1.0f, 1.0f,
            // 16
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f, 1.0f,
    };

    // Stores the size of the verts array / number of coordinates needed for the triangle i.e. 6
    float numVerticies = sizeof(verts);

    glGenVertexArrays(1, &VaoId);
    glBindVertexArray(VaoId);

    //GLuint VboId; // variable for vertex buffer object id
    glGenBuffers(2, &VboId); // Creates 2 buffers
    glBindBuffer(GL_ARRAY_BUFFER, VboId); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, numVerticies, verts, GL_STATIC_DRAW); // sends vertex or coordinate data to the GPU

    /* Creates the vertex Attribute Pointer */
    //GLuint floatsPerVertex = VertexMeta.VertexCoords3D; // Number of coordinates per vertex
    glEnableVertexAttribArray(0); // Specifies the initial position of the coordinates in the buffer
    /*
     * Instructs the GPU on how to handle the vertex buffer object data.
     * Parameters: attribPointerPosition | coordinates per vertex | data type | deactivate normalization | 0 strides | o offset
     */
    glVertexAttribPointer(0, VertexInfo.getDimension(), GL_FLOAT, GL_FALSE, VertexInfo.getVertexStride(), 0);

    /*Sets an attribute pointer position for the vertex colors i.e. Attribute 1 for rgba floats. Attribute 0 was for position x, y */
    glEnableVertexAttribArray(1); // Specifies position 1 for the color values in the buffer
    // Parameters: attribPosition 1 | floats per color is 4 i.e. rgba | data type | deactivaate normalization | 7 strides until the next color i.e. rgbaxyz | 3 floats until the beginning of each color
    glVertexAttribPointer(1, VertexInfo.getColorOffset(), GL_FLOAT, GL_FALSE, VertexInfo.getColorStride(), VertexInfo.getRGBAOffset());

    /*
     * Create a buffer object for the indicies
     */
    GLushort indicies[] = {
            // Top
            0, 1, 3,
            0, 3, 2,
            3, 1, 4,
            3, 4, 2,
            // Bottom
            0, 5, 7,
            0, 7, 6,
            7, 5, 8,
            7, 8, 6,
            // Left
            0, 9, 11,
            0, 11, 10,
            11, 9, 12,
            11, 12, 10,
            // Right
            0, 13, 15,
            0, 15, 14,
            15, 13, 16,
            15, 16, 14
    };

    GLushort AlternateIndices[] = {
            // Outer square border:
            3, 4, 16,
            3, 15, 16,
            15, 16, 8,
            15, 7, 8,
            7, 8, 12,
            7, 11, 12,
            11, 12, 4,
            11, 3, 4,

            // Inner square
            0, 11, 3,
            0, 3, 15,
            0, 15, 7,
            0, 7, 11
    };

    glGenBuffers(2, IndexBufferId);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(AlternateIndices), AlternateIndices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId[0]);
}

void DestroyVBO(void){
    GLenum ErrorCheckValue = glGetError();

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &VboId);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(2, IndexBufferId);

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

// Implements the UCreateShaders function
void UCreateShaders(void){
    // Create a shader program object
    ProgramId = glCreateProgram();

    VertexShaderId = glCreateShader(GL_VERTEX_SHADER); // Create a Vertex Shader Object
    FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER); // Create a vertex shader object

    glShaderSource(VertexShaderId, 1, &VertexShader, NULL); // retrieves the vertex shader source code
    glShaderSource(FragmentShaderId, 1, &FragmentShader, NULL); // retrieves the fragment shader source code

    glCompileShader(VertexShaderId); // Compile the vertex shader
    glCompileShader(FragmentShaderId); // Compile the fragment shader

    // Attaches the vertex and fragment shader to the shader program
    glAttachShader(ProgramId, VertexShaderId);
    glAttachShader(ProgramId, FragmentShaderId);

    glLinkProgram(ProgramId); // Links the shader program
    glUseProgram(ProgramId); // Uses the shader program
}

void DestroyShaders(void){
    GLenum ErrorCheckValue = glGetError();

    glUseProgram(0);

    glDetachShader(ProgramId, VertexShaderId);
    glDetachShader(ProgramId, FragmentShaderId);

    glDeleteShader(FragmentShaderId);
    glDeleteShader(VertexShaderId);

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

void KeyboardFunction(unsigned char Key, int X, int Y)
{
    switch (Key)
    {
        case 'T':
            ActiveIndexBuffer = 1;
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId[ActiveIndexBuffer]);
            break;
        case 't':
        {
            ActiveIndexBuffer = 0;
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId[ActiveIndexBuffer]);
            break;
        }
        default:
            break;
    }
}

void IdleFunction(void)
{
    glutPostRedisplay();
}
