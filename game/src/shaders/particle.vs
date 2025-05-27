#version 330

layout(location = 0) in vec2 vertexPos;            // Base triangle vertex
layout(location = 1) in vec2 instancePos;          // Instance position
layout(location = 2) in float instanceSize;        // Instance size

uniform mat4 projection;

void main() {
    vec2 scaledPos = vertexPos * instanceSize;
    vec2 finalPos = scaledPos + instancePos;
    gl_Position = projection * vec4(finalPos, 0.0, 1.0);
}