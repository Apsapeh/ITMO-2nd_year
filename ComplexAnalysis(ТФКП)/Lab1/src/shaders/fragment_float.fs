#version 400 core

out vec4 fragColor;
in vec2 fragCoord; // x, y ∈ [-1..1]

uniform vec2 u_Center;
uniform vec2 u_ZoomByAxis; // (zoom, zoom * aspect_ratio)
uniform uint u_MaxIterations;

vec3 palette(float t) {
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.0, 1.0, 1.0);
    vec3 d = vec3(0.0, 0.1, 0.2);
    return a + b * cos(6.28318 * (c * t + d));
}

void main() {
    // Создание c, где действительная часть это X, а мнимая это Y
    vec2 c = fragCoord / u_ZoomByAxis + u_Center;
    
    vec2 z = vec2(0.0f);
    uint i = 0;
    float x2 = 0.0, y2 = 0.0;
    for (; i < u_MaxIterations; ++i) {
        // Кэширование для квадратов, т.к. используется при проверке и вычислении
        x2 = z.x * z.x;
        y2 = z.y * z.y;
        
        // Более производительная альтернатива |z| > 2
        // |z| = sqrt(z.x * z.x + z.y * z.y)
        // sqrt(z.x * z.x + z.y * z.y) > 2
        // z.x * z.x + z.y * z.y > 4, взятие корня очень тяжёлая операция
        if (x2 + y2 > 4.0) break;
        
        z = vec2(x2 - y2, 2.0 * z.x * z.y) + c;
    }
    
    if (i == u_MaxIterations) {
        fragColor = vec4(0.0, 0.0, 0.0, 1.0);
    } else {
        float zlen = sqrt(z.x * z.x + z.y * z.y);
        float smoothed = float(i) + 1.0 - log(log(float(zlen))) / log(2.0);
        float t = smoothed / float(u_MaxIterations);
        
        vec3 color = palette(t);
        fragColor = vec4(color, 1.0);
    }
}
