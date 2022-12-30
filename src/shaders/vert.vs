#version 450 core

layout (location = 0) in vec4 aPos;

layout (location = 1) in vec3 acolor;

out VS_OUT {
    vec3 color;
} vs_out;

void main()
{
    gl_Position = aPos;
    
    vs_out.color = acolor;
}
