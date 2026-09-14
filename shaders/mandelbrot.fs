#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;

// Output fragment color
out vec4 finalColor;

#define MAX_ITERATIONS 50
#define PLANE_WIDTH     6
#define PLANE_HEIGHT    4

void main() {
    vec2 c = vec2(fragPosition.x, fragPosition.z);
    vec2 z = vec2(0.0, 0.0);
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        z = vec2(z.x*z.x - z.y*z.y, 2.0*z.x*z.y) + c; // Zn+1 = Zn² + c, Mandelbrot Formula
        if (dot(z, z) > 4.0) {                        // If distance to 0 squared is bigger 4
            finalColor = vec4(0.0, 0.0, 1.0, 1.0);
            return;
        }
    }
    finalColor = vec4(0.0, 0.0, 0.0, 1.0);            // Color of the fractal itself
}