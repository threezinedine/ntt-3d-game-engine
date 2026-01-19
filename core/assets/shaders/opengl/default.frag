#version 460

layout (binding = 1) uniform UniformBlock1
{
    vec4 color;
};

layout (location = 0) out vec4 FragColor;

void main()
{
    FragColor = color;
}