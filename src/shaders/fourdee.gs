
#version 450 core
layout(points) in;
layout(points, max_vertices = 7) out;

uniform mat4 cube;
uniform mat4 MVP;

in VS_OUT { vec3 color; }
gs_in[];

out vec4 fColor;

float map(float value, float min1, float max1, float min2, float max2) {
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

void main() {

  gl_PointSize = 2;

  vec4 a = gl_in[0].gl_Position;
  vec4 b = cube * a;
  vec4 c = mat4(1. / (2.0 - b.w)) * b;
  vec4 d = vec4(c.x, c.y, c.z, 1);

  gl_Position = MVP * d;

  fColor = vec4(gs_in[0].color, 1);

  EmitVertex();
  EndPrimitive();
}
