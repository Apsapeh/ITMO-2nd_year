#pragma once

int renderInit(void);

void renderExit(void);

void renderDraw(void);

// Компиляция шейдера
unsigned int compileShader(unsigned int type, const char* source);
