#include <shaders.h>
#include <iostream>
#include <glerrors.h>

using namespace std;

Shader create_shader(ShaderType type, const char *src) {
    Shader shader;
    switch (type) {
        case ShaderType::VERTEX:
            shader.id = glCreateShader(GL_VERTEX_SHADER);
            glCheckError();
            break;
        case ShaderType::FRAGMENT:
            shader.id = glCreateShader(GL_FRAGMENT_SHADER);
            glCheckError();
            break;
        default:
            /* Unreachable */
            std::cerr << "Invalid shader type passed\n";
    };

    glShaderSource(shader.id, 1, &src, nullptr);
    glCheckError();
    glCompileShader(shader.id);
    glCheckError();

    glGetShaderiv(shader.id, GL_COMPILE_STATUS, &success);
    glCheckError();

    if (!success) {
        glGetShaderInfoLog(shader.id, 512, nullptr, infoLog);
        glCheckError();
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return shader;
}

std::shared_ptr<ShaderProgram> create_shader_program(const char *vertex_src, const char *fragment_src) {
    shared_ptr<ShaderProgram> program = make_shared<ShaderProgram>();
    Shader vertex_shader = create_shader(ShaderType::VERTEX, vertex_src);
    Shader fragment_shader = create_shader(ShaderType::FRAGMENT, fragment_src);

    program->id = glCreateProgram();
    glCheckError();
    glAttachShader(program->id, vertex_shader.id);
    glCheckError();
    glAttachShader(program->id, fragment_shader.id);
    glCheckError();
    glLinkProgram(program->id);
    glCheckError();

    glGetProgramiv(program->id, GL_LINK_STATUS, &success);
    glCheckError();
    if (!success) {
        glGetProgramInfoLog(program->id, 512, nullptr, infoLog);
        glCheckError();
        std::cerr << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex_shader.id);
    glCheckError();
    glDeleteShader(fragment_shader.id);
    glCheckError();

    return program;
}

