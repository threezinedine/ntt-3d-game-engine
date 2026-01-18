#version 450

struct Vertex
{
    float x, y;
};

layout (binding = 0) readonly buffer VertexData
{
    Vertex vertices[];
} vertices;

void main() {
#if 1
    Vertex vertex = vertices.vertices[gl_VertexIndex];
    gl_Position = vec4(vertex.x, vertex.y, 0.0, 1.0);
#else
    vec2 hardcoded[3] = vec2[](
        vec2(-0.6, -0.6),
        vec2( 0.6, -0.6),
        vec2( 0.0,  0.6)
    );

    int index = 1;

    Vertex v = vertices.vertices[index];  // just read the very first vertex

    if (gl_VertexIndex == index)
    {
        if (abs(v.x - 0.5) < 0.001 && abs(v.y + 0.5) < 0.001) {
            gl_Position = vec4(-1.0, 0.8, 0.0, 1.0);
        } else {
            gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
        }
    }
    else 
    {
        gl_Position = vec4(hardcoded[gl_VertexIndex], 0.0, 1.0);
    }
#endif
}   