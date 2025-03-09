#include "../lib/glad/glad.h"

#include <GLFW/glfw3.h>

#include <iostream>

bool load_grayscale_frames(const char* filename , int* width_out , int* height_out , unsigned char** data_out);

int main()
{
    // Initializing GLFW
    if( !glfwInit() )
    {
        printf("GLFW initialization failed !\n");
        return -1;
    }

    int frame_width;
    int frame_height;
    unsigned char* frame_data;
        
    if( !load_grayscale_frames("/home/arpit-singh/Documents/openGL/Video_Player/assets/sample_mkv_file.mkv" , &frame_width , &frame_height , &frame_data) )
    {
        printf("Could not load Video Frame !\n");
        return -1;
    }

    printf("frame_width: %d\n" , frame_width);
    printf("frame_height: %d\n" , frame_height);

    // Creating a GLFW window
    GLFWwindow* window = glfwCreateWindow(frame_width , frame_height , "Video Player" , NULL , NULL);

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

    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Set up an orthographic projection matrix for 2D rendering

        int window_width, window_height;

        glfwGetWindowSize(window, &window_width, &window_height);

        glfwGetFramebufferSize(window, &window_width, &window_height);

        glViewport(0, 0, window_width, window_height);

        glMatrixMode(GL_PROJECTION);

        glLoadIdentity();

        glOrtho(0, window_width, window_height, 0, -1, 1);

        glMatrixMode(GL_MODELVIEW);

        glDrawPixels(frame_width , frame_height , GL_RGB , GL_UNSIGNED_BYTE , frame_data);

        glfwSwapBuffers(window);

        glfwWaitEvents();
    }

    delete[] frame_data;

    glfwDestroyWindow(window);

    glfwTerminate();
    
    return 0;
}