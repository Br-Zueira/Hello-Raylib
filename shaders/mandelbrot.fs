#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;

// Output fragment color
out vec4 finalColor;

#define PI 3.1415926535897932384626433832795
#define MAX_ITERATIONS 50

// Uniform variables
uniform vec2 offset;
uniform float zoom;

void main() {
    // Vec2 serves as some version of "complex number (x is real, y is imaginary part)"
    float cx = fragPosition.x/zoom + offset.x;
    float cy = fragPosition.z/zoom + offset.y;
    vec2 mandelC = vec2(cx, cy);
    vec2 z = vec2(0.0, 0.0);

    // The Mandelbrot set calculation
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        // Zn+1 = Zn² + c, Mandelbrot Set Formula
        z = vec2(z.x*z.x - z.y*z.y, 2.0*z.x*z.y) + mandelC;
        
        // If distance to 0 squared is bigger 4
        if (dot(z, z) > 4.0) {
            // Puts i into a (0.0, 1.0) range where bigger i means smaller t
            float t = (float(i) - log2(log2(dot(z, z)) * 0.5))/MAX_ITERATIONS;

            // This formula makes those gorgeous procedural color schemes
            vec3 a = vec3(0.5, 0.5, 0.5);
            vec3 b = vec3(0.5, 0.5, 0.5);
            vec3 c = vec3(1.0, 1.0, 1.0);
            vec3 d = vec3(0.0, 0.33, 0.67);
            vec3 color = a + b * cos(2 * PI * (c * t + d));

            finalColor = vec4(color, 1.0);
            return;
        }
    }
    finalColor = vec4(0.0, 0.0, 0.0, 1.0);            // Color of the fractal itself
}