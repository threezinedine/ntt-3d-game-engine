#version 450 

layout(set = 0, binding = 1) uniform UniformBlock1
{
    vec4 color;
};

layout(location = 0) out vec4 fragColor;

void main()
{
    fragColor = color;
}