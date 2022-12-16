#version 450 core
layout (points) in;
layout (points, max_vertices = 2) out;

uniform mat4 xyangle;
uniform mat4 zwangle;
uniform mat3x4 proj;

void main() {
    vec4 vec = gl_in[0].gl_Position * .3;
    gl_Position = vec4(vec.x, vec.y, vec.z, 5.);
    gl_PointSize = 5. + (gl_Position.z * 10.);


    EmitVertex();

    gl_Position = vec * xyangle;
    gl_Position = gl_Position * zwangle;

    float dist = 2.;
    float w = 1. / (dist - gl_Position.w);
    mat4 proj = mat4(
        w,  0., 0., 0.,
        0., w,  0., 0.,
        0., 0., w,  0.,
        0., 0., 0., 0.
    );
    vec4 p = proj * gl_Position;
    gl_Position = vec4(p.x, p.y, p.z, 5.);
    gl_PointSize = 5. + (gl_Position.z * 10.);
    EmitVertex();

    EndPrimitive();
}
