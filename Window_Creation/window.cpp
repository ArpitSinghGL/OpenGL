#include "./glad/glad.h"

#include <GLFW/glfw3.h>

#include <iostream>


int main()
{
    // Initializing GLFW
    if( !glfwInit() )
    {
        printf("GLFW initialization failed !\n");
        return -1;
    }

    // Creating a GLFW window
    GLFWwindow* window = glfwCreateWindow(800 , 800 , "Image Renderer" , NULL , NULL);

    if(!window)
    {
        printf("Could not create a window\n");
        glfwTerminate();
        return -1;
    }

    //Making the openGL context current
    glfwMakeContextCurrent(window);

    // Load openGL function pointers using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    
    // Set the OpenGL viewport
    glViewport(0, 0, 640, 480);

    unsigned char* buffer = new unsigned char[100 * 100 * 3];

    for(int y = 0 ; y < 100 ; y++)
    {
        for(int x = 0 ; x < 100 ; x++)
        {
            buffer[y * 100 * 3 + 3 * x + 0] = 0xff;   // R
            buffer[y * 100 * 3 + 3 * x + 1] = 0x00;   // G
            buffer[y * 100 * 3 + 3 * x + 2] = 0x00;   // B
        }
    }

    for(int y = 25 ; y < 75 ; y++)
    {
        for(int x = 25 ; x < 75 ; x++)
        {
            buffer[y * 100 * 3 + 3 * x + 0] = 0x00;   // R
            buffer[y * 100 * 3 + 3 * x + 1] = 0x00;   // G
            buffer[y * 100 * 3 + 3 * x + 2] = 0xff;   // B
        }
    }

    GLuint* tex_handle;

    // Generating a Texture
    glGenTextures(1 , tex_handle);

    // Binding the Texture
    glBindTexture(GL_TEXTURE_2D , *tex_handle);

    glTexImage2D(GL_TEXTURE_2D , 0 , GL_RGB , 100 , 100 , 0 , GL_RGB , GL_UNSIGNED_BYTE , buffer);

    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Set up an orthographic projection matrix for 2D rendering

        glMatrixMode(GL_PROJECTION);

        glLoadIdentity();

        glMatrixMode(GL_MODELVIEW);

        glWindowPos2i(650 , 540);

        glDrawPixels(100 , 100 , GL_RGB , GL_UNSIGNED_BYTE , buffer);

        glfwSwapBuffers(window);

        glfwWaitEvents();
    }

    delete[] buffer;

    glfwDestroyWindow(window);

    glfwTerminate();
    
    return 0;
}