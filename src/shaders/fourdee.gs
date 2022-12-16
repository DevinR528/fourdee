#version 450 core
layout (points) in;
layout (points, max_vertices = 2) out;

uniform mat4 xyangle;
uniform mat4 zwangle;

void main() {
    gl_PointSize = 2. + (10. * gl_in[0].gl_Position.z);
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    gl_PointSize = 2. + (10. * gl_in[0].gl_Position.z);
    gl_Position = gl_in[0].gl_Position * xyangle;
    EmitVertex();

    gl_PointSize = 2. + (10. * gl_in[0].gl_Position.z);
    gl_Position = gl_in[0].gl_Position * zwangle;
    EmitVertex();

    EndPrimitive();
}
