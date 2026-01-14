#version 450

// Input: vertex index (used to index into positions array)
layout(location = 0) in uint vertexIndex;

// Output: position in clip coordinates
layout(location = 0) out vec4 outPosition;

// Hardcoded vertex positions (normalized device coordinates)
const vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
);

void main() {
    // Output the vertex position as clip coordinates (w = 1.0)
    outPosition = vec4(positions[vertexIndex], 0.0, 1.0);
}   