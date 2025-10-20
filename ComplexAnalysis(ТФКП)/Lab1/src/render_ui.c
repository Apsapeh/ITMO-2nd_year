#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "render.h"
#include "render_ui.h"
#include "input.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT

#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#include <nuklear.h>

const char uiVertexShaderSource[] = {
#include <ui_vertex.vs.h>
};

const char uiFragmentShaderSource[] = {
#include <ui_fragment.fs.h>
};

static void device_upload_atlas(const void* image, int width, int height);
static void draw(int width, int height);
static void pump_input(struct nk_context* ctx, GLFWwindow* win);

#define MAX_MEMORY 512 * 1024
// #define MAX_ELEMENT_MEMORY 128 * 1024

struct nk_context ctx;
struct nk_font_atlas atlas;

unsigned int font_tex = 0;
struct nk_draw_null_texture tex_null;

static unsigned int uiShaderProgram;
static unsigned int uiVAO, uiVBO, uiEBO;
static unsigned int uProjectionLoc;

struct your_vertex {
    float pos[2];
    float uv[2];
    unsigned char col[4];
};

void initNuklearUI(void) {
    const void* image;
    int w, h;
    struct nk_font* font;

    struct nk_font_config config = nk_font_config(18);
    config.oversample_h = 1;
    config.oversample_v = 1;
    config.range = nk_font_cyrillic_glyph_ranges();

    nk_font_atlas_init_default(&atlas);
    nk_font_atlas_begin(&atlas);
    font = nk_font_atlas_add_default(&atlas, 18, &config);
    // font = nk_font_atlas_add_from_file(&atlas, "/Users/ghost/Desktop/C++
    // Projects/Projects/Nuklear_GLFW3_GL1_Driver/OpenSans-Regular.ttf", 18,
    // &config);

    /*font = nk_font_atlas_add_from_file(
        &atlas, "examples/Assets/Comfortaa-Regular.ttf", 18, &config
    );*/
    image = nk_font_atlas_bake(&atlas, &w, &h, NK_FONT_ATLAS_RGBA32);
    device_upload_atlas(image, w, h);
    nk_font_atlas_end(&atlas, nk_handle_id((int) font_tex), &tex_null);
    nk_init_default(&ctx, &font->handle);

    // Инициализация UI шейдеров
    unsigned int uiVertexShader = compileShader(GL_VERTEX_SHADER, uiVertexShaderSource);
    unsigned int uiFragmentShader = compileShader(GL_FRAGMENT_SHADER, uiFragmentShaderSource);

    uiShaderProgram = glCreateProgram();
    glAttachShader(uiShaderProgram, uiVertexShader);
    glAttachShader(uiShaderProgram, uiFragmentShader);
    glLinkProgram(uiShaderProgram);

    glDeleteShader(uiVertexShader);
    glDeleteShader(uiFragmentShader);

    uProjectionLoc = glGetUniformLocation(uiShaderProgram, "uProjection");

    // Создание VAO
    glGenVertexArrays(1, &uiVAO);
    glGenBuffers(1, &uiVBO);
    glGenBuffers(1, &uiEBO);

    glBindVertexArray(uiVAO);

    glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiEBO);

    // Позиция
    glVertexAttribPointer(
            0, 2, GL_FLOAT, GL_FALSE, sizeof(struct your_vertex),
            (void*) offsetof(struct your_vertex, pos)
    );
    glEnableVertexAttribArray(0);

    // UV
    glVertexAttribPointer(
            1, 2, GL_FLOAT, GL_FALSE, sizeof(struct your_vertex),
            (void*) offsetof(struct your_vertex, uv)
    );
    glEnableVertexAttribArray(1);

    // Цвет
    glVertexAttribPointer(
            2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(struct your_vertex),
            (void*) offsetof(struct your_vertex, col)
    );
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

static void device_upload_atlas(const void* image, int width, int height) {
    glGenTextures(1, &font_tex);
    glBindTexture(GL_TEXTURE_2D, font_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei) width, (GLsizei) height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
            image
    );
}

void drawNuklearUI(GLFWwindow* window, double delta) {
    pump_input(&ctx, window);


    if (nk_begin(&ctx, "Mandelbrot Controls", nk_rect(10, 10, 350, 320), 0)) {

        // Отображение информации
        nk_layout_row_dynamic(&ctx, 25, 1);
        nk_label(&ctx, "=== Information ===", NK_TEXT_CENTERED);

        nk_layout_row_dynamic(&ctx, 20, 1);
        char buffer[256];

        snprintf(buffer, sizeof(buffer), "Center X: %.15f", input_centerX);
        nk_label(&ctx, buffer, NK_TEXT_LEFT);

        snprintf(buffer, sizeof(buffer), "Center Y: %.15f", input_centerY);
        nk_label(&ctx, buffer, NK_TEXT_LEFT);

        snprintf(buffer, sizeof(buffer), "Zoom: %.15f", input_zoom);
        nk_label(&ctx, buffer, NK_TEXT_LEFT);

        snprintf(buffer, sizeof(buffer), "Max Iterations: %u", input_maxIterations);
        nk_label(&ctx, buffer, NK_TEXT_LEFT);

        snprintf(buffer, sizeof(buffer), "Shader: %s", input_isFloatShader ? "Float" : "Double");
        nk_label(&ctx, buffer, NK_TEXT_LEFT);
        
        snprintf(buffer, sizeof(buffer), "Frametime: %.3fms (%d FPS)", delta * 1000.0, (int)(1.0 / delta));
        nk_label(&ctx, buffer, NK_TEXT_LEFT);

        // Разделитель
        nk_layout_row_dynamic(&ctx, 10, 1);
        nk_spacing(&ctx, 1);

        // Управление итерациями
        nk_layout_row_dynamic(&ctx, 25, 1);
        nk_label(&ctx, "Max Iterations:", NK_TEXT_LEFT);

        nk_layout_row_dynamic(&ctx, 25, 1);
        nk_property_int(&ctx, "#Iterations", 10, (int*) &input_maxIterations, 10000, 10, 1);

        // Переключатель шейдера
        nk_layout_row_dynamic(&ctx, 25, 1);
        if (nk_checkbox_label(&ctx, "Use Float Shader", (nk_bool*) &input_isFloatShader)) {
        }
    }
    nk_end(&ctx);

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    draw(width, height);
}

static void draw(int width, int height) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glDisable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height);

    glUseProgram(uiShaderProgram);

    // Создаём ортографическую проекцию
    float projection[16] = {
            2.0f / (float) width,
            0.0f,
            0.0f,
            0.0f,
            0.0f,
            2.0f / (float) -height,
            0.0f,
            0.0f,
            0.0f,
            0.0f,
            -2.0f,
            0.0f,
            -1.0f,
            1.0f,
            1.0f,
            1.0f
    };
    glUniformMatrix4fv(uProjectionLoc, 1, GL_FALSE, projection);

    // Конвертируем Nuklear команды
    struct nk_convert_config cfg = {0};
    static const struct nk_draw_vertex_layout_element vertex_layout[] = {
            {NK_VERTEX_POSITION, NK_FORMAT_FLOAT, NK_OFFSETOF(struct your_vertex, pos)},
            {NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT, NK_OFFSETOF(struct your_vertex, uv)},
            {NK_VERTEX_COLOR, NK_FORMAT_R8G8B8A8, NK_OFFSETOF(struct your_vertex, col)},
            {NK_VERTEX_LAYOUT_END}
    };
    cfg.shape_AA = NK_ANTI_ALIASING_ON;
    cfg.line_AA = NK_ANTI_ALIASING_ON;
    cfg.vertex_layout = vertex_layout;
    cfg.vertex_size = sizeof(struct your_vertex);
    cfg.vertex_alignment = NK_ALIGNOF(struct your_vertex);
    cfg.circle_segment_count = 22;
    cfg.curve_segment_count = 22;
    cfg.arc_segment_count = 22;
    cfg.global_alpha = 1.0f;
    cfg.tex_null = tex_null;

    struct nk_buffer cmds, verts, idx;
    nk_buffer_init_default(&cmds);
    nk_buffer_init_default(&verts);
    nk_buffer_init_default(&idx);
    nk_convert(&ctx, &cmds, &verts, &idx, &cfg);

    // Загружаем данные в буферы
    glBindVertexArray(uiVAO);

    glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
    glBufferData(
            GL_ARRAY_BUFFER, nk_buffer_total(&verts), nk_buffer_memory_const(&verts), GL_STREAM_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiEBO);
    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER, nk_buffer_total(&idx), nk_buffer_memory_const(&idx), GL_STREAM_DRAW
    );

    // Рисуем команды
    const struct nk_draw_command* cmd;
    const nk_draw_index* offset = NULL;

    nk_draw_foreach(cmd, &ctx, &cmds) {
        if (!cmd->elem_count)
            continue;

        glBindTexture(GL_TEXTURE_2D, (GLuint) cmd->texture.id);
        glScissor(
                (GLint) cmd->clip_rect.x,
                (GLint) (height - (GLint) (cmd->clip_rect.y + cmd->clip_rect.h)),
                (GLint) cmd->clip_rect.w, (GLint) cmd->clip_rect.h
        );
        glEnable(GL_SCISSOR_TEST);

        glDrawElements(GL_TRIANGLES, (GLsizei) cmd->elem_count, GL_UNSIGNED_SHORT, offset);
        offset += cmd->elem_count;
    }

    glDisable(GL_SCISSOR_TEST);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);

    nk_buffer_free(&cmds);
    nk_buffer_free(&verts);
    nk_buffer_free(&idx);
    nk_clear(&ctx);

    glDisable(GL_BLEND);
}

static void pump_input(struct nk_context* ctx, GLFWwindow* win) {
    double x, y;
    nk_input_begin(ctx);
    // glfwPollEvents();

    nk_input_key(ctx, NK_KEY_DEL, glfwGetKey(win, GLFW_KEY_DELETE) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_ENTER, glfwGetKey(win, GLFW_KEY_ENTER) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_TAB, glfwGetKey(win, GLFW_KEY_TAB) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_BACKSPACE, glfwGetKey(win, GLFW_KEY_BACKSPACE) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_LEFT, glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_RIGHT, glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_UP, glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_DOWN, glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS);

    if (glfwGetKey(win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
        glfwGetKey(win, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
        nk_input_key(ctx, NK_KEY_COPY, glfwGetKey(win, GLFW_KEY_C) == GLFW_PRESS);
        nk_input_key(ctx, NK_KEY_PASTE, glfwGetKey(win, GLFW_KEY_P) == GLFW_PRESS);
        nk_input_key(ctx, NK_KEY_CUT, glfwGetKey(win, GLFW_KEY_X) == GLFW_PRESS);
        nk_input_key(ctx, NK_KEY_CUT, glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS);
        nk_input_key(ctx, NK_KEY_SHIFT, 1);
    } else {
        nk_input_key(ctx, NK_KEY_COPY, 0);
        nk_input_key(ctx, NK_KEY_PASTE, 0);
        nk_input_key(ctx, NK_KEY_CUT, 0);
        nk_input_key(ctx, NK_KEY_SHIFT, 0);
    }

    glfwGetCursorPos(win, &x, &y);
    nk_input_motion(ctx, (int) x, (int) y);
    nk_input_button(
            ctx, NK_BUTTON_LEFT, (int) x, (int) y,
            glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS
    );
    nk_input_button(
            ctx, NK_BUTTON_MIDDLE, (int) x, (int) y,
            glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS
    );
    nk_input_button(
            ctx, NK_BUTTON_RIGHT, (int) x, (int) y,
            glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS
    );
    nk_input_end(ctx);
}
