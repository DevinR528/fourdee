// glad has to be included before GLFW or any other gl* libraries
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

#include "points.hpp"
#include "shader.hpp"
#include "text.hpp"

#ifdef __GNUC__
#define UNUSED(x) UNUSED_##x __attribute__((__unused__))
#else
#define UNUSED(x) UNUSED_##x
#endif

#define N(array) (sizeof(array) / sizeof(*array))

static void framebuffer_size_callback(GLFWwindow *window, int width,
                                      int height);
static void cursor_position_callback(GLFWwindow *window, double x, double y);
void processInput(GLFWwindow *window);

GLuint SCR_WIDTH = 800;
GLuint SCR_HEIGHT = 800;

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
  // clang-format off
  glm::vec3 position(
    cos(camera_yrot) * sin(camera_xrot) * radius,
    sin(camera_yrot) * radius,
    cos(camera_yrot) * cos(camera_xrot) * radius);
// clang-format off
  ProjectionMatrix = glm::perspective(
      glm::radians(70.0f), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 1000.0f);
  ViewMatrix = glm::lookAt(position, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

}

int main() {
  // Initialize and configure openGL using GLFW
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
  // Set callbacks for input
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, cursor_position_callback);

  // glad: load all OpenGL function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  printf("Renderer: %s\n", glGetString(GL_RENDERER));
  printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

  // Initialize dumb fonts...
  std::map<u_char, Character> char_map;
  if (init_fonts(char_map) != 0) {
    return -1;
  }

  {
    Shader text_shader = Shader("src/shaders/text.vs", "src/shaders/text.fs");

    Shader shader_prog = Shader("src/shaders/vert.vs", "src/shaders/frag.fs",
                                "src/shaders/fourdee.gs");

    unsigned alloc_size = (16 * divisions * divisions) * sizeof(struct a);

    struct a *vertices = (struct a *)malloc(alloc_size);

    unsigned n = push_points(divisions, vertices);

    size_t text_vertices = sizeof(float) * 6 * 4;
    // We got buffers for our points and for text now
    GLuint VBO, VAO, TBO, TAO;

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

    // Enable openGL stuff for points to work
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // This is already registered so we can unbind it
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Incase we have more VAO's unbind
    glBindVertexArray(0);

    // Now create text buffers... maybe
    glGenVertexArrays(1, &TAO);
    glGenBuffers(1, &TBO);
    glBindVertexArray(TAO);
    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Enable openGL stuff for text to work
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Must set `ProjectionMatrix` and `ViewMatrix` to something based on size of window
    recalculate();

    while (!glfwWindowShouldClose(window)) {
      processInput(window);

      // Render
      glClearColor(0.0, 0.0, 0.0, 1.0);
      glClear(GL_COLOR_BUFFER_BIT);
      glClear(GL_DEPTH_BUFFER_BIT);

      shader_prog.activate();
      shader_prog.setMat4("cube", cube);
      glm::mat4 MVP = ProjectionMatrix * ViewMatrix;
      shader_prog.setMat4("MVP", MVP);

      glBindVertexArray(VAO);
      // Do the damn thang
      glDrawArrays(GL_POINTS, 0, n);

    //   glBindVertexArray(0);
      // clang-format off
      render_text(
        char_map,
        text_shader, TAO, TBO,
        "Hey Zander!!", SCR_HEIGHT, SCR_WIDTH,
        600.0f, 770.0f, 0.5f,
        glm::vec3(0.3f, 0.7f, 0.9f)
      );
      // clang-format on

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
    float cube_xwrot = -0.01;

    // clang-format off
    cube = glm::mat4(
        glm::cos(cube_xwrot), 0.0f, 0.0f, -glm::sin(cube_xwrot),
        0.0f,                 1.0f, 0.0f,                  0.0f,
        0.0f,                 0.0f, 1.0f,                  0.0f,
        glm::sin(cube_xwrot), 0.0f, 0.0f,  glm::cos(cube_xwrot)
    ) * cube;
    // clang-format on
  }

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    float cube_xwrot = +0.01;

    // clang-format off
    cube = glm::mat4(
        glm::cos(cube_xwrot), 0.0f, 0.0f, -glm::sin(cube_xwrot),
        0.0f,                 1.0f, 0.0f,                  0.0f,
        0.0f,                 0.0f, 1.0f,                  0.0f,
        glm::sin(cube_xwrot), 0.0f, 0.0f,  glm::cos(cube_xwrot)
    ) * cube;
    // clang-format on
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    float cube_ywrot = +0.01;

    // clang-format off
    cube = glm::mat4(
        1.0f,                 0.0f, 0.0f,                  0.0f,
        0.0f, glm::cos(cube_ywrot), 0.0f, -glm::sin(cube_ywrot),
        0.0f,                 0.0f, 1.0f,                  0.0f,
        0.0f, glm::sin(cube_ywrot), 0.0f,  glm::cos(cube_ywrot)
    ) * cube;
    // clang-format on
  }

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    float cube_ywrot = -0.01;
    // clang-format off
    cube = glm::mat4(
        1.0f,                 0.0f, 0.0f,                  0.0f,
        0.0f, glm::cos(cube_ywrot), 0.0f, -glm::sin(cube_ywrot),
        0.0f,                 0.0f, 1.0f,                  0.0f,
        0.0f, glm::sin(cube_ywrot), 0.0f,  glm::cos(cube_ywrot)
    ) * cube;
    // clang-format on
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

bool L_PRESS = false;
bool R_PRESS = false;
double START_CAMERA_X;
double START_CAMERA_Y;
double START_MOUSE_X;
double START_MOUSE_Y;
glm::mat4 START_CUBE;

static void cursor_position_callback(GLFWwindow *window, double xpos,
                                     double ypos) {
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS &&
      !R_PRESS) {
    R_PRESS = true;
    START_CAMERA_X = camera_xrot;
    START_CAMERA_Y = camera_yrot;

    START_MOUSE_X = xpos;
    START_MOUSE_Y = ypos;
  } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) ==
                 GLFW_RELEASE &&
             R_PRESS) {
    R_PRESS = false;
  }

  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS &&
      !L_PRESS) {
    L_PRESS = true;
    START_CUBE = cube;

    START_MOUSE_X = xpos;
    START_MOUSE_Y = ypos;
  } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) ==
                 GLFW_RELEASE &&
             L_PRESS) {
    L_PRESS = false;
  }

  if (R_PRESS) {
    camera_xrot = START_CAMERA_X +
                  map(xpos - START_MOUSE_X, 0, SCR_WIDTH, 0, -(2.0f * M_PI));
    camera_yrot = START_CAMERA_Y +
                  map(ypos - START_MOUSE_Y, 0, SCR_HEIGHT, 0, 2.0f * M_PI);
    recalculate();
  }

  if (L_PRESS) {
    float delta_x = map(xpos - START_MOUSE_X, 0, SCR_WIDTH, 0, -(2.0f * M_PI));
    float delta_y = map(ypos - START_MOUSE_Y, 0, SCR_HEIGHT, 0, 2.0f * M_PI);
    // clang-format off
    cube = glm::mat4(
        glm::cos(delta_x), 0.0f, 0.0f, -glm::sin(delta_x),
                     0.0f, 1.0f, 0.0f,               0.0f,
                     0.0f, 0.0f, 1.0f,               0.0f,
        glm::sin(delta_x), 0.0f, 0.0f,  glm::cos(delta_x)
    ) * START_CUBE;

    cube = glm::mat4(
        1.0f,              0.0f, 0.0f,               0.0f,
        0.0f, glm::cos(delta_y), 0.0f, -glm::sin(delta_y),
        0.0f,              0.0f, 1.0f,               0.0f,
        0.0f, glm::sin(delta_y), 0.0f,  glm::cos(delta_y)
    ) * cube;
    // clang-format on
  }
}

// If the window has changed size. This is registered with GLFW.
static void framebuffer_size_callback(GLFWwindow *UNUSED(window), int new_width,
                                      int new_height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  SCR_WIDTH = new_width;
  SCR_HEIGHT = new_height;

  recalculate();

  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
}
