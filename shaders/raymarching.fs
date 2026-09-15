#version 330

in vec3 fragPosition;

uniform float radius;
uniform vec3 viewPos;
uniform vec3 spherePos;

out vec4 finalColor;

void main() {
    finalColor = vec4(1.0, 0.0, 1.0, 1.0);
}