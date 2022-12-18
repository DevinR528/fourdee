// These have to be included before GLFW or any other gl* libraries
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

#include "shader.hpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const GLuint SCR_WIDTH = 800;
const GLuint SCR_HEIGHT = 600;

float xrot = 0.0;
float yrot = 0.0;

float tra_x = 0.0;
float tra_y = 0.0;
float tra_z = 0.0;

int main() {
  // initialize and configure
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

  // glfw window creation
  // --------------------
  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "ForDee", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  printf("Renderer: %s\n", glGetString(GL_RENDERER));
  printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

  Shader shader_prog = Shader("src/shaders/vert.vs", "src/shaders/frag.fs", "src/shaders/fourdee.gs");

//   GLfloat vertices[] = {
//     //     A              B
//     -1., -1.,  1.,    1., -1.,  1.,
//     //     B              C
//      1., -1.,  1.,    1.,  1.,  1.,
//     //     C              D
//      1.,  1.,  1.,   -1.,  1.,  1.,
//     //     D              A
//      -1.,  1.,  1.,  -1., -1.,  1.,
//     //     A              F
//     -1., -1.,  1.,   -1., -1., -1.,
//     //     B              G
//      1., -1.,  1.,    1., -1., -1.,
//     //     C              H
//      1.,  1.,  1.,    1.,  1., -1.,
//     //     D              E
//      -1.,  1.,  1.,  -1.,  1., -1.,
//     //     E              F
//     -1.,  1., -1.,   -1., -1., -1.,
//     //     F              G
//     -1., -1., -1.,    1., -1., -1.,
//     //     G              H
//      1., -1., -1.,    1.,  1., -1.,
//      //    H              E
//      1.,  1., -1.,    -1.,  1., -1.,
//   };

//   GLfloat vertices[] = {
//      -1., -1.,  1.,  1.,    1., -1.,  1.,  1.,    1.,  1.,  1.,  1.,    -1.,  1.,  1.,  1.,
//      -1., -1., -1.,  1.,    1., -1., -1.,  1.,    1.,  1., -1.,  1.,    -1.,  1., -1.,  1.,

//      -1.,  1., -1., -1,   -1., -1., -1., -1.,     1., -1., -1., -1.,     1.,  1., -1., -1.,
//   };

  GLfloat vertices[] = {
      1.,  1.,  1.,  1.,    1., -1.,  1.,  1.,   -1., -1.,  1.,  1.,    -1.,  1.,  1.,  1.,
      1.,  1., -1.,  1.,    1., -1., -1.,  1.,   -1., -1., -1.,  1.,    -1.,  1., -1.,  1.,
      1.,  1.,  1., -1,     1., -1.,  1., -1.,   -1., -1.,  1., -1.,    -1.,  1.,  1., -1.,
      1.,  1., -1., -1,     1., -1., -1., -1.,   -1., -1., -1., -1.,    -1.,  1., -1., -1.,
  };

// GLfloat vertices[] = { .5,0,0,1,   0,.5,0,1,   0,0,.5,1,   0,0,0,.5 };

  GLuint VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and
  // then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(0);
  glEnable(GL_PROGRAM_POINT_SIZE);

  // This is already registered so we can unbind it
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // Incase we have more VAO's unbind
  glBindVertexArray(0);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // Render
    glClearColor(0.2, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Create transformations
    // Make sure to initialize matrix to identity, a single scalar does this
    glm::mat4 transform = glm::mat4(1.0);
    transform = glm::translate(transform, glm::vec3(0, 0, tra_z));
    transform = glm::rotate(transform, tra_x, glm::vec3(0., 1., 0.));
    transform = glm::rotate(transform, tra_y, glm::vec3(1., 0., 0.));

    // Ready shit for GPU
    float zero = 0.0;
    float one = 1.0;
    shader_prog.setMat4("transform", transform);
    shader_prog.setMat4("xzangle", glm::mat4(
        glm::cos(xrot),  zero,  -glm::sin(xrot),  zero,
                  zero,   one,             zero,  zero,
        glm::sin(xrot),  zero,   glm::cos(xrot),  zero,
                  zero,  zero,             zero,   one
    ));
    shader_prog.setMat4("ywangle", glm::mat4(
        one,             zero,  zero,             zero,
        zero,  glm::cos(yrot),  zero,  -glm::sin(yrot),
        zero,            zero,   one,             zero,
        zero,  glm::sin(yrot),  zero,   glm::cos(yrot)
    ));
    shader_prog.setMat4("proj", glm::mat4(0.));

    // Draw
    shader_prog.activate();

    // Could only bind once but a more complicated prog might have multiple VAO's
    glBindVertexArray(VAO);

    // Do the damn thang
    glDrawArrays(GL_POINTS, 0, 64);

    // Swap buffers and poll IO events to ready for next go round
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glfwTerminate();
  // `shader_prog` is deleted on end of scope (in it's destructor) the vertex/frag/geo shaders
  // are deleted when linked

  return 0;
}

// Input like keyboard/mouse/etc.
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) tra_y += 0.1;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) tra_y -= 0.1;

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) tra_x -= 0.1;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) tra_x += 0.1;

  if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) xrot -= 0.01;
  if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) xrot += 0.01;

  if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) yrot += 0.01;
  if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) yrot -= 0.01;

  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) tra_z -= 0.1;
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) tra_z += 0.1;

  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    xrot = 0.0; yrot = 0.0;
    tra_x = 0.0; tra_y = 0.0; tra_z = 0.0;
  }
}

// If the window has changed size. This is registered with GLFW.
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}
