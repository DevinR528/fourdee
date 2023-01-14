#pragma once

struct a {
  float x, y, z, w, r, g, b;
};

float map(float x, float x1, float x2, float y1, float y2) {
  return ((x - x1) / (x2 - x1)) * (y2 - y1) + y1;
}

unsigned push_points(unsigned divisions, struct a *vertices) {
  unsigned n = 0;

//   for (unsigned i = 0; i < divisions; i++) {
//     for (unsigned j = 0; j < divisions; j++) {
//       vertices[n++] = (struct a){
//           .x = map(i, 0, divisions, +1, -1),
//           .y = +1,
//           .z = +1,
//           .w = map(j, 0, divisions, +1, -1),
//           .r = 0.7f,
//           .g = 0.1f,
//           .b = 0.6f,
//       };
//     }
//   }
//   for (unsigned i = 0; i < divisions; i++) {
//     for (unsigned j = 0; j < divisions; j++) {
//       vertices[n++] = (struct a){
//           .x = -1,
//           .y = +1,
//           .z = map(i, 0, divisions, +1, -1),
//           .w = map(j, 0, divisions, +1, -1),
//           .r = 0.7f,
//           .g = 0.2f,
//           .b = 0.4f,
//       };
//     }
//   }
//   for (unsigned i = 0; i < divisions; i++) {
//     for (unsigned j = 0; j < divisions; j++) {
//       vertices[n++] = (struct a){
//           .x = map(i, 0, divisions, +1, -1),
//           .y = +1,
//           .z = -1,
//           .w = map(j, 0, divisions, +1, -1),
//           .r = 0.3f,
//           .g = 0.5f,
//           .b = 0.4f,
//       };
//     }
//   }
//   for (unsigned i = 0; i < divisions; i++) {
//     for (unsigned j = 0; j < divisions; j++) {
//       vertices[n++] = (struct a){
//           .x = +1,
//           .y = +1,
//           .z = map(i, 0, divisions, -1, +1),
//           .w = map(j, 0, divisions, +1, -1),
//           .r = 0.8f,
//           .g = 0.8f,
//           .b = 0.4f,
//       };
//     }
//   }

  for (unsigned i = 0; i < divisions; i++) {
    for (unsigned j = 0; j < divisions; j++) {
      vertices[n++] = (struct a){
          .x = map(i, 0, divisions, +1, -1),
          .y = +1,
          .z = map(j, 0, divisions, +1, -1),
          .w = 0,
          .r = 0.9f,
          .g = 0.9f,
          .b = 0.9f,
      };
    }
  }

  for (unsigned i = 0; i < divisions; i++) {
    for (unsigned j = 0; j < divisions; j++) {
      vertices[n++] = (struct a){
          .x = map(i, 0, divisions, +1, -1),
          .y = -1,
          .z = map(j, 0, divisions, +1, -1),
          .w = 0,
          .r = 0.2f,
          .g = 0.2f,
          .b = 0.2f,
      };
    }
  }

  for (unsigned i = 0; i < divisions; i++) {
    for (unsigned j = 0; j < divisions; j++) {
      vertices[n++] = (struct a){
          .x = map(i, 0, divisions, +1, -1),
          .y = map(j, 0, divisions, +1, -1),
          .z = +1,
          .w = 0,
          .r = 0.5f,
          .g = 0.8f,
          .b = 0.4f,
      };
    }
  }
  for (unsigned i = 0; i < divisions; i++) {
    for (unsigned j = 0; j < divisions; j++) {
      vertices[n++] = (struct a){
          .x = -1,
          .y = map(j, 0, divisions, +1, -1),
          .z = map(i, 0, divisions, +1, -1),
          .w = 0,
          .r = 0.9f,
          .g = 0.8f,
          .b = 0.4f,
      };
    }
  }
  for (unsigned i = 0; i < divisions; i++) {
    for (unsigned j = 0; j < divisions; j++) {
      vertices[n++] = (struct a){
          .x = map(i, 0, divisions, -1, +1),
          .y = map(j, 0, divisions, +1, -1),
          .z = -1,
          .w = 0,
          .r = 0.1f,
          .g = 0.4f,
          .b = 0.4f,
      };
    }
  }
  for (unsigned i = 0; i < divisions; i++) {
    for (unsigned j = 0; j < divisions; j++) {
      vertices[n++] = (struct a){
          .x = +1,
          .y = map(j, 0, divisions, +1, -1),
          .z = map(i, 0, divisions, -1, +1),
          .w = 0,
          .r = 0.1f,
          .g = 0.4f,
          .b = 0.8f,
      };
    }
  }

//   for (unsigned i = 0; i < divisions; i++) {
//     for (unsigned j = 0; j < divisions; j++) {
//       vertices[n++] = (struct a){
//           .x = map(i, 0, divisions, +1, -1),
//           .y = -1,
//           .z = +1,
//           .w = map(j, 0, divisions, -1, +1),
//           .r = 0.1f,
//           .g = 0.4f,
//           .b = 0.8f,
//       };
//     }
//   }
//   for (unsigned i = 0; i < divisions; i++) {
//     for (unsigned j = 0; j < divisions; j++) {
//       vertices[n++] = (struct a){
//           .x = -1,
//           .y = -1,
//           .z = map(i, 0, divisions, +1, -1),
//           .w = map(j, 0, divisions, -1, +1),
//           .r = 0.6f,
//           .g = 0.4f,
//           .b = 0.7f,
//       };
//     }
//   }
//   for (unsigned i = 0; i < divisions; i++) {
//     for (unsigned j = 0; j < divisions; j++) {
//       vertices[n++] = (struct a){
//           .x = map(i, 0, divisions, -1, +1),
//           .y = -1,
//           .z = -1,
//           .w = map(j, 0, divisions, -1, +1),
//           .r = 0.9f,
//           .g = 0.2f,
//           .b = 0.3f,
//       };
//     }
//   }
//   for (unsigned i = 0; i < divisions; i++) {
//     for (unsigned j = 0; j < divisions; j++) {
//       vertices[n++] = (struct a){
//           .x = +1,
//           .y = -1,
//           .z = map(i, 0, divisions, -1, +1),
//           .w = map(j, 0, divisions, -1, +1),
//           .r = 0.5f,
//           .g = 0.5f,
//           .b = 0.8f,
//       };
//     }
//   }

//   for (unsigned i = 0; i < divisions; i++) {
//     for (unsigned j = 0; j < divisions; j++) {
//       vertices[n++] = (struct a){
//           .x = map(i, 0, divisions, +1, -1),
//           .y = map(j, 0, divisions, -1, +1),
//           .z = +1,
//           .w = +1,
//           .r = 0.1f,
//           .g = 0.5f,
//           .b = 0.8f,
//       };
//     }
//   }
//   for (unsigned i = 0; i < divisions; i++) {
//     for (unsigned j = 0; j < divisions; j++) {
//       vertices[n++] = (struct a){
//           .x = -1,
//           .y = map(j, 0, divisions, -1, +1),
//           .z = map(i, 0, divisions, +1, -1),
//           .w = +1,
//           .r = 0.8f,
//           .g = 0.2f,
//           .b = 0.8f,
//       };
//     }
//   }
//   for (unsigned i = 0; i < divisions; i++) {
//     for (unsigned j = 0; j < divisions; j++) {
//       vertices[n++] = (struct a){
//           .x = map(i, 0, divisions, -1, +1),
//           .y = map(j, 0, divisions, -1, +1),
//           .z = -1,
//           .w = +1,
//           .r = 0.8f,
//           .g = 0.7f,
//           .b = 0.8f,
//       };
//     }
//   }
//   for (unsigned i = 0; i < divisions; i++) {
//     for (unsigned j = 0; j < divisions; j++) {
//       vertices[n++] = (struct a){
//           .x = +1,
//           .y = map(j, 0, divisions, -1, +1),
//           .z = map(i, 0, divisions, -1, +1),
//           .w = +1,
//           .r = 0.2f,
//           .g = 0.3f,
//           .b = 0.8f,
//       };
//     }
//   }
  return n;
}
