// These have to be included before GLFW or any other gl* libraries
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

#include "shader.hpp"

#define N(array) (sizeof(array) / sizeof(*array))

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

GLuint SCR_WIDTH = 800;
GLuint SCR_HEIGHT = 600;

float camera_xrot = 0.0;
float camera_yrot = 0.0;

float cube_xwrot = 0.0;
float cube_ywrot = 0.0;

float radius = 4;

// float tra_x = 0.0;
// float tra_y = 0.0;
// float tra_z = 0.0;

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 cube(1.0f);

unsigned divisions = 100;

void recalculate() {
  glm::vec3 position(cos(camera_yrot) * sin(camera_xrot) * radius,
                     sin(camera_yrot) * radius,
                     cos(camera_yrot) * cos(camera_xrot) * radius);

  ProjectionMatrix = glm::perspective(
      glm::radians(70.0f), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 1000.0f);

  ViewMatrix = glm::lookAt(position, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

float map(float x, float x1, float x2, float y1, float y2) {
  return ((x - x1) / (x2 - x1)) * (y2 - y1) + y1;
}

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

  {

    Shader shader_prog = Shader("src/shaders/vert.vs", "src/shaders/frag.fs",
                                "src/shaders/fourdee.gs");

    //  glm::vec4 vertices[] = {
    //    { 1,  1, -1,  1},
    //    { 1,  1,  1,  1},
    //    { 1, -1, -1,  1},
    //    { 1, -1,  1,  1},
    //    {-1,  1, -1,  1},
    //    {-1,  1,  1,  1},
    //    {-1, -1, -1,  1},
    //    {-1, -1,  1,  1},
    //
    //    { 1,  1, -1, -1},
    //    { 1,  1,  1, -1},
    //    { 1, -1, -1, -1},
    //    { 1, -1,  1, -1},
    //    {-1,  1, -1, -1},
    //    {-1,  1,  1, -1},
    //    {-1, -1, -1, -1},
    //    {-1, -1,  1, -1},
    //  };

    struct a {
      float x, y, z, w, r, g, b;
    };

    unsigned alloc_size = (16 * divisions * divisions) * sizeof(struct a);

    struct a *vertices = (struct a *)malloc(alloc_size);

    unsigned n = 0;

    for (unsigned i = 0; i < divisions; i++) {
      for (unsigned j = 0; j < divisions; j++) {
        vertices[n++] = (struct a){
            .x = map(i, 0, divisions, +1, -1),
            .y = +1,
            .z = +1,
            .w = map(j, 0, divisions, +1, -1),
            .r = 0.7f,
            .g = 0.1f,
            .b = 0.6f,
        };
      }
    }

    for (unsigned i = 0; i < divisions; i++) {
      for (unsigned j = 0; j < divisions; j++) {
        vertices[n++] = (struct a){
            .x = -1,
            .y = +1,
            .z = map(i, 0, divisions, +1, -1),
            .w = map(j, 0, divisions, +1, -1),
            .r = 0.7f,
            .g = 0.2f,
            .b = 0.4f,
        };
      }
    }

    for (unsigned i = 0; i < divisions; i++) {
      for (unsigned j = 0; j < divisions; j++) {
        vertices[n++] = (struct a){
            .x = map(i, 0, divisions, +1, -1),
            .y = +1,
            .z = -1,
            .w = map(j, 0, divisions, +1, -1),
            .r = 0.3f,
            .g = 0.5f,
            .b = 0.4f,
        };
      }
    }

    for (unsigned i = 0; i < divisions; i++) {
      for (unsigned j = 0; j < divisions; j++) {
        vertices[n++] = (struct a){
            .x = +1,
            .y = +1,
            .z = map(i, 0, divisions, -1, +1),
            .w = map(j, 0, divisions, +1, -1),
            .r = 0.8f,
            .g = 0.8f,
            .b = 0.4f,
        };
      }
    }

    for (unsigned i = 0; i < divisions; i++) {
      for (unsigned j = 0; j < divisions; j++) {
        vertices[n++] = (struct a){
            .x = map(i, 0, divisions, +1, -1),
            .y = map(j, 0, divisions, +1, -1),
            .z = +1,
            .w = -1,
            .r = 0.5f,
            .g = 0.8f,
            .b = 0.4f,
        };
      }
    }
    //
    for (unsigned i = 0; i < divisions; i++) {
      for (unsigned j = 0; j < divisions; j++) {
        vertices[n++] = (struct a){
            .x = -1,
            .y = map(j, 0, divisions, +1, -1),
            .z = map(i, 0, divisions, +1, -1),
            .w = -1,
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
            .w = -1,
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
            .w = -1,
            .r = 0.1f,
            .g = 0.4f,
            .b = 0.8f,
        };
      }
    }

    for (unsigned i = 0; i < divisions; i++) {
      for (unsigned j = 0; j < divisions; j++) {
        vertices[n++] = (struct a){
            .x = map(i, 0, divisions, +1, -1),
            .y = -1,
            .z = +1,
            .w = map(j, 0, divisions, -1, +1),
            .r = 0.1f,
            .g = 0.4f,
            .b = 0.8f,
        };
      }
    }

    for (unsigned i = 0; i < divisions; i++) {
      for (unsigned j = 0; j < divisions; j++) {
        vertices[n++] = (struct a){
            .x = -1,
            .y = -1,
            .z = map(i, 0, divisions, +1, -1),
            .w = map(j, 0, divisions, -1, +1),
            .r = 0.6f,
            .g = 0.4f,
            .b = 0.7f,
        };
      }
    }

    for (unsigned i = 0; i < divisions; i++) {
      for (unsigned j = 0; j < divisions; j++) {
        vertices[n++] = (struct a){
            .x = map(i, 0, divisions, -1, +1),
            .y = -1,
            .z = -1,
            .w = map(j, 0, divisions, -1, +1),
            .r = 0.9f,
            .g = 0.2f,
            .b = 0.3f,
        };
      }
    }

    for (unsigned i = 0; i < divisions; i++) {
      for (unsigned j = 0; j < divisions; j++) {
        vertices[n++] = (struct a){
            .x = +1,
            .y = -1,
            .z = map(i, 0, divisions, -1, +1),
            .w = map(j, 0, divisions, -1, +1),
            .r = 0.5f,
            .g = 0.5f,
            .b = 0.8f,
        };
      }
    }

    for (unsigned i = 0; i < divisions; i++) {
      for (unsigned j = 0; j < divisions; j++) {
        vertices[n++] = (struct a){
            .x = map(i, 0, divisions, +1, -1),
            .y = map(j, 0, divisions, -1, +1),
            .z = +1,
            .w = +1,
            .r = 0.1f,
            .g = 0.5f,
            .b = 0.8f,
        };
      }
    }

    for (unsigned i = 0; i < divisions; i++) {
      for (unsigned j = 0; j < divisions; j++) {
        vertices[n++] = (struct a){
            .x = -1,
            .y = map(j, 0, divisions, -1, +1),
            .z = map(i, 0, divisions, +1, -1),
            .w = +1,
            .r = 0.8f,
            .g = 0.2f,
            .b = 0.8f,
        };
      }
    }

    for (unsigned i = 0; i < divisions; i++) {
      for (unsigned j = 0; j < divisions; j++) {
        vertices[n++] = (struct a){
            .x = map(i, 0, divisions, -1, +1),
            .y = map(j, 0, divisions, -1, +1),
            .z = -1,
            .w = +1,
            .r = 0.8f,
            .g = 0.7f,
            .b = 0.8f,
        };
      }
    }

    for (unsigned i = 0; i < divisions; i++) {
      for (unsigned j = 0; j < divisions; j++) {
        vertices[n++] = (struct a){
            .x = +1,
            .y = map(j, 0, divisions, -1, +1),
            .z = map(i, 0, divisions, -1, +1),
            .w = +1,
            .r = 0.2f,
            .g = 0.3f,
            .b = 0.8f,
        };
      }
    }

    GLuint VBO, VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s),
    // and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, alloc_size, vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(struct a), nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct a),
                          (void *)(sizeof(float) * 4));

    glEnable(GL_PROGRAM_POINT_SIZE);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // This is already registered so we can unbind it
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Incase we have more VAO's unbind
    glBindVertexArray(0);

    recalculate();

    while (!glfwWindowShouldClose(window)) {
      processInput(window);

      // Render
      glClearColor(0.0, 0.0, 0.0, 1.0);
      glClear(GL_COLOR_BUFFER_BIT);

      glClear(GL_DEPTH_BUFFER_BIT);

      shader_prog.setMat4("cube", cube);

      glm::mat4 MVP = ProjectionMatrix * ViewMatrix;

      shader_prog.setMat4("MVP", MVP);
      //		glUniformMatrix4fv(MPVID, 1, GL_FALSE, &MVP[0][0]);

      // Draw
      shader_prog.activate();

      // Could only bind once but a more complicated prog might have multiple
      // VAO's
      glBindVertexArray(VAO);

      // Do the damn thang
      glDrawArrays(GL_POINTS, 0, n);

      // Swap buffers and poll IO events to ready for next go round
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
  }

  glfwTerminate();
  // `shader_prog` is deleted on end of scope (in it's destructor) the
  // vertex/frag/geo shaders are deleted when linked

  return 0;
}

// Input like keyboard/mouse/etc.
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    float cube_xwrot = -0.1;

    cube = glm::mat4(glm::cos(cube_xwrot), 0.0f, 0.0f, -glm::sin(cube_xwrot),
                     0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                     glm::sin(cube_xwrot), 0.0f, 0.0f, glm::cos(cube_xwrot)) *
           cube;
  }

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    float cube_xwrot = +0.1;

    cube = glm::mat4(glm::cos(cube_xwrot), 0.0f, 0.0f, -glm::sin(cube_xwrot),
                     0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                     glm::sin(cube_xwrot), 0.0f, 0.0f, glm::cos(cube_xwrot)) *
           cube;
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    float cube_ywrot = +0.1;

    cube = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, glm::cos(cube_ywrot), 0.0f,
                     -glm::sin(cube_ywrot), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                     glm::sin(cube_ywrot), 0.0f, glm::cos(cube_ywrot)) *
           cube;
  }

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    float cube_ywrot = -0.1;

    cube = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, glm::cos(cube_ywrot), 0.0f,
                     -glm::sin(cube_ywrot), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                     glm::sin(cube_ywrot), 0.0f, glm::cos(cube_ywrot)) *
           cube;
  }

  float old_camera_xrot = camera_xrot, old_camera_yrot = camera_yrot;

  if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    camera_xrot -= 0.01;
  if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    camera_yrot -= 0.01;
  if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    camera_yrot += 0.01;
  if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    camera_xrot += 0.01;

  //  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) tra_z -= 0.1;
  //  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) tra_z += 0.1;

  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    cube = glm::mat4(1.0);
    camera_xrot = 0.0;
    camera_yrot = 0.0;
    //    tra_x = 0.0; tra_y = 0.0; tra_z = 0.0;
  }

  if (old_camera_xrot != camera_xrot || old_camera_yrot != camera_yrot) {
    recalculate();
  }
}

// If the window has changed size. This is registered with GLFW.
void framebuffer_size_callback(GLFWwindow *window, int new_width,
                               int new_height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.

  SCR_WIDTH = new_width;
  SCR_HEIGHT = new_height;

  recalculate();

  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
}
