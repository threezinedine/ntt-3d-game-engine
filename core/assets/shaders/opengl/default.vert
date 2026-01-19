#version 460 

struct Vertex {
    float x, y;
};

layout (binding = 0, std430) readonly buffer VertexBuffer {
    Vertex vertices[];
} vertices;

void main() {
    Vertex vertex = vertices.vertices[gl_VertexID];
    gl_Position = vec4(vertex.x, vertex.y, 0.0, 1.0);
}