#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;

// Output fragment color
out vec4 finalColor;

void main() {
    finalColor = vec4(0.0, fragPosition.z/4, 0.0, 1);
}