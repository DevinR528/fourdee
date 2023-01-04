

#include "glad/glad.h"
#include "glm/glm.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <map>
#include <string>

#include "shader.hpp"
#include "text.hpp"

int init_fonts(std::map<u_char, Character> &char_map) {
  FT_Library ft;
  if (FT_Init_FreeType(&ft)) {
    std::cout << "Error: Could not init FreeType Library\n";
    return -1;
  }

  FT_Face face;
  if (FT_New_Face(ft, "fonts/Hack-Regular.ttf", 0, &face)) {
    std::cout << "Error: Failed to load font" << std::endl;
    return -1;
  }

  // 0 lets us dynamically calc width based on height, does the 3rd param cap us
  // at 48???
  FT_Set_Pixel_Sizes(face, 0, 48);

  // Disable byte-alignment restriction (I don't know what that means???)
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  for (u_char ch = 1; ch < 128; ch++) {
    if (FT_Load_Char(face, ch, FT_LOAD_RENDER)) {
      std::cout << "Could not load '" << ch << "'\n";
      continue;
    }

    uint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
                 face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                 face->glyph->bitmap.buffer);

    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // now store character for later use
    Character chara = {
        .texture_id = texture,
        .size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        .bearing =
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        .advance = face->glyph->advance.x};
    char_map.insert(std::pair<u_char, Character>(ch, chara));
  }

  // Free all the FreeType stuff now that we got our textures
  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  return 0;
}

// clang-format off
void render_text(
  std::map<u_char, Character> &char_map,
  Shader &shader, GLuint tao, GLuint tbo,
  std::string text,
  float height, float width,
  float x, float y, float scale,
  glm::vec3 color
) {
  shader.activate();
  shader.setMat4("projection", glm::ortho(0.0f, height, 0.0f, width));
  glUniform3f(
    glGetUniformLocation(shader.prog_id(), "text_color"),
    color.x, color.y, color.z
  );
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(tao);

  for (auto &&c : text) {
    Character ch = char_map[c];

    float xpos = x + ch.bearing.x * scale;
    float ypos = y - (ch.size.y - ch.bearing.y) * scale;

    float w = ch.size.x * scale;
    float h = ch.size.y * scale;
    // update VBO for each character
    float vertices[6][4] = {
      { xpos,     ypos + h,   0.0f, 0.0f },
      { xpos,     ypos,       0.0f, 1.0f },
      { xpos + w, ypos,       1.0f, 1.0f },

      { xpos,     ypos + h,   0.0f, 0.0f },
      { xpos + w, ypos,       1.0f, 1.0f },
      { xpos + w, ypos + h,   1.0f, 0.0f }
    };
    // render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, ch.texture_id);
    // update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
    // bitshift by 6 to get value in pixels (2^6 = 64)
    x += (ch.advance >> 6) * scale;
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
// clang-format on
