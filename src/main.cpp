#include <iostream>
#include <cmath>
#include <cassert>
#include <memory>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Local includes
#include <objparser.h>
#include <glerrors.h>
#include <shaders.h>

using namespace std;

#define glCheckError() glCheckError_(__FILE__, __LINE__) 

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

GLint success;
GLchar infoLog[512];

ObjParser obj_parser;

struct Paddle {
    GLuint VBO;
    GLuint VAO;
    GLuint EBO;

    void init();
    void update(GLuint dt);
    void draw();
};

void Paddle::init() {
}

void Paddle::update(GLuint dt) {
}

void Paddle::draw() {
}

struct AIPaddle : public Paddle {
};

struct Ball {
    GLuint VBO;
    GLuint VAO;
    GLuint EBO;
    size_t n_indexes;

    void init();
    void update(GLuint dt);
    void draw();
};

void Ball::init() {
    ObjModel model = obj_parser.model("/private/tmp/sphere.obj");
    n_indexes = model.indexes.size();

    glGenVertexArrays(1, &VAO); glCheckError();
    glGenBuffers(1, &VBO); glCheckError();
    glGenBuffers(1, &EBO); glCheckError();

    glBindVertexArray(VAO); glCheckError();

    glBindBuffer(GL_ARRAY_BUFFER, VBO); glCheckError();
    glBufferData(GL_ARRAY_BUFFER, model.vertices.size(), &model.vertices[0], GL_STATIC_DRAW); glCheckError();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); glCheckError();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indexes.size(), &model.indexes[0], GL_STATIC_DRAW); glCheckError();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); glCheckError();
    glEnableVertexAttribArray(0); glCheckError();

    glBindVertexArray(0); glCheckError();
}

void Ball::update(GLuint dt) {
}

void Ball::draw() {
    glBindVertexArray(VAO); glCheckError();
    glDrawElements(GL_TRIANGLES, n_indexes, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); glCheckError();
}

Paddle player;
AIPaddle opponent;
Ball ball;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

void update(GLuint dt) {
    ball.update(dt);
    player.update(dt);
    opponent.update(dt);
}

void draw() {
    player.draw();
    opponent.draw();
    ball.draw();
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "PONG!!!", nullptr, nullptr);
    if (window == nullptr) {
        cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    /* Initialize glew */
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cerr << "Failed to initialize GLEW\n";
        return -1;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glfwSetKeyCallback(window, key_callback);

    player.init();
    opponent.init();
    ball.init();

    /* Create a basic shader */
    const char *vertex_shader_src = "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "void main()\n"
        "{\n"
        "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
        "}\n";

    const char *fragment_shader_src = "#version 330 core\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "color = vec4(1.0, 1.0, 1.0, 1.0);\n"
        "}\n";

    shared_ptr<ShaderProgram> basic_shader = create_shader_program(vertex_shader_src, fragment_shader_src);

    GLfloat currentTime = 0.0;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        GLfloat timeValue = glfwGetTime();
        GLuint dt = (timeValue - currentTime) * 1000;

        glUseProgram(basic_shader->id); glCheckError();
        update(dt);
        draw();

        glfwSwapBuffers(window);
        currentTime = timeValue;
    }

    glfwTerminate();

    return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}
