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

const char fragmentShaderFloatSource[] = {
#include <fragment_float.fs.h>
};

// Компиляция шейдера
static unsigned int compileShader(unsigned int type, const char* source);

static unsigned int VAO, VBO, EBO;

static unsigned int shaderProgramMandelbrotFloat;
static unsigned int u_CenterFloat;
static unsigned int u_ZoomByAxisFloat;
static unsigned int u_MaxIterationsFloat;

static unsigned int shaderProgramMandelbrot;
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
    unsigned int fragmentShader;
   
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    shaderProgramMandelbrot = glCreateProgram();
    glAttachShader(shaderProgramMandelbrot, vertexShader);
    glAttachShader(shaderProgramMandelbrot, fragmentShader);
    glLinkProgram(shaderProgramMandelbrot);
    glDeleteShader(fragmentShader);
    
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderFloatSource);
    shaderProgramMandelbrotFloat = glCreateProgram();
    glAttachShader(shaderProgramMandelbrotFloat, vertexShader);
    glAttachShader(shaderProgramMandelbrotFloat, fragmentShader);
    glLinkProgram(shaderProgramMandelbrotFloat);
    glDeleteShader(fragmentShader);

    glDeleteShader(vertexShader);

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
    u_Center = glGetUniformLocation(shaderProgramMandelbrot, "u_Center");
    u_ZoomByAxis = glGetUniformLocation(shaderProgramMandelbrot, "u_ZoomByAxis");
    u_MaxIterations = glGetUniformLocation(shaderProgramMandelbrot, "u_MaxIterations");
    
    u_CenterFloat = glGetUniformLocation(shaderProgramMandelbrotFloat, "u_Center");
    u_ZoomByAxisFloat = glGetUniformLocation(shaderProgramMandelbrotFloat, "u_ZoomByAxis");
    u_MaxIterationsFloat = glGetUniformLocation(shaderProgramMandelbrotFloat, "u_MaxIterations");
    return 0;
}

void renderExit(void) {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgramMandelbrot);
}

void renderDraw(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, input_resolutionX, input_resolutionY);

    if (input_isFloatShader) {
        glUseProgram(shaderProgramMandelbrotFloat);
        glUniform2f(u_CenterFloat, (float)input_centerX, (float)input_centerY);
        glUniform2f(u_ZoomByAxisFloat, (float)input_zoom, (float)(input_zoom * input_aspectRatio));
        glUniform1ui(u_MaxIterationsFloat, input_maxIterations);
    } else {
        glUseProgram(shaderProgramMandelbrot);
        glUniform2d(u_Center, input_centerX, input_centerY);
        glUniform2d(u_ZoomByAxis, input_zoom, input_zoom * input_aspectRatio);
        glUniform1ui(u_MaxIterations, input_maxIterations);
    }
    
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
