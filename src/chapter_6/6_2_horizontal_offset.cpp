#include <stdio.h>
#include <math.h>

#include "../glad.c"
#include <GLFW/glfw3.h>
#include <amp_lib/amp_def.h>
#include "shader_utils.cpp"

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

            r32 vertices[] =
            {   //   Positions         Colors
                 0.5, -0.5,  0.0, 1.0, 0.0, 0.0,    // bottom right
                -0.5, -0.5,  0.0, 0.0, 1.0, 0.0,    // bottom left
                 0.0,  0.5,  0.0, 0.0, 0.0, 1.0     // top
            };

            u32 vbo;
            glGenBuffers(1, &vbo);

            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


            u32 vao;
            glGenVertexArrays(1, &vao);

            glBindVertexArray(vao);
            // vertex attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(r32), nullptr);
            glEnableVertexAttribArray(0);

            // color attribute
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(r32), (void*)(3 * sizeof(r32)));
            glEnableVertexAttribArray(1);

            compiled_shader shader = compile_shader_program_from_files("shaders/6_2_horizontal_offset.vs", "shaders/shader_basics.fs");

            if (shader.id != SHADER_ERROR)
            {
                while (running && !glfwWindowShouldClose(window))
                {
                    // NOTE(amp): input
                    glfwPollEvents();
                    process_input(window);

                    // NOTE(amp): render
                    // In later chapters, we would bind/unbind vaos/vbos/ebos here,
                    // but since it's all one set for now we just bind before the 
                    // loop and forget about it.
                    glClearColor(0.2, 0.3, 0.3, 1.0);
                    glClear(GL_COLOR_BUFFER_BIT);

                    glUseProgram(shader.id);

                    i32 offset_location = glGetUniformLocation(shader.id, "horizontal_offset");
                    
                    r64 time = glfwGetTime();
                    r32 x_offset = (r32)((sin(time)*0.5));
                    glUniform1f(offset_location, x_offset);

                    glDrawArrays(GL_TRIANGLES, 0, 3);

                    glfwSwapBuffers(window);
                }
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