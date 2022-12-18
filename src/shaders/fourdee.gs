#version 450 core
layout (points) in;
layout (points, max_vertices = 3) out;

uniform mat4 xzangle;
uniform mat4 ywangle;
uniform mat3x4 proj;

float map(float value, float min1, float max1, float min2, float max2) {
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

void main() {

    vec4 vec = gl_in[0].gl_Position * .3;
    gl_PointSize = map(vec.z, -.3, .3, 1, 10);
    gl_Position = vec4(vec.x, vec.y, vec.z, vec.w);
    EmitVertex();

    gl_Position = vec * xzangle;
    gl_Position = gl_Position * ywangle;
    float dist = 3.;
    float w = map(1. / (dist - gl_Position.w), -1, 1, -.3, .3);
    mat4 proj = mat4(
        w,  0., 0., 0.,
        0., w,  0., 0.,
        0., 0., w,  0.,
        0., 0., 0., .3
    );
    vec4 p = proj * gl_Position;
    gl_Position = vec4(p.x, p.y, p.z, p.w);
    gl_PointSize = map(gl_Position.z, -.1, .1, 1, 10);
    EmitVertex();
    gl_Position = vec4(p.x, p.y, p.z, -p.w);
    EmitVertex();

    EndPrimitive();
}
