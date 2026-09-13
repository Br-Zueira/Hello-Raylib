#version 330

// Input vertex attributes
in vec3 vertexPosition;

// Input uniform values
uniform mat4 mvp;

// Output vertex attributes (to fragment shader)
out vec3 fragPosition;

void main()
{
    // Just passes relative coordinates straight to Fragment Shader
    fragPosition = vertexPosition;

    // Calculate final vertex position
    gl_Position = mvp*vec4(vertexPosition, 1.0);
}
