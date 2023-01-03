#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"

#include <map>
#include <string>

#include "shader.hpp"

// A pre-rendered character of fixed height and width (I think..)
struct Character {
  // ID handle of the glyph texture
  unsigned int texture_id;
  // Size of glyph
  glm::ivec2 size;
  // Offset from baseline to left/top of glyph
  glm::ivec2 bearing;
  // Offset to advance to next glyph
  signed long advance;
};

int init_fonts(std::map<u_char, Character> &char_map);

void render_text(
    std::map<u_char, Character> &char_map,
    Shader &shader, GLuint tao, GLuint tbo,
    std::string text,
    float height, float width,
    float x, float y, float scale,
    glm::vec3 color
);
