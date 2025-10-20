#include <stdio.h>
#include <GLFW/glfw3.h>

#include "render.h"
#include "render_ui.h"
#include "input.h"

#define WIDTH 1280
#define HEIGHT 720

int main(void) {
    // Инициализация GLFW
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Mandelbrot Set", NULL, NULL);
    if (!window) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return 1;
    }

    // Инициализация переменных фреймбуффера. Костыль по большей части
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    framebuffer_resize_callback(NULL, width, height);

    glfwMakeContextCurrent(window);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);

    //glfwSwapInterval(0);

    if (renderInit()) {
        return 1;
    }

    initNuklearUI();

    // Основной цикл рендеринга
    double prev_time = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        double current_time = glfwGetTime();
        double delta = current_time - prev_time;
        prev_time = current_time;

        glfwPollEvents();

        inputInterpolate(delta);
        renderDraw();

        if (input_isMenuShow)
            drawNuklearUI(window, delta);

        glfwSwapBuffers(window);
    }

    renderExit();
    glfwTerminate();
    return 0;
}
