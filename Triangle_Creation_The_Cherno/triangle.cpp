#include "./glad/glad.h"

#include <GLFW/glfw3.h>

#include <stdio.h>

#include <iostream>

static unsigned int createShader_compileShader(unsigned int type , const std::string& source)
{
    unsigned int id = glCreateShader(type);
    // Coverting const std :: string to const char*
    const char* src = source.c_str();
    // Replaces/Sets the source code of the shader with handle = id to the source code provided
    // in "src"
    glShaderSource(id , 1 , &src , nullptr);  // Look up openGL docs
    glCompileShader(id);

    // Error Handling
    int result;
    glGetShaderiv(id , GL_COMPILE_STATUS , &result);
    // result = GL_FALSE implies that the Shader did not compile successfully. 
    if(result == GL_FALSE)
    {
        // Creating a length placeholder to store the length of the Error Message
        int length;
        // Populating the length placeholder with the size of the error message
        glGetShaderiv(id , GL_INFO_LOG_LENGTH , &length);
        // Creating a message buffer to store the error message
        char* message = new char[length];
        // Populating the message buffer with the actual Error Message
        glGetShaderInfoLog(id , length , &length , message);
        // Displaying the Error Message
        std::cout << "Failed to compile " << 
        (type == GL_VERTEX_SHADER ? "vertex" : "fragment") 
        << " shader !" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static  unsigned int createProgram_linkShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = createShader_compileShader(GL_VERTEX_SHADER , vertexShader);
    unsigned int fs = createShader_compileShader(GL_FRAGMENT_SHADER , fragmentShader);

    glAttachShader(program , vs);
    glAttachShader(program , fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main()
{
    // Initializing GLFW
    if (!glfwInit())
    {
        printf("Could not initialize GLFW\n");
        return -1;
    }

    // Creating a GLFW window
    GLFWwindow* window = glfwCreateWindow(500 , 500 , "Window" , NULL , NULL);

    if (!window )
    {
        std :: cout << "Could not create window \n";
        return -1;
    }

    // Making the context of the specified window current for the calling thread
    glfwMakeContextCurrent(window);

    // Load openGL function pointers using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // GLuint = unsigned int
    // GLsizei = int

    // Note: A vertex is not a position. It is just a point in the geometry that we want to draw.
    // A vertex can have various attributes such as:
    // 1. Position
    // 2. Color
    // 3. UV coordinates
    // 4. Texture coordinates
    // 5. Normal

    // We can think of a Vertex as a struct and the vertex attributes as the member variables
    // or properties of the struct Vertex.

    // Data to be put into the vertex buffer object in the GPU VRAM (Video RAM)
    float vertices[] = {
        -0.5f , -0.5f , // Vertex with only one attribute i.e. Position with 2 components
        0.0f , 0.5f ,   // Vertex with only one attribute i.e. Position with 2 components
        0.5f , -0.5f    // Vertex with only one attribute i.e. Position with 2 components
    };

    GLuint buffer_id;  // Stores the ID of the buffer generated by glGenBuffers
    glGenBuffers(1 , &buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER , buffer_id);

    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), vertices, GL_STATIC_DRAW);

    // glVertexAttribPointer(index, size, type, normalized, stride, pointer) :
    // is used in openGL to specify the layout of each vertex attribute within a vertex buffer.

    // This function is called once per vertex attribute.

    // This function is used to tell/instruct openGL how to:
    // Process or interpret the memory layout of the buffer referred by buffer_id.
    // It helps to specify the number of vertices, the number of vertex attributes
    // in a vertex, the offset of each vertex attribute inside a vertex , the data-type of the 
    // components of a vertex attribute, total number of bytes used to describe a vertex.
    
    // 1. index: index of the vertex attribute (in the above case, there is only one vertex attribute
    // i.e. Postion, therefore the index of position is 0)
    // 2. size: denotes the number of components in the vertex attribute
    // 3. type: denotes the data-type of the components of the vertex attribute
    // 4. stride: denotes the number of bytes used to describe a vertex
    // 5. pointer: denotes the offset of the attribute of a vertex.

    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(0 , 2 , GL_FLOAT , GL_FALSE , sizeof(float) * 2 , 0);

    std::string vertexShader = 
    "#version 330 core\n"
    "\n"
    "layout(location = 0) in vec4 position;"
    "\n"
    "void main()\n"
    "{\n"
    "   gl_position = position;\n"
    "}\n";

    std::string fragmentShader = 
    "#version 330 core\n"
    "\n"
    "layout(location = 0) out vec4 color;"
    "\n"
    "void main()\n"
    "{\n"
    "   color = vec4(0.0 , 1.0 , 0.0 , 1.0);\n"
    "}\n";

    unsigned int program = createProgram_linkShader(vertexShader , fragmentShader);
    glUseProgram(program);

    // Loop until the user closes the window
    while( !glfwWindowShouldClose(window) )
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES , 0 , 3);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process the events
        glfwPollEvents();
    }

    glDeleteProgram(program);

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}