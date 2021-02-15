#include <stdio.h>
#include <math.h>

#include "glad.c"
#include <GLFW/glfw3.h>
#include <amp_lib/amp_def.h>

struct key_input
{
    b32 was_pressed;
    b32 is_pressed;
};

global_variable b32 running = true;
global_variable b32 wireframe = false;

global_variable key_input w_input = {};

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
    i32 key_press = glfwGetKey(window, GLFW_KEY_W);
    if ( key_press == GLFW_PRESS)
    {
        if(!w_input.was_pressed)
        {
            wireframe = !wireframe;
            w_input.was_pressed = true;
            w_input.is_pressed = true;
        }
        
    }
    else if (key_press == GLFW_RELEASE)
    {
        if (w_input.was_pressed)
        {
            w_input.was_pressed = false;
        }

        w_input.is_pressed = false;

    }

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
            {
                 0.5,  0.5,  0.0, // top right
                 0.5, -0.5,  0.0, // bottom right
                -0.5, -0.5,  0.0, // bottom left
                -0.5,  0.5, 0.0   // top left
            };

            u32 indices[] =
            {
                0, 1, 3, // first triangle
                1, 2, 3  // second triangle
            };

            u32 vbo;
            glGenBuffers(1, &vbo);

            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            u32 vao;
            glGenVertexArrays(1, &vao);

            glBindVertexArray(vao);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(r32), nullptr);
            glEnableVertexAttribArray(0);

            u32 ebo;
            glGenBuffers(1, &ebo);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            const char* vertex_shader_source =
                "#version 330 core\n"
                "layout (location = 0) in vec3 aPos;\n"
                "void main()\n"
                "{\n"
                "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                "}\0";

            u32 vertex_shader;
            vertex_shader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
            glCompileShader(vertex_shader);

            int build_success;
            char log[512];
            glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &build_success);

            if (build_success)
            {
                const char* frag_shader_source =
                    "#version 330 core\n"
                    "out vec4 FragColor;\n"
                    "void main()\n{"
                    "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n}";

                u32 frag_shader;
                frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
                glShaderSource(frag_shader, 1, &frag_shader_source, nullptr);
                glCompileShader(frag_shader);
                glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &build_success);
                if (build_success)
                {
                    u32 shader_program;
                    shader_program = glCreateProgram();
                    glAttachShader(shader_program, vertex_shader);
                    glAttachShader(shader_program, frag_shader);
                    glLinkProgram(shader_program);

                    glGetProgramiv(shader_program, GL_LINK_STATUS, &build_success);
                    if (build_success)
                    {
                        glDeleteShader(vertex_shader);
                        glDeleteShader(frag_shader);

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

                            glPolygonMode( GL_FRONT_AND_BACK, (wireframe ? GL_LINE : GL_FILL) );

                            glUseProgram(shader_program);
                            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                            glfwSwapBuffers(window);
                        }
                    }
                    else
                    {
                        glGetProgramInfoLog(shader_program, sizeof(log), nullptr, log);
                        printf("ERROR::SHADER::LINK_FAILED:\n\t%s", log);
                    }
                }
                else
                {
                    glGetShaderInfoLog(frag_shader, sizeof(log), nullptr, log);
                    printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED:\n\t%s", log);
                }
            }
            else
            {
                glGetShaderInfoLog(vertex_shader, sizeof(log), nullptr, log);
                printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED:\n\t%s", log);
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