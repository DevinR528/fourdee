#pragma once

#include "glad/glad.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <optional>

class Shader {
private:
  int m_succ;
  char m_info[512];

  // the program ID
  GLuint m_id = 0;

  Shader(GLuint id) : m_id(id) { }

  static std::string open_file(const std::string& path) {
    std::ifstream shader_file;
    shader_file.open(path);
    if (shader_file.fail()) {
      std::cout << "failed to load file: " << path << "\n";
    }
    std::stringstream sss;
    sss << shader_file.rdbuf();
    shader_file.close();
    auto str = sss.str();
    return str;
  }

  void check_error(GLuint shader, std::string_view msg, std::string_view path) {
    // check for shader compile errors
    glGetShaderiv(shader, GL_COMPILE_STATUS, &m_succ);
    if (!m_succ) {
      glGetShaderInfoLog(shader, 512, NULL, m_info);
      std::cout << msg << path << "\n" << m_info << "\n";
      exit(1);
    }
  }

public:


  // constructor reads and builds the shader
  Shader(
    const std::string& vertex,
    const std::string& fragment,
    const std::string& geometry = std::string()
  ) {
    auto s = Shader::open_file(vertex);
    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    const char* p = s.c_str();
    glShaderSource(vert, 1, &p, NULL);
    glCompileShader(vert);
    check_error(vert, "VERTEX COMPILATION", vertex);

    auto ss = Shader::open_file(fragment);
    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    const char* pp = ss.c_str();
    glShaderSource(frag, 1, &pp, NULL);
    glCompileShader(frag);
    check_error(frag, "FRAGMENT COMPILATION", fragment);

    GLuint geo = 0;
    if (geometry.length() > 0) {
        auto sss = Shader::open_file(geometry);
        geo = glCreateShader(GL_GEOMETRY_SHADER);
        const char* ppp = sss.c_str();
        glShaderSource(geo, 1, &ppp, NULL);
        glCompileShader(geo);
        check_error(geo, "GEOMETRY COMPILATION", geometry);
        std::cout << "geo shader\n";
    }

    // link shaders
    GLuint shader_prog = glCreateProgram();
    glAttachShader(shader_prog, vert);
    glAttachShader(shader_prog, frag);

    if (geo != 0) glAttachShader(shader_prog, geo);

    glLinkProgram(shader_prog);
    check_error(shader_prog, "PROGRAM LINKING", "");

    glDeleteShader(vert);
    glDeleteShader(vert);

    if (geo != 0) glDeleteShader(geo);

    m_id = shader_prog;
  }

  GLuint prog_id() const & { return m_id; }

  // Activate the shader
  void activate() { glUseProgram(m_id); }

  void setMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(
        glGetUniformLocation(m_id, name.c_str()),
        1, GL_FALSE, glm::value_ptr(mat)
    );
  }

  void setFloat(const std::string &name, float fl) const {
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), fl);
  }

  ~Shader() {
    if (m_id != 0) glDeleteShader(m_id);
  }
};
