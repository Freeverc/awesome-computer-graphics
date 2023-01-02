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
#include <stdio.h>
#include <stdlib.h>

float PI = 3.1415926;
GLfloat width = 640;
GLfloat height = 480;
float theta = -90.0;
float eyex = 0, eyey = 0, eyez = 0;
float atx = 0, aty = 0, atz = 0;
bool flag = true;
int inner = 10, outer = 100;
float s = outer + 50;
int ww, hh;

float angle = 0;
float r = s;
float step = r / 10;
bool render_mode = true;
GLuint texCube;
GLuint texSky;

void Display(void);
void Reshape(int w, int h);
void mytime(int value);
void drawground();
void drawsphere();
void drawwall();
void init();
void RenderInit();
void mykeyboard(unsigned char key, int x, int y);
void load_texture(const char *file_name);

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

  load_texture("../images/im.bmp");

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
  glShadeModel(GL_SMOOTH);

  if (render_mode) {
    // 绘制填充图形
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  } else {
    // 不填充图形
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  RenderInit();
}

void RenderInit() {

  // // glEnable(GL_CULL_FACE); // 使能物体背面不渲染
  // glFrontFace(GL_CCW);   // 正面环绕方式，逆时针
  // glEnable(GL_LIGHTING); // 使能光照
  // glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight); //设置背景光照

  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glShadeModel(GL_SMOOTH);

  // 背景颜色
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  GLfloat light_position[] = {1.0, 1.0, 1.0, 100.0}; // 1.0表示光源为点坐标x,y,z
  GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0}; //光源的颜色
  GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0}; //微弱环境光，使物体可见
  GLfloat ambientLight[] = {1.0, 1.0, 1.0, 1.0}; //环境光照

  glLightfv(GL_LIGHT0, GL_POSITION,
            light_position); //光源编号-7，光源特性，参数数据
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT,
                 lmodel_ambient); //指定全局的环境光，物体才能可见//*/

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST); // 使能深度测试
  glEnable(GL_TEXTURE_2D);
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
  drawground();

  drawsphere();

  glutSwapBuffers();
}

void load_texture(const char *file_name) {
  GLint width, height, total_bytes;
  GLubyte *pixels = 0;
  GLuint last_texture_ID = 0, texture_ID = 0;

  // 打开文件，如果失败，返回
  FILE *pFile = fopen(file_name, "rb");
  if (pFile == 0) {
    printf("Wrong!!!!\n");
    return;
  }

#define BMP_Header_Length 54 //图像数据在内存块中的偏移量

  // 读取文件中图象的宽度和高度
  fseek(pFile, 0x0012, SEEK_SET);
  fread(&width, 4, 1, pFile);
  fread(&height, 4, 1, pFile);
  fseek(pFile, BMP_Header_Length, SEEK_SET);

  printf("im %d %d\n", width, height);

  // 计算每行像素所占字节数，并根据此数据计算总像素字节数
  GLint line_bytes = width * 3;
  while (line_bytes % 4 != 0)
    ++line_bytes;
  total_bytes = line_bytes * height;

  // 根据总像素字节数分配内存
  pixels = (GLubyte *)malloc(total_bytes);
  if (pixels == 0) {
    fclose(pFile);
    return;
  }

  // 读取像素数据
  if (fread(pixels, total_bytes, 1, pFile) <= 0) {
    free(pixels);
    fclose(pFile);
    return;
  }

  // 绑定新的纹理，载入纹理并设置纹理参数
  // 在绑定前，先获得原来绑定的纹理编号，以便在最后进行恢复
  GLint lastTextureID = last_texture_ID;
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureID);

  // 分配一个新的纹理编号
  glGenTextures(1, &texSky);
  if (texSky == 0) {
    free(pixels);
    fclose(pFile);
    return;
  }

  glBindTexture(GL_TEXTURE_2D, texSky); //绑定纹理

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT,
               GL_UNSIGNED_BYTE, pixels); //为纹理对象指定一个纹理

  //设置4个常用的纹理参数。。如何把纹理像素映射成像素
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
            GL_REPLACE); //指定纹理贴图与材质的混合模式

  glGenTextures(1, &texCube);
  if (texCube == 0) {
    free(pixels);
    fclose(pFile);
    return;
  }

  glEnable(GL_TEXTURE_CUBE_MAP);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texCube);
  glActiveTexture(GL_TEXTURE0);

  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0,
               GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0,
               GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);

  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0,
               GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0,
               GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);

  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0,
               GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0,
               GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);

  // glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

  // glBindTexture(GL_TEXTURE_2D, lastTextureID); //恢复之前的纹理绑定

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
            GL_REPLACE); //指定纹理贴图与材质的混合模式

  free(pixels);
  fclose(pFile);
}

void drawsphere() {
  if (render_mode) {
    // 绘制填充图形
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  } else {
    // 不填充图形
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  float x = -outer / 2, y = -outer / 2, z = -outer / 2, w = 30;

  float positionData[] = {-1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,
                          1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
                          1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,
                          -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f};
  for (int i = 0; i < 24; ++i) {
    positionData[i] *= w;
  }
  unsigned int indexData[] = {0, 1, 2, 3, 6, 7, 4, 5, 1, 3, 5, 7,
                              0, 2, 4, 6, 3, 5, 4, 2, 1, 7, 6, 0};

  glBindTexture(GL_TEXTURE_2D, 0);

  glPushMatrix();
  glTranslatef(x, y, z);
  glRotatef(30, 0, 1, 0);
  glColor3f(1, 0.0, 0.0);
  glutSolidCube(w);
  glPopMatrix();

  // glEnable(GL_TEXTURE_CUBE_MAP);
  // glActiveTexture(GL_TEXTURE0);
  // glBindTexture(GL_TEXTURE_CUBE_MAP, texCube);

  // glEnableClientState(GL_VERTEX_ARRAY);
  // glVertexPointer(3, GL_FLOAT, 3 * sizeof(GL_FLOAT), positionData);

  // glEnableClientState(GL_INDEX_ARRAY);
  // glVertexPointer(1, GL_INT, sizeof(GL_INT), indexData);
  // glDrawArrays(GL_QUADS, 0, 8);

  glPushMatrix();
  glTranslatef(outer * 3 / 4, -outer * 2 / 3, -outer);
  glRotatef(-90, 1, 0, 0);
  glColor3f(0.1, 0.1, 0.9);
  glutSolidCone(outer / 4, outer / 2, 20, 20);
  glPopMatrix();

  glRotatef(theta, 0, 1, 0);
  glTranslatef(0, 0, 0);
  glPushMatrix();
  glColor3f(1.0, 0, 1.0);
  glutWireTorus(inner, outer / 2, 30, 50);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(outer * 2 / 3, -outer, outer / 4);
  glRotatef(0, 1, 0, 0);
  glColor3f(0.0, 1.0, 0.0);
  glutWireTeapot(30);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(outer * 3 / 4, outer * 2 / 3, -outer / 5);
  glRotatef(theta, 0, 0, 1);
  glColor3f(0.0, 1.0, 1.0);
  glutWireSphere(outer / 2, 20, 20);
  glPopMatrix();
}

void drawground() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glColor3f(0.5, 0.5, 1.0);
  // ground
  for (int i = -outer; i < outer; i += inner)
    for (int j = -outer; j < outer; j += inner) {
      glBegin(GL_QUADS);
      glVertex3d(j, -outer, i);
      glVertex3d(j, -outer, i + inner);
      glVertex3d(j + inner, -outer, i + inner);
      glVertex3d(j + inner, -outer, i);
      glEnd();
    }

  // top
  for (int i = -outer; i < outer; i += inner)
    for (int j = -outer; j < outer; j += inner) {
      glBegin(GL_QUADS);
      glVertex3d(j, outer, i);
      glVertex3d(j, outer, i + inner);
      glVertex3d(j + inner, outer, i + inner);
      glVertex3d(j + inner, outer, i);
      glEnd();
    }
}

void drawwall() {
  // 绘制底面以及纹理
  glBindTexture(GL_TEXTURE_2D, texSky);
  glBegin(GL_QUADS);
  // Back
  glTexCoord2f(0.0f, 0.0f); //纹理坐标配置函数
  glVertex3f(-outer, -outer, -outer);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-outer, outer, -outer);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(outer, outer, -outer);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(outer, -outer, -outer);

  // Left
  glTexCoord2f(0.0f, 0.0f); //纹理坐标配置函数
  glVertex3f(-outer, -outer, -outer);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-outer, -outer, outer);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(-outer, outer, outer);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(-outer, outer, -outer);

  // Right
  glTexCoord2f(0.0f, 0.0f); //纹理坐标配置函数
  glVertex3f(outer, -outer, -outer);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(outer, -outer, outer);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(outer, outer, outer);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(outer, outer, -outer);

  // Down
  glTexCoord2f(0.0f, 0.0f); //纹理坐标配置函数
  glVertex3f(-outer, -outer, -outer);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-outer, -outer, outer);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(outer, -outer, outer);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(outer, -outer, -outer);

  // Up
  glTexCoord2f(0.0f, 0.0f); //纹理坐标配置函数
  glVertex3f(-outer, outer, -outer);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-outer, outer, outer);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(outer, outer, outer);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(outer, outer, -outer);

  glEnd();
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glColor3f(0.5, 0.0, 1.0);
  int i, j;

  // LEFT
  for (i = -outer; i < outer; i += inner)
    for (j = -outer; j < outer; j += inner) {
      glBegin(GL_QUADS);
      glVertex3d(-outer, j, i);
      glVertex3d(-outer, j + inner, i);
      glVertex3d(-outer, j + inner, i + inner);
      glVertex3d(-outer, j, i + inner);
      glEnd();
    }

  // right
  for (i = -outer; i <= outer; i += inner)
    for (j = -outer; j <= outer; j += inner) {
      glBegin(GL_QUADS);
      glVertex3d(outer, j, i);
      glVertex3d(outer, j + inner, i);
      glVertex3d(outer, j + inner, i + inner);
      glVertex3d(outer, j, i + inner);
      glEnd();
    }

  glColor3f(0.0, 0.5, 1.0);

  // front
  for (i = -outer; i <= outer; i += inner)
    for (j = -outer; j <= outer; j += inner) {
      glBegin(GL_QUADS);
      glVertex3d(j, i, -outer);
      glVertex3d(j + inner, i, -outer);
      glVertex3d(j + inner, i + inner, -outer);
      glVertex3d(j, i + inner, -outer);
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