#ifndef SHADERS_H_
#define SHADERS_H_

#include <memory>
#include <OpenGL/gl.h>

extern GLint success;
extern GLchar infoLog[512];

enum class ShaderType {
    VERTEX,
    FRAGMENT,
};

struct Shader {
    ShaderType shader_type;
    GLuint id;
};

struct ShaderProgram {
    Shader vertex, fragment;
    GLuint id;
};

Shader create_shader(ShaderType type, const char *src);

std::shared_ptr<ShaderProgram> create_shader_program(const char *vertex_src, const char *fragment_src);

#endif //  SHADERS_H_
