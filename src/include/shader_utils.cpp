#pragma once

// disable vs warning about using fgets
#pragma warning(push)
#pragma warning(disable:4996)
// TODO(amp): remove c++ libs, write platform-specific file i/o code.

#include <stdio.h>

#include <amp_lib\amp_def.h>
#include <glad/glad.h> // include glad to get the required OpenGL headers

#include <stdio.h>

#define SHADER_ERROR UINT_MAX

struct compiled_shader
{
    u32 id;
};

internal compiled_shader
compile_shader_program_from_files(char* vs_path, char* fs_path)
{
    compiled_shader result = { UINT_MAX };

    FILE* vs_file = fopen(vs_path, "r");
    if (vs_file)
    {
        FILE *fs_file = fopen(fs_path, "r");
        if (fs_file)
        {
            fseek(vs_file, 0L, SEEK_END);
            u32 vs_len = ftell(vs_file);
            rewind(vs_file);

            fseek(fs_file, 0L, SEEK_END);
            u32 fs_len = ftell(fs_file);
            rewind(fs_file);

            char* vs_code = (char*)calloc(1, vs_len + 1);
            char* fs_code = (char*)calloc(1, fs_len + 1);

            if (vs_code && fs_code)
            {

                fread((void*)vs_code, sizeof(char), vs_len, vs_file);
                fread((void*)fs_code, sizeof(char), fs_len, fs_file);

                b32 succeeded;
                char log[512];

                u32 vertex_shader = glCreateShader(GL_VERTEX_SHADER);
                glShaderSource(vertex_shader, 1, &vs_code, nullptr);
                glCompileShader(vertex_shader);

                glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &succeeded);
                if (succeeded)
                {
                    u32 frag_shader;
                    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
                    glShaderSource(frag_shader, 1, &fs_code, nullptr);
                    glCompileShader(frag_shader);

                    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &succeeded);
                    if (succeeded)
                    {
                        u32 shader_program;
                        shader_program = glCreateProgram();
                        glAttachShader(shader_program, vertex_shader);
                        glAttachShader(shader_program, frag_shader);
                        glLinkProgram(shader_program);

                        glGetProgramiv(shader_program, GL_LINK_STATUS, &succeeded);
                        if (succeeded)
                        {
                            result.id = shader_program;

                            glDeleteShader(vertex_shader);
                            glDeleteShader(frag_shader);
                        }
                        else
                        {
                            glGetProgramInfoLog(shader_program, sizeof(log), nullptr, log);
                            printf("ERROR::SHADER::LINK_FAILED:\n\t%s\n", log);
                        }
                    }
                    else
                    {
                        glGetShaderInfoLog(frag_shader, sizeof(log), nullptr, log);
                        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED:\n\t%s\n\n======================\nSOURCE:\n%s\n", log, fs_code);
                    }
                }
                else
                {
                    glGetShaderInfoLog(vertex_shader, sizeof(log), nullptr, log);
                    printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED:\n\t%s\n\n===================\nSOURCE:\n%s\n", log, vs_code);
                }

                free(vs_code);
                free(fs_code);
            }
            else
            {
                printf("ERROR::compile_shader_program_from_files::CALLOC_FAILED\n");
            }
        }
        else
        {
            printf("ERROR::VERTEX: Cannot load file %s", vs_path);
        }
    }
    else
    {
        printf("ERROR::VERTEX: Cannot load file %s", vs_path);
    }

    return result;
}

#pragma warning(pop)