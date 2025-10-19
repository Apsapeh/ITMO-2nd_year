#pragma once

#include <GLFW/glfw3.h>

// Глобальные переменные для управления камерой
extern double input_centerX;
extern double input_centerY;
extern double input_zoom;
extern unsigned int input_maxIterations;

// Глобальные переменные для фреймбуффера
extern int input_resolutionX;
extern int input_resolutionY;
extern double input_aspectRatio;

// Интерполяция для сглаженных анимаций (вызывать в каждом кадре)
void inputInterpolate(double time_delta);

// Callback для колесика мыши (масштабирование)
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Callback для кнопок мыши
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
// Callback для движения мыши
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

// Callback для клавиатуры
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Callback для изменения фреймбуффера
void framebuffer_resize_callback(GLFWwindow* window, int width, int height);
