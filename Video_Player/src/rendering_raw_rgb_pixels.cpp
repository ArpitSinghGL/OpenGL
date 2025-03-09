#include "../lib/glad/glad.h"

#include <GLFW/glfw3.h>

#include <iostream>

bool load_raw_rgb_pixel_data(int* width , int* height , unsigned char** data);

int main()
{
    // Initializing GLFW
    if( !glfwInit() )
    {
        printf("GLFW initialization failed !\n");
        return -1;
    }

    // Creating a GLFW window
    GLFWwindow* window = glfwCreateWindow(800 , 800 , "Video Player" , NULL , NULL);

    if(!window)
    {
        printf("Could not create a window\n");
        glfwTerminate();
        return -1;
    }

    int frame_width , frame_height;
    unsigned char* frame_data;

    if( !load_raw_rgb_pixel_data(&frame_width , &frame_height , &frame_data) )
    {
        printf("Could not load Video Frame !\n");
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
    
    // Set the OpenGL viewport : Part of the window where rendering happens
    // glViewport(0, 0, 640, 480);

    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Set up an orthographic projection matrix for 2D rendering

        int window_width, window_height;

        glfwGetWindowSize(window, &window_width, &window_height);

        glMatrixMode(GL_PROJECTION);

        glLoadIdentity();

        glOrtho(0, window_width, window_height, 0, -1, 1);

        glMatrixMode(GL_MODELVIEW);

        // glWindowPos2i(650 , 540);

        glDrawPixels(100 , 100 , GL_RGB , GL_UNSIGNED_BYTE , frame_data);

        glfwSwapBuffers(window);

        glfwWaitEvents();
    }

    delete[] frame_data;

    glfwDestroyWindow(window);

    glfwTerminate();
    
    return 0;
}