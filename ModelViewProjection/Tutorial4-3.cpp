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
GLint shaderProgram, WindowWidth = 800, WindowHeight = 600;
GLuint VBO, VAO, EBO, texture;

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
void IdleFunction(void);

GLuint
        VertexShaderId,
        FragmentShaderId,
        ProgramId,
        VaoId,
        VboId,
        EboId,
        ColorBufferId,
        IndexBufferId[1],
        ActiveIndexBuffer = 0;

///*Vertex Shader Source Code*/
//const GLchar * vertexShaderSource = GLSL(330,
//        layout (location = 0) in vec3 position; //vertex data from the vertex attrib pointer 0
//        layout (location = 1) in vec4 color; // color data from vertex attrib pointer 1
//
//        out vec4 mobileColor; // variable to transfer color data to the fragment shader
//
//        uniform mat4 shaderTransform; // 4x4 matrix variable for transforming vertex data
//
//        void main() {
//            gl_Position = shaderTransform * vec4(position, 1.0f); // transform vertex data using matrix
//            mobileColor = color; // references incoming color data
//        }
//);
//
///*Fragment shader source code*/
//const GLchar * fragmentShaderSource = GLSL(330,
//        in vec4 mobileColor; // Variable to hold incoming color data from vertex shader
//        out vec4 gpuColor; // variable to pass color data to the GPU
//
//        void main(){
//            gpuColor = vec4(mobileColor, 1.0); // sends color data to the GPU for rendering
//        }
//);
/*Vertex Shader Program Source Code*/
const GLchar * vertexShaderSource = GLSL(440,
                                   in layout(location=0) vec3 vertex_Position; // Receive vertex coordinates from attribute 0. i.e. 2 floats per vertex
                                           /*Get the vertex colors from the Vertex Buffer object*/
                                           in layout(location=1) vec3 colorFromVBO; // for attribute 1 expect vec(4) floats passed into the vertex shader
                                           out vec3 colorFromVShader; // declare a vec 4 variable that will referencethe vertex colors passed into the Vertex shader from the buffer

                                           uniform mat4 shaderTransform; // 4x4 matrix variable for transforming vertex data

                                           void main(){
                                               gl_Position = shaderTransform * vec4(vertex_Position, 1.0f); // Send the vertex positions to gl_Position vec 4
                                               colorFromVShader = colorFromVBO; // references vertex colors sent from the buffer
                                           }
                              );

/*  Fragment Shader Program Source Code*/
const GLchar * fragmentShaderSource = GLSL(440,
                                     in vec3 colorFromVShader; // Vertex colors from the vertex shader
                                             out vec4 vertex_Color; // vec 4 variable that will reference the vertex colors passed into the fragment shader from the vertex shader
                                             void main(){
                                                 vertex_Color = vec4(colorFromVShader, 1.0f); // Send the vertex colors to the GPU
                                             }
                                );


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
    if (GLEW_OK != GlewInitResult){
        fprintf(stderr, "ERROR: %s\n", glewGetErrorString(GlewInitResult));
        exit(EXIT_FAILURE);
    }

    // Displays GPU OpenGL version
    fprintf(stderr, "INFO: OpenGL Version: %s\n", glGetString(GL_VERSION));

    UCreateVBO(); // Calls the function to create the vertex buffer object
    UCreateShader(); // Calls the function to create the shader program

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
    //glutKeyboardFunc(KeyboardFunction); // changes the graphics based upon a keystroke
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(VaoId);

    //Declares a 4x4 identity matrix uniform to the handle transformations
    glm::mat4  currentTransform;

    //Moves the 0.5 in y
    currentTransform = glm::translate(currentTransform, glm::vec3(0.0f,0.5f,0.0f));
    //Rotates Shape 45 degrees on the z axis
    currentTransform = glm::rotate(currentTransform, 45.0f, glm::vec3(0.0f,0.0f,1.0f));
    // Scales the shape down by half it's original size in xyz
    currentTransform = glm::scale(currentTransform, glm::vec3(0.5f, 0.5f, 0.5f));

    //Sends transform information to the Vertex shader
    GLuint transformLocation = glGetUniformLocation(ProgramId, "shaderTransform");
    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(currentTransform));

    // Draws the triangles
    /* Creates the triangle */
    // Draw the triangle using the indices
    if (ActiveIndexBuffer == 0) {
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
    } else {
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, NULL);
    }

    glBindVertexArray(0); // deactivates the vertex array object

    glutSwapBuffers(); // flips the back buffer with the front buffer every frame
}

/* Creates the shader program */
void UCreateShader(void){
    // Create a shader program object
    ProgramId = glCreateProgram();

    VertexShaderId = glCreateShader(GL_VERTEX_SHADER); // Create a Vertex Shader Object
    FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER); // Create a vertex shader object

    glShaderSource(VertexShaderId, 1, &vertexShaderSource, NULL); // retrieves the vertex shader source code
    glShaderSource(FragmentShaderId, 1, &fragmentShaderSource, NULL); // retrieves the fragment shader source code

    glCompileShader(VertexShaderId); // Compile the vertex shader
    glCompileShader(FragmentShaderId); // Compile the fragment shader

    // Attaches the vertex and fragment shader to the shader program
    glAttachShader(ProgramId, VertexShaderId);
    glAttachShader(ProgramId, FragmentShaderId);

    glLinkProgram(ProgramId); // Links the shader program
    glUseProgram(ProgramId); // Uses the shader program

    // delete the vertex and fragment shaders once linked
    glDeleteShader(VertexShaderId);
    glDeleteShader(FragmentShaderId);
}
// destoriy the shaders properly
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

/* Creates the buffer and array objects */
void UCreateVBO(){
    // define the number of dimension 2D (x, y) coordinates or 3D (x, y, z) coordinates
    VertexMeta VertexInfo = VertexMeta(3);

    //Position and color data
    GLfloat vertices[] = {
            // vertex positions
            // colors

            // top right vertex 0
            0.5f, 0.5f, 0.0f,
            1.0f, 0.0f, 0.0f,

            // bottom right vertex 1
            0.5f, -0.5f, 0.0f,
            0.0f, 1.0f, 0.0f,

            // bottom left vertex 2
            -0.5f, -0.5f, 0.0f,
            0.0f, 0.0f, 1.0f,

            // top left vertex 3
            -0.5f, 0.5f, 0.0f,
            1.0f, 0.0f, 1.0f,
    };

    glGenVertexArrays(1, &VaoId);
    glBindVertexArray(VaoId);

    //GLuint VboId; // variable for vertex buffer object id
    glGenBuffers(2, &VboId); // Creates 2 buffers
    glBindBuffer(GL_ARRAY_BUFFER, VboId); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // sends vertex or coordinate data to the GPU

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
     * Create a buffer object for the indices
     */
    GLushort indices[] = {
            0,1,3,
            1,2,3,
    };

    glGenBuffers(1, IndexBufferId);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId[0]);

}
// Destory VBO properly
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

/*
 * When a key is pressed in the window then process this keystroke
 * and perform a behavior.
 */
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

/*
 * Mark the normal plane of current window as needing to be redisplayed.
 * The next iteration through glutMainLoop, the window's display callback will be called to
 * redisplay the window's normal plane
 */
void IdleFunction(void)
{
    glutPostRedisplay();
}
