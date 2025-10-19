#include <stdlib.h>
#include <stdio.h>

#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "render.h"
#include "input.h"

const char vertexShaderSource[] = {
#include <vertex.vs.h>
};

const char fragmentShaderSource[] = {
#include <fragment.fs.h>
};

// Компиляция шейдера
static unsigned int compileShader(unsigned int type, const char* source);

static unsigned int shaderProgram;
static unsigned int VAO, VBO, EBO;
static unsigned int u_Center;
static unsigned int u_ZoomByAxis;
static unsigned int u_MaxIterations;

int renderInit(void) {
    // Инициализация GLAD
    if (!gladLoadGL(glfwGetProcAddress)) {
        printf("Failed to load GLAD\n");
        return 1;
    }

    // Создание шейдерной программы
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Создание полноэкранного четырёхугольника
    float vertices[] = {-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f};

    unsigned int indices[] = {0, 1, 2, 2, 3, 0};

    // Инициализация OpenGL буфферов
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    // Получение uniform-переменных
    u_Center = glGetUniformLocation(shaderProgram, "u_Center");
    u_ZoomByAxis = glGetUniformLocation(shaderProgram, "u_ZoomByAxis");
    u_MaxIterations = glGetUniformLocation(shaderProgram, "u_MaxIterations");
    return 0;
}

void renderExit(void) {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
}

void renderDraw(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, input_resolutionX, input_resolutionY);

    glUseProgram(shaderProgram);
    glUniform2d(u_Center, input_centerX, input_centerY);
    glUniform2d(u_ZoomByAxis, input_zoom, input_zoom * input_aspectRatio);
    glUniform1ui(u_MaxIterations, input_maxIterations);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}


static unsigned int compileShader(unsigned int type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("Shader compilation error: %s\n", infoLog);
        exit(10);
    }
    return shader;
}
