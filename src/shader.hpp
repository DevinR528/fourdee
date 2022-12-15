#pragma once

#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
private:
    int m_succ;
    char m_info[512];
    Shader(GLuint id) : m_id(id) {}
public:
  // the program ID
  GLuint m_id;

  // constructor reads and builds the shader
  Shader(const std::string &path, GLuint kind) {
    std::ifstream shader_file;
    shader_file.open(path);
    if (shader_file.fail()) {
      std::cout << "failed to load file: " << path << "\n";
    }
    std::stringstream sss; sss << shader_file.rdbuf();
    shader_file.close();
    auto shader_src = sss.str().c_str();

    GLuint shader = glCreateShader(kind);
    glShaderSource(shader, 1, &shader_src, NULL);
    glCompileShader(shader);

    // check for shader compile errors
    glGetShaderiv(shader, GL_COMPILE_STATUS, &m_succ);
    if (!m_succ) {
      glGetShaderInfoLog(shader, 512, NULL, m_info);
      std::cout << "COMPILATION_FAILED for: " << path << "\n"
                << m_info << "\n";
    }

    m_id = glCreateProgram();
    glAttachShader(m_id, shader);
    glLinkProgram(m_id);
    // print linking errors if any
    glGetProgramiv(m_id, GL_LINK_STATUS, &m_succ);
    if (!m_succ) {
      glGetProgramInfoLog(m_id, 512, NULL, m_info);
      std::cout << "LINKING_FAILED for: " << path << "\n"
                << m_info << "\n";
    }

    glDeleteShader(shader);
  }

  GLuint id() const& { return m_id; }

  Shader link(GLuint other_id) {
    // link shaders
    GLuint shader_prog = glCreateProgram();
    glAttachShader(shader_prog, m_id);
    glAttachShader(shader_prog, other_id);
    glLinkProgram(shader_prog);
    // check for linking errors
    glGetProgramiv(shader_prog, GL_LINK_STATUS, &m_succ);
    if (!m_succ) {
        glGetProgramInfoLog(shader_prog, 512, NULL, m_info);
        std::cout << "PROGRAM_LINKING_FAILED\n"
                << m_info << "\n";
        return 0;
    }
    return Shader(shader_prog);
  }

  // Activate the shader
  void activate() { glUseProgram(m_id); }

  ~Shader() {
    glDeleteShader(m_id);
  }
};
