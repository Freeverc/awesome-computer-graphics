/*  Justin Tennant  */
#if defined(__APPLE__) && defined(__MACH__)

#include <GLUT/glut.h>

#else
#include <GL/glut.h>
#endif

#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <regex>

// 定义一些基本的数据结构
typedef struct OBJ_COLOR {
  GLfloat red, green, blue;
  OBJ_COLOR() : red(1.0), green(1.0), blue(1.0) {}
  OBJ_COLOR(double r, double g, double b) : red(r), green(g), blue(b) {}
} ObjColor;

typedef struct vertex {
  double x, y, z;
  vertex(double x_, double y_, double z_) : x(x_), y(y_), z(z_){};
} vertex;
typedef struct face_triangle {
  unsigned long v1, v2, v3;
} face_triangle;
typedef struct face_quad {
  unsigned long v1, v2, v3, v4;
} face_quad;

// 要显示的模型
struct ObjModel {
  ObjModel(double x_, double y_, double z_) : position(x_, y_, z_){};
  std::vector<vertex> vertices;
  std::vector<face_quad> faces_quads;
  std::vector<face_triangle> faces_triangles;
  vertex position;
  ObjColor color;
};

bool is_quad;     // 是矩形的模型还是三角形
bool render_mode; // true = solid body, false = wireframe

// 一些参数
const float ZOOM_SPEED = 0.2f;
const float ROTATE_SPEED = 0.2f;
const float MOVE_SPEED = 0.5f;
float DISTANCE = 4.0f;
int move_id = 0;

struct camera {
  GLfloat x, y, z, phi, theta;
  camera() : x(-4.0f), y(2.0f), z(0.0f), phi(0), theta(0) {}
} camera;

std::vector<ObjModel> models;

// 绘制三角形
void drawTriangle(vertex v1, vertex v2, vertex v3) {
  glVertex3f(v1.x, v1.y, v1.z);
  glVertex3f(v2.x, v2.y, v2.z);
  glVertex3f(v3.x, v3.y, v3.z);
}

// 绘制场景, 场景包含5个平面, 每个平面的颜色具有渐变效果
void drawRoom() {
  glBegin(GL_TRIANGLES);
  for (int x = 0; x < 500; x++) {
    for (int z = 0; z < 500; z++) {
      glColor3f(0.8, 0.4 + 0.6 / 250000 * x * z, 0.8);
      vertex v1 = vertex(-125 + x * 0.5, 0, -125 + z * 0.5);
      vertex v2 = vertex(-125 + x * 0.5, 0, -125 + (z + 1) * 0.5);
      vertex v3 = vertex(-125 + (x + 1) * 0.5, 0, -125 + (1 + z) * 0.5);
      vertex v4 = vertex(-125 + (x + 1) * 0.5, 0, -125 + z * 0.5);
      drawTriangle(v1, v2, v3);
      drawTriangle(v1, v3, v4);
    }
  }
  glEnd();
  glPushMatrix();
  glTranslatef(0, 125, -125);
  glRotatef(90, 1, 0, 0);
  glBegin(GL_TRIANGLES);
  for (int x = 0; x < 500; x++) {
    for (int z = 0; z < 500; z++) {
      // glColor3f(0.8, 0.4 + 0.6/250000*x*z, 0.8);
      glColor3f(0.8, 0.4 + 0.6 / 250000 * x * z, 0.8);
      vertex v1 = vertex(-125 + x * 0.5, 0, -125 + z * 0.5);
      vertex v2 = vertex(-125 + x * 0.5, 0, -125 + (z + 1) * 0.5);
      vertex v3 = vertex(-125 + (x + 1) * 0.5, 0, -125 + (1 + z) * 0.5);
      vertex v4 = vertex(-125 + (x + 1) * 0.5, 0, -125 + z * 0.5);
      drawTriangle(v1, v2, v3);
      drawTriangle(v1, v3, v4);
    }
  }
  glEnd();
  glPopMatrix();
  glPushMatrix();
  glTranslatef(0, 125, 125);
  glRotatef(-90, 1, 0, 0);
  glBegin(GL_TRIANGLES);
  for (int x = 0; x < 500; x++) {
    for (int z = 0; z < 500; z++) {
      glColor3f(0.8, 0.4 + 0.6 / 250000 * x * z, 0.8);
      vertex v1 = vertex(-125 + x * 0.5, 0, -125 + z * 0.5);
      vertex v2 = vertex(-125 + x * 0.5, 0, -125 + (z + 1) * 0.5);
      vertex v3 = vertex(-125 + (x + 1) * 0.5, 0, -125 + (1 + z) * 0.5);
      vertex v4 = vertex(-125 + (x + 1) * 0.5, 0, -125 + z * 0.5);
      drawTriangle(v1, v2, v3);
      drawTriangle(v1, v3, v4);
    }
  }
  glEnd();
  glPopMatrix();
  glPushMatrix();
  glTranslatef(125, 125, 0);
  glRotatef(90, 0, 0, 1);
  glBegin(GL_TRIANGLES);
  for (int x = 0; x < 500; x++) {
    for (int z = 0; z < 500; z++) {
      glColor3f(0.8, 0.4 + 0.6 / 250000 * x * z, 0.8);
      vertex v1 = vertex(-125 + x * 0.5, 0, -125 + z * 0.5);
      vertex v2 = vertex(-125 + x * 0.5, 0, -125 + (z + 1) * 0.5);
      vertex v3 = vertex(-125 + (x + 1) * 0.5, 0, -125 + (1 + z) * 0.5);
      vertex v4 = vertex(-125 + (x + 1) * 0.5, 0, -125 + z * 0.5);
      drawTriangle(v1, v2, v3);
      drawTriangle(v1, v3, v4);
    }
  }
  glEnd();
  glPopMatrix();
  glPushMatrix();
  glTranslatef(-125, 125, 0);
  glRotatef(-90, 0, 0, 1);
  glBegin(GL_TRIANGLES);
  for (int x = 0; x < 500; x++) {
    for (int z = 0; z < 500; z++) {
      glColor3f(0.8, 0.4 + 0.6 / 250000 * x * z, 0.8);
      vertex v1 = vertex(-125 + x * 0.5, 0, -125 + z * 0.5);
      vertex v2 = vertex(-125 + x * 0.5, 0, -125 + (z + 1) * 0.5);
      vertex v3 = vertex(-125 + (x + 1) * 0.5, 0, -125 + (1 + z) * 0.5);
      vertex v4 = vertex(-125 + (x + 1) * 0.5, 0, -125 + z * 0.5);
      drawTriangle(v1, v2, v3);
      drawTriangle(v1, v3, v4);
    }
  }
  glEnd();
  glPopMatrix();
}

// 初始化，设置光线等，这里不需要场景太复杂,不需要复杂的光线，就注掉了
void init() {
  // glShadeModel(GL_SMOOTH);
  // glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  // glClearDepth(0.0f);
  // glEnable(GL_DEPTH_TEST);
  // glDepthFunc(GL_LEQUAL);
  // glEnable(GL_COLOR);
  // glEnable(GL_COLOR_MATERIAL);
  // glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  // glEnable(GL_LIGHTING);
  // glEnable(GL_NORMALIZE);
  // glEnable(GL_LIGHT1);
  // GLfloat lightAmbient1[4] = {0.2, 0.2, 0.2, 1.0};
  // GLfloat lightPos1[4] = {0.5, 0.5, 0.5, 1.0};
  // GLfloat lightDiffuse1[4] = {0.8, 0.8, 0.8, 1.0};
  // GLfloat lightSpec1[4] = {1.0, 1.0, 1.0, 1.0};
  // GLfloat lightLinAtten = 0.0f;
  // GLfloat lightQuadAtten = 1.0f;
  // glLightfv(GL_LIGHT1, GL_POSITION, (GLfloat *) &lightPos1);
  // glLightfv(GL_LIGHT1, GL_AMBIENT, (GLfloat *) &lightAmbient1);
  // glLightfv(GL_LIGHT1, GL_DIFFUSE, (GLfloat *) &lightDiffuse1);
  // glLightfv(GL_LIGHT1, GL_SPECULAR, (GLfloat *) &lightSpec1);
  // glLightfv(GL_LIGHT1, GL_LINEAR_ATTENUATION, &lightLinAtten);
  // glLightfv(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, &lightQuadAtten);
  // glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

// 计算平面的法向量，用于渲染网格
void calculate_normal(ObjModel &model, face_triangle f, GLdouble *normal) {
  // x
  normal[0] = (model.vertices[f.v2 - 1].y - model.vertices[f.v1 - 1].y) *
                  (model.vertices[f.v3 - 1].z - model.vertices[f.v1 - 1].z) -
              (model.vertices[f.v3 - 1].y - model.vertices[f.v1 - 1].y) *
                  (model.vertices[f.v2 - 1].z - model.vertices[f.v1 - 1].z);
  // y
  normal[1] = (model.vertices[f.v2 - 1].z - model.vertices[f.v1 - 1].z) *
                  (model.vertices[f.v3 - 1].x - model.vertices[f.v1 - 1].x) -
              (model.vertices[f.v2 - 1].x - model.vertices[f.v1 - 1].x) *
                  (model.vertices[f.v3 - 1].z - model.vertices[f.v1 - 1].z);
  // z
  normal[2] = (model.vertices[f.v2 - 1].x - model.vertices[f.v1 - 1].x) *
                  (model.vertices[f.v3 - 1].y - model.vertices[f.v1 - 1].y) -
              (model.vertices[f.v3 - 1].x - model.vertices[f.v1 - 1].x) *
                  (model.vertices[f.v2 - 1].y - model.vertices[f.v1 - 1].y);
}

// 绘制模型
void draw_obj() {
  if (render_mode) {
    // 绘制填充图形
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  } else {
    // 不填充图形
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  if (is_quad) {
    // 绘制矩形模型
    for (ObjModel model : models) {
      for (face_quad f : model.faces_quads) {
        // Calculate normal by calculating two tri normals & averaging
        face_triangle f_tri1 = {f.v1, f.v2, f.v3};
        face_triangle f_tri2 = {f.v2, f.v3, f.v4};
        GLdouble normal_tri1[3];
        GLdouble normal_tri2[3];
        calculate_normal(model, f_tri1, normal_tri1);
        calculate_normal(model, f_tri2, normal_tri2);
        GLdouble normal[3] = {(normal_tri1[0] + normal_tri2[0]) / 2,
                              (normal_tri1[1] + normal_tri2[1]) / 2,
                              (normal_tri1[2] + normal_tri2[2]) / 2};

        glBegin(GL_QUADS);
        glColor3f(model.color.red, model.color.green, model.color.blue);
        glNormal3dv(normal);
        glVertex3d(model.position.x + model.vertices[f.v1 - 1].x,
                   model.position.y + model.vertices[f.v1 - 1].y,
                   model.position.z + model.vertices[f.v1 - 1].z);
        glVertex3d(model.position.x + model.vertices[f.v2 - 1].x,
                   model.position.y + model.vertices[f.v2 - 1].y,
                   model.position.z + model.vertices[f.v2 - 1].z);
        glVertex3d(model.position.x + model.vertices[f.v3 - 1].x,
                   model.position.y + model.vertices[f.v3 - 1].y,
                   model.position.z + model.vertices[f.v3 - 1].z);
        glVertex3d(model.position.x + model.vertices[f.v4 - 1].x,
                   model.position.y + model.vertices[f.v4 - 1].y,
                   model.position.z + model.vertices[f.v4 - 1].z);
        glEnd();
      }
    }
  } else {
    // 绘制三角模型
    for (ObjModel model : models) {
      for (face_triangle f : model.faces_triangles) {
        GLdouble normal[3];
        calculate_normal(model, f, normal);
        glBegin(GL_TRIANGLES);
        glColor3f(model.color.red, model.color.green, model.color.blue);
        glNormal3dv(normal);
        glVertex3d(model.position.x + model.vertices[f.v1 - 1].x,
                   model.position.y + model.vertices[f.v1 - 1].y,
                   model.position.z + model.vertices[f.v1 - 1].z);
        glVertex3d(model.position.x + model.vertices[f.v2 - 1].x,
                   model.position.y + model.vertices[f.v2 - 1].y,
                   model.position.z + model.vertices[f.v2 - 1].z);
        glVertex3d(model.position.x + model.vertices[f.v3 - 1].x,
                   model.position.y + model.vertices[f.v3 - 1].y,
                   model.position.z + model.vertices[f.v3 - 1].z);
        glEnd();
      }
    }
  }
  glFlush();
}

// 改变窗口大小
void reshape(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (h == 0) {
    gluPerspective(90, (float)w, 1.0, 5000.0);
  } else {
    gluPerspective(90, (float)w / (float)h, 1.0, 5000.0);
  }
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

// 方向键的处理，用于调整相机视角
void arrow_keys(int key, int x, int y) {
  switch (key) {
    // 改变相机距离
  case GLUT_KEY_UP: {
    DISTANCE -= ZOOM_SPEED;
    break;
  }
  case GLUT_KEY_DOWN: {
    DISTANCE += ZOOM_SPEED;
    break;
  }
    // 改变相机角度
  case GLUT_KEY_LEFT: {
    camera.theta -= ROTATE_SPEED;
    break;
  }
  case GLUT_KEY_RIGHT:
    camera.theta += ROTATE_SPEED;
    break;
  default:
    break;
  }
}

// 普通键的处理，用于移动物体、切换要移动的物体等。
void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 27:
    exit(0);
  case 'm':
    render_mode = !render_mode;
    break;
    // 改变物体
  case 'c':
    move_id++;
    move_id %= 3;
    break;
    // 水平移动
  case 'w':
    models[move_id].position.x += MOVE_SPEED;
    break;
  case 's':
    models[move_id].position.x -= MOVE_SPEED;
    break;
    // 水平移动
  case 'a':
    models[move_id].position.z += MOVE_SPEED;
    break;
  case 'd':
    models[move_id].position.z -= MOVE_SPEED;
    break;
    // 上下移动
  case 'q':
    models[move_id].position.y += MOVE_SPEED;
    break;
  case 'e':
    models[move_id].position.y -= MOVE_SPEED;
    break;
  default:
    break;
  }
}

// 显示界面，调用上面的一些函数
void display() {
  // 先初始化相机
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  camera.x = DISTANCE * cos(camera.phi) * sin(camera.theta);
  camera.y = 5.0f + DISTANCE * sin(camera.phi) * sin(camera.theta);
  camera.z = DISTANCE * cos(camera.theta);

  gluLookAt(camera.x, camera.y, camera.z, 0, 2.0f, 0, 0.0f, 1.0f, 0.0f);
  // 绘制场景和物体
  drawRoom();
  draw_obj();
  glutSwapBuffers();
  glutPostRedisplay();
}

// 读取obj文件的每一行
void obj_parse(char *file_line, ObjModel &model) {

  if (file_line[0] == '\0') {
    // End of file
    return;
  }

  char *tokenized;
  tokenized = strtok(file_line, " ");
  char first_char = tokenized[0];

  if (first_char == '#') {
    // Comment line, ignore
    return;
  } else if (first_char == 'v') {
    // vertex

    double x = strtod(strtok(NULL, " "), NULL);
    double y = strtod(strtok(NULL, " "), NULL);
    double z = strtod(strtok(NULL, " "), NULL);

    vertex v = {x * 20, y * 20, z * 20};
    model.vertices.push_back(v);
  } else if (first_char == 'f') {
    // Face

    unsigned long v1 = strtoul(strtok(NULL, " "), NULL, 0);
    unsigned long v2 = strtoul(strtok(NULL, " "), NULL, 0);
    unsigned long v3 = strtoul(strtok(NULL, " "), NULL, 0);

    unsigned long v4;
    char *v4_str = strtok(NULL, " ");
    if (v4_str != NULL) {
      // Face is a quad
      v4 = strtoul(v4_str, NULL, 0);

      face_quad f = {v1, v2, v3, v4};
      model.faces_quads.push_back(f);
    } else {
      // Face is a triangle
      face_triangle f = {v1, v2, v3};
      model.faces_triangles.push_back(f);
    }
  }
}

// 读取obj文件，转化成我们定义的模型
void open_obj(std::string &filename, ObjModel &model) {

  //// Argument parsing ////
  std::regex obj_regex(".+(\\.obj)$");
  if (!std::regex_match(filename, obj_regex)) {
    // Filename is invalid
    std::cout << "This application only accepts *.obj files, exiting..."
              << std::endl;
    exit(1);
  }

  //// Filename accepted, attempting to open ////

  std::cout << "Opening file: " << filename << std::endl;

  std::ifstream file_stream;
  file_stream.open(filename, std::ifstream::in);

  if ((file_stream.rdstate() & std::ifstream::failbit) != 0) {
    std::cerr << "Error opening " << filename << std::endl;
    exit(1);
  }

  while (file_stream.good()) {
    char file_line[100];
    file_stream.getline(file_line, 100);
    obj_parse(file_line, model);
  }

  file_stream.close();

  is_quad = (model.faces_quads.size() > model.faces_triangles.size());
}

int main(int argc, char *argv[]) {
  std::cout << "array : zoom and rotate camera\n \
w a s d q e : Move objects\n \
c : change object to move \n \
t : switch between rendering faces and not\n"
            << std::endl;

  // 读取三个文件
  std::string filename_0 = "./models/bunny.obj";
  std::string filename_1 = "./models/sandal_mini.obj";
  std::string filename_2 = "./models/cessna_mini.obj";
  models.push_back(ObjModel(10, 0, 0));
  models.push_back(ObjModel(0, 2, 10));
  models.push_back(ObjModel(0, 40, 0));
  models[0].color = ObjColor(0.9, 0.7, 0.7);
  models[1].color = ObjColor(0.5, 1, 0.5);
  models[2].color = ObjColor(0.5, 0.5, 1);
  open_obj(filename_0, models[0]);
  open_obj(filename_1, models[1]);
  open_obj(filename_2, models[2]);
  // initialize rendering with solid body
  render_mode = true;

  // 初始化
  std::cout << "Running..." << std::endl;
  int window;
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(960, 720);
  glutInitWindowPosition(0, 0);
  window = glutCreateWindow("Justin Tennant OBJ Visualizer");
  init();
  // 设置各种回调函数
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutSpecialFunc(arrow_keys);
  glutKeyboardFunc(keyboard);
  // 程序真正跑起来
  glutMainLoop();
}