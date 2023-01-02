#define GLUT_DISABLE_ATEXIT_HACK
// #include <glut.h>
#ifdef __APPLE__
#include <Glut/glut.h>
#include <OpenGL/gl.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <GLUT/glcmolorut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>

float PI = 3.1415926;
GLfloat width = 640;
GLfloat height = 480;
float theta = -90.0;
float eyex = 0, eyey = 0, eyez = 0;
float atx = 0, aty = 0, atz = 0;
bool flag = true;
int inner = 10, outer = 80;
float s = outer + 4 * inner + 50;
int ww, hh;

float angle = 0;
float r = s;
float step = r / 10;

void Display(void);
void Reshape(int w, int h);
void mytime(int value);
void drawground();
void drawsphere();
void draw_box();
void drawwall();
void init();
void mykeyboard(unsigned char key, int x, int y);

int main(int argc, char *argv[]) {
  //初始化GULT库
  glutInit(&argc, argv);
  //设置显示方式，其中GLUT_RGB表示使用RGB颜色，与之对应的还有GLUT_INDEX（表示使用索引颜色）。
  // GLUT_SINGLE表示使用单缓冲，与之对应的还有GLUT_DOUBLE（使用双缓冲）
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  //初始化窗口位置
  glutInitWindowPosition(100, 100);
  //初始化窗口大小
  glutInitWindowSize(width, height);
  //创建窗口，参数将被作为窗口的标题。
  glutCreateWindow("3D World");
  glutReshapeFunc(Reshape);

  init();
  //显示回调函数，窗口需要重绘时就会调用其中注册的函数
  //一般将要显示的内容放在显示回调函数里
  glutDisplayFunc(Display);
  glutKeyboardFunc(mykeyboard);
  glutTimerFunc(100, mytime, 10);
  glutMainLoop();
  return 0;
}

void init() {
  glClearColor(1, 1, 1, 1);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void mykeyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 'W':
  case 'w':
    eyex = eyex - step * sin(angle * PI / 180.0);
    eyez = eyez - step * cos(angle * PI / 180.0);
    atx = atx - step * sin(angle * PI / 180.0);
    atz = atz - step * cos(angle * PI / 180.0);
    break;

  case 'S':
  case 's':
    eyex = eyex + step * sin(angle * PI / 180.0);
    eyez = eyez + step * cos(angle * PI / 180.0);
    atx = atx + step * sin(angle * PI / 180.0);
    atz = atz + step * cos(angle * PI / 180.0);
    break;

  case 'A':
  case 'a':
    angle += 1;
    atx = eyex - r * sin(angle * PI / 180.0);
    atz = eyez - r * cos(angle * PI / 180.0);
    break;

  case 'D':
  case 'd':
    angle -= 1;
    atx = eyex - r * sin(angle * PI / 180.0);
    atz = eyez - r * cos(angle * PI / 180.0);
    break;
  }
  glutPostRedisplay();
}

void Display(void) {
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(eyex, eyey, eyez, atx, aty, atz, 0, 1, 0);
  glPushMatrix();
  glColor3f(0.0, 0.0, 1.0);
  drawwall();
  glColor3f(1.0, 0.0, 0.0);
  drawground();
  drawsphere();

  glColor3f(0.0, 1.0, 0.0);
  draw_box();
  glPopMatrix();
  glutSwapBuffers();
}

void drawsphere() {
  float tr;
  tr = (outer + 3 * inner);
  glRotatef(theta, 0, 1, 0);

  glPushMatrix();
  glColor3f(1.0, 0, 1.0);
  // glutWireTorus(inner, outer, 30, 50);
  glutSolidTorus(inner, outer, 30, 50);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(outer, 0, 0);
  glRotatef(theta, 0, 1, 0);
  glTranslatef(-outer, 0, 0);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(tr, 0, 0);
  glRotatef(-45, 1, 0, 0);
  glColor3f(0.0, 1.0, 0.0);
  // glutWireTeapot(30);
  glutSolidTeapot(30);
  glPopMatrix();
}

void draw_box() {
  // // ground
  // for (int i = -outer - 4 * inner; i < outer + 4 * inner; i += 6 * inner)
  //   for (int j = -outer - 4 * inner; j < outer + 4 * inner; j += 6 * inner) {
  //     glBegin(GL_QUADS);
  //     glVertex3d(j, -outer - 4 * inner, i);
  //     glVertex3d(j, -outer - 4 * inner, i + 6 * inner);
  //     glVertex3d(j + 2 * inner, -outer - 4 * inner, i + 6 * inner);
  //     glVertex3d(j + 2 * inner, -outer - 4 * inner, i);
  //     glEnd();
  //   }

  float tr = (outer + 2 * inner);
  glPushMatrix();
  glTranslatef(tr, 0, 0);
  glRotatef(-45, 1, 0, 0);
  glColor3f(0.0, 1.0, 0.0);
  // glutSolidCube(30);
  glutWireSphere(10, 100, 1);
  glPopMatrix();
}

void drawground() {
  // ground
  for (int i = -outer - 4 * inner; i < outer + 4 * inner; i += 2 * inner)
    for (int j = -outer - 4 * inner; j < outer + 4 * inner; j += 2 * inner) {
      glBegin(GL_QUADS);
      glVertex3d(j, -outer - 4 * inner, i);
      glVertex3d(j, -outer - 4 * inner, i + 2 * inner);
      glVertex3d(j + 2 * inner, -outer - 4 * inner, i + 2 * inner);
      glVertex3d(j + 2 * inner, -outer - 4 * inner, i);
      glEnd();
    }

  // top
  for (int i = -outer - 4 * inner; i < outer + 4 * inner; i += 2 * inner)
    for (int j = -outer - 4 * inner; j < outer + 4 * inner; j += 2 * inner) {
      glBegin(GL_QUADS);
      glVertex3d(j, outer + 4 * inner, i);
      glVertex3d(j, outer + 4 * inner, i + 2 * inner);
      glVertex3d(j + 2 * inner, outer + 4 * inner, i + 2 * inner);
      glVertex3d(j + 2 * inner, outer + 4 * inner, i);
      glEnd();
    }
}

void drawwall() {
  int i, j;
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // LEFT
  for (i = -outer - 4 * inner; i < outer + 4 * inner; i += 2 * inner)
    for (j = -outer - 4 * inner; j < outer + 4 * inner; j += 2 * inner) {
      glBegin(GL_QUADS);
      glVertex3d(-outer - 4 * inner, j, i);
      glVertex3d(-outer - 4 * inner, j + 2 * inner, i);
      glVertex3d(-outer - 4 * inner, j + 2 * inner, i + 2 * inner);
      glVertex3d(-outer - 4 * inner, j, i + 2 * inner);
      glEnd();
    }

  // right
  for (i = -outer - 4 * inner; i <= outer + 4 * inner - 2 * inner;
       i += 2 * inner)
    for (j = -outer - 4 * inner; j <= outer + 4 * inner - 2 * inner;
         j += 2 * inner) {
      glBegin(GL_QUADS);
      glVertex3d(outer + 4 * inner, j, i);
      glVertex3d(outer + 4 * inner, j + 2 * inner, i);
      glVertex3d(outer + 4 * inner, j + 2 * inner, i + 2 * inner);
      glVertex3d(outer + 4 * inner, j, i + 2 * inner);
      glEnd();
    }
  glColor3f(1.0, 1.0, 0.0);

  // front
  for (i = -outer - 4 * inner; i <= outer + 4 * inner - 2 * inner;
       i += 2 * inner)
    for (j = -outer - 4 * inner; j <= outer + 4 * inner - 2 * inner;
         j += 2 * inner) {
      glBegin(GL_QUADS);
      glVertex3d(j, i, -outer - 4 * inner);
      glVertex3d(j + 2 * inner, i, -outer - 4 * inner);
      glVertex3d(j + 2 * inner, i + 2 * inner, -outer - 4 * inner);
      glVertex3d(j, i + 2 * inner, -outer - 4 * inner);
      glEnd();
    }
}

void mytime(int value) {
  theta += 0.5;
  if (theta >= 360.0)
    theta -= 360.0;
  glutPostRedisplay();
  glutTimerFunc(100, mytime, 10);
}

void Reshape(GLsizei w, GLsizei h) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90, w / h, 10, 2 * outer + 8 * inner + 250);
  glViewport(0, 0, w, h);
  glMatrixMode(GL_MODELVIEW);
  ww = w;
  hh = h;
};