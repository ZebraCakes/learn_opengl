#include <stdio.h>
#include <math.h>

#include "../glad.c"
#include <GLFW/glfw3.h>
#include <amp_lib/amp_def.h>

global_variable b32 running = true;

// NOTE(amp): We want to ignore the unreferenced formal parameter this one time
#pragma warning(push)
#pragma warning(disable:4100)
void framebuffer_size_callback(GLFWwindow* window, i32 width, i32 height)
{
    glViewport(0, 0, width, height);
}
#pragma warning(pop)

void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        running = false;
        glfwSetWindowShouldClose(window, true);
    }
}

int main(/*int arg_count, char** args*/)
{
    int succeeded = 0;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Learn OpenGL", nullptr, nullptr);
    if (window)
    {
        glfwMakeContextCurrent(window);
        
        if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            glViewport(0, 0, 800, 600);
            glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


            r32 sine  = 0;
            r32 cosine = 0;
            while (running && !glfwWindowShouldClose(window))
            {
                // NOTE(amp): input
                glfwPollEvents();
                process_input(window);

                // NOTE(amp): render
                glClearColor(0.2, sinf(sine), cosf(cosine), 1.0);
                glClear(GL_COLOR_BUFFER_BIT);

                glfwSwapBuffers(window);

                sine += 0.01f;
                cosine += 0.03f;
            }
        }
        else
        {
            printf("Failed to initialize GLAD\n");
            succeeded = -1;
        }
    }
    else
    {
        printf("Failed to create GLFW window\n");
        succeeded = -1;
    }

    glfwTerminate();

    return succeeded;

}