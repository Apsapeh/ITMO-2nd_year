#include <stdio.h>
#include <math.h>
#include <glad/gl.h>
#include "input.h"

#define CENTER_X_DEFAULT 0.0
#define CENTER_Y_DEFAULT 0.0
#define ZOOM_DEFAULT 1.0
#define MAX_ITERATIONS_DEFAULT 100

#define ZOOM_SPEED 5.0
#define ZOOM_THRESHOLD 0.0001
#define CENTER_SPEED 5.0
#define CENTER_THRESHOLD 0.000000001

// Глобальные переменные для управления камерой
double input_centerX = CENTER_X_DEFAULT;
double input_centerY = CENTER_Y_DEFAULT;
double input_zoom = ZOOM_DEFAULT;
unsigned int input_maxIterations = MAX_ITERATIONS_DEFAULT;

int input_isFloatShader = 1;
unsigned char input_isMenuShow = 1;

// Глобальные переменные для фреймбуффера
int input_resolutionX;
int input_resolutionY;
double input_aspectRatio;

// Переменные для перетаскивания мышью
unsigned char isDragging = 0;
double lastMouseX, lastMouseY;

double wishCenterX = CENTER_X_DEFAULT;
double wishCenterY = CENTER_Y_DEFAULT;
double wishZoom = ZOOM_DEFAULT;

// Переменные для зума к точке
double zoomTargetFractalX = 0.0;
double zoomTargetFractalY = 0.0;
double zoomTargetNormX = 0.0;
double zoomTargetNormY = 0.0;
unsigned char isZooming = 0;

void inputInterpolate(double time_delta) {
    // Интерполяция зума
    if (fabs(wishZoom - input_zoom) > ZOOM_THRESHOLD) {
        double delta = wishZoom - input_zoom;
        input_zoom += delta * ZOOM_SPEED * time_delta;

        if (isZooming) {
            double zoomByAxisX = input_zoom;
            double zoomByAxisY = input_zoom * input_aspectRatio;
            input_centerX = zoomTargetFractalX - zoomTargetNormX / zoomByAxisX;
            input_centerY = zoomTargetFractalY - zoomTargetNormY / zoomByAxisY;

            wishCenterX = input_centerX;
            wishCenterY = input_centerY;
        }
    } else {
        input_zoom = wishZoom;
        isZooming = 0;
    }

    // Интерполяция центра (только если не идёт зум к точке)
    if (!isZooming && (fabs(wishCenterX - input_centerX) > CENTER_THRESHOLD ||
                       fabs(wishCenterY - input_centerY) > CENTER_THRESHOLD)) {
        double deltaX = wishCenterX - input_centerX;
        double deltaY = wishCenterY - input_centerY;
        input_centerX += deltaX * CENTER_SPEED * time_delta;
        input_centerY += deltaY * CENTER_SPEED * time_delta;
    } else if (!isZooming) {
        input_centerX = wishCenterX;
        input_centerY = wishCenterY;
    }
}

// Callback для колесика мыши (масштабирование)
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // Норализуем координаты до [-1..1], как в шейдере
    zoomTargetNormX = (2.0 * mouseX / width) - 1.0;
    zoomTargetNormY = 1.0 - (2.0 * mouseY / height);

    double zoomByAxisX = input_zoom;
    double zoomByAxisY = input_zoom * input_aspectRatio;

    zoomTargetFractalX = input_centerX + zoomTargetNormX / zoomByAxisX;
    zoomTargetFractalY = input_centerY + zoomTargetNormY / zoomByAxisY;

    if (yoffset > 0) {
        wishZoom *= 1.2;
    } else {
        wishZoom /= 1.2;
    }

    isZooming = 1;
}

// Callback для кнопок мыши
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            isDragging = 1;
            glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
        } else if (action == GLFW_RELEASE) {
            isDragging = 0;
        }
    }
}

// Callback для движения мыши
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (isDragging) {
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        double dx = (xpos - lastMouseX) / (double) width * 2.0;
        double dy = (ypos - lastMouseY) / (double) height * 2.0;

        wishCenterX -= dx / input_zoom;
        wishCenterY += dy / (input_zoom * input_aspectRatio);

        lastMouseX = xpos;
        lastMouseY = ypos;

        isZooming = 0;
    }
}

// Callback для клавиатуры
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_EQUAL || key == GLFW_KEY_KP_ADD) {
            input_maxIterations += 50;
            printf("Max iterations: %u\n", input_maxIterations);
        }
        if (key == GLFW_KEY_MINUS || key == GLFW_KEY_KP_SUBTRACT) {
            input_maxIterations = (input_maxIterations > 50) ? input_maxIterations - 50 : 50;
            printf("Max iterations: %u\n", input_maxIterations);
        }
        if (key == GLFW_KEY_R) {
            wishZoom = ZOOM_DEFAULT;
        }
        if (key == GLFW_KEY_S) {
            input_isFloatShader = 1;
        } else if (key == GLFW_KEY_D) {
            input_isFloatShader = 0;
        }

        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }
    
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_M) {
            input_isMenuShow = !input_isMenuShow;
        }
    }
    
}

// Callback для изменения фреймбуффера
void framebuffer_resize_callback(GLFWwindow* window, int width, int height) {
    input_resolutionX = width;
    input_resolutionY = height;
    input_aspectRatio = (double) width / (double) height;
}
